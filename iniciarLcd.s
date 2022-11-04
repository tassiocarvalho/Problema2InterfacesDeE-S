.equ sys_open, 5        @Valor da syscall de abertura de arquivos
.equ sys_map, 192       @Valor da syscall de mapeamento de memória (gera endereço virtual)
.equ page_len, 4096     @Tamanho da memória a ser utilizada para mapear, em número de páginas de memória (4096 = 1 página)
.equ prot_read, 1       @Flag para modo de leitura do arquivo "\dev\mem"
.equ prot_write, 2      @Flag para modo de escrita do arquivo "\dev\mem"
.equ map_shared, 1      @Libera compartilhamento de memória

.global iniciarLcd

.macro delay time       @Receve como parâmetro o tempo em segundos ou nanosegundos
    ldr r0, =\time      @Carrega em R0 o tempo que o processador deve "dormir"
    ldr r1, =\time
    mov r7, #162        @Move para R7 (Registrador que define qual Syscall será executada) o valor 162 (valor da syscall nanosleep)
    swi 0               @Executa a Syscall
.endm

.macro setLvl pin, lvl      @Recebe como parâmero as informações do pino e qual nível lógica colocar no pino
    mov r0, #40             @Move #40 para R0 (40 é o offset do clear register)
    mov r2, #12             @Move #12 para R2 (12 é a diferença entre os offsets do clear e do set registers)
    mov r1, \lvl            @Move para R1 o valor do nível lógico desejado
    mul r5, r1, r2          @Multiplica 12 pelo nível lógico recebido
    sub r0, r0, r5          @Subtrai 40 pelo resultado obtido na operação anterior
    mov r2, r8              @Move o endereço base dos GPIO obtido no mapeamento para o R2
    add r2, r2, r0          @Soma a esse endereço o offset calculado nas operações anteriores, podendo ser 28 (set register) ou 40 (clear register)
    mov r0, #1              @Move #1 para R0
    ldr r3, =\pin           @Carrega o endereço de memória contendo o offset do GPFSel em R3
    add r3, #8              @Adiciona 8 a esse endereço, para obter o endereço que contém a posição do bit responsável por definir o nível lógico daquele pino específico
    ldr r3, [r3]            @Carrega o valor contido nesse endereço em R3
    lsl r0, r3              @Desloca o bit colocado em R0 para a posição obtida na operação anterior
    str r0, [r2]            @Armazena no registrador de clear ou set o nível lógico do pino atualizado
.endm

.macro setLcd lvlrs, lvldb7, lvldb6, lvldb5, lvldb4 @Recebe como parâmetro os níveis lógicos a serem definidos nos pinos RS, DB7, DB6, DB5, DB4

    setLvl rs, #\lvlrs          @Define o nível lógico de RS (0 para dar comandos e 1 para escrever na LCD)
    setLvl db7, #\lvldb7        @Define o nível lógico de DB7
    setLvl db6, #\lvldb6        @Define o nível lógico de DB6
    setLvl db5, #\lvldb5        @Define o nível lógico de DB5
    setLvl db4, #\lvldb4        @Define o nível lógico de DB4
    setLvl e, #0                @Define nível lógico "0" no Enable (fecha o envio de dados nos pinos de dados)
    delay timespecnano150       @Aplica um delay de 1.5 milisegundos
    setLvl e, #1                @Define nível lógico "1" no Enable (habilita o envio de dados nos pinos de dados)
    delay timespecnano150       @Aplica um delay de 1.5 milisegundos
    setLvl e, #0                @Define nível lógico "0" no Enable (fecha o envio de dados nos pinos de dados)
    .ltorg                      @Certifica que uma literal pool está dentro da range exigida (sem essa instrução, códigos muito grandes tendem a "estourar" o limite da literal pool)
.endm                           @Literal pool é a distância entre o valor atual (do reg. PC) da instrução exucutada no momento e o endereço da constante que uma instrução acessa,

iniciarLcd:
    ldr r0, =fileName       @Carrega em R0 o endereço que contém o nome do arquivo ("\dev\mem")
    mov r2, #0x1b0
    orr r2, #0x006          @Armazena em R2 o hexadecimal #0x1b6 para determinar os modos de abertura (usamos no modo de leitura e escrita)
    mov r1, r2
    mov r7, #sys_open       @Armazena em R7 o valor da Syscall 5 (Para abertura de arquivos)
    swi 0                   @A syscall é executada
    movs r4, r0             @A syscall retorna em R0 o file descriptor (será usado no mapeamento), o file descriptor foi movido para R4

    ldr r5, =gpioaddr       @Carrega em R5 o endereço de memória que contem o endereço base dos GPIO em páginas de memória (0x20200000 \ 4096 = 0x20200)
    ldr r5, [r5]            @carrega em R5 o endereço dos GPIO (0x20200)
    mov r1, #page_len       @Move para R1 a quantidade de memória a usar em páginas de memória (foi usado 4096 bytes ou 1 página de memória)
    mov r2, #(prot_read + prot_write)   @Move para R2 os modos de acesso ao arquivo (leitura e escrita)
    mov r3, #map_shared     @Define que a memória será compartilhada
    mov r0, #0              @Define que o SO poderá definir qual endereço de memória virtual será usado para mapear
    mov r7, #sys_map        @Define a syscall de mapeamento no R7
    swi 0                   @Executa a syscall de mapeamento
    movs r8, r0             @O endereço virtual gerado é retornado em R0, em seguida é movido para R8

    setLcd 0, 0, 0, 1, 1        @Comando de function set
    delay timespecnano5         @Delay de 5 milisegundos

    setLcd 0, 0, 0, 1, 1        @Comando de function set

    setLcd 0, 0, 0, 1, 1        @Comando de function set que muda o modo de operação da LCD para 4 bits
    setLcd 0, 0, 0, 1, 0

    setLcd 0, 0, 0, 1, 0        @Comando de function set que define número de linhas e a fonte a ser usada pelo LCD
    setLcd 0, 0, 0, 0, 0

    setLcd 0, 0, 0, 0, 0        @Comando que desliga o display
    setLcd 0, 1, 0, 0, 0

    setLcd 0, 0, 0, 0, 0        @Comando que limpa o display
    setLcd 0, 0, 0, 0, 1

    setLcd 0, 0, 0, 0, 0        @Comando de entry mode set (shift do cursor para direita e o endereço foi configurado para incremento)
    setLcd 0, 0, 1, 1, 0

    setLcd 0, 0, 0, 0, 0        @Comando para Ligar o display e o cursor
    setLcd 0, 1, 1, 1, 0

    setLcd 0, 0, 0, 0, 0        @Comando de entry mode set (shift do cursor para direita e o endereço foi configurado para incremento)
    setLcd 0, 0, 1, 1, 0

    bx lr

.data
fileName: .asciz "/dev/mem"     @Diretório usado para o mapeamento de memória
gpioaddr: .word 0x20200         @Endereço dos GPIO / 4096
timespecnano5: .word 0          @Delay de 5 milisegundos
                .word 5000000
timespecnano150: .word 0        @Delay de 1.5 milisegundos
                .word 1500000
time1s: .word 1                 @Delay de 1 segundo
        .word 000000000
value: .word 0x90204122         @Valor do contador
rs:  .word 8                    @Offset do GPFSel do pino RS
     .word 15                   @Posição dos bits de FSel do pino RS
     .word 25                   @Posição do bit para clear e set register do pino RS
e:   .word 0                    @Offset do GPFSel do pino Enable
     .word 3                    @Posição dos bits de FSel do pino Enable
     .word 1                    @Posição do bit para clear e set register do pino Enable
db4: .word 4                    @Offset do GPFSel do pino DB4
     .word 6                    @Posiçã o dos bits de FSel do pino DB4
     .word 12                   @Posição do bit para clear e set register do pino DB4
db5: .word 4                    @Offset do GPFSel do pino DB5
     .word 18                   @Posição dos bits de FSel do pino DB5
     .word 16                   @Posição do bit para clear e set register do pino DB5
db6: .word 8                    @Offset do GPFSel do pino DB6
     .word 0                    @Posição dos bits de FSel do pino DB6
     .word 20                   @Posição do bit para clear e set register do pino DB6
db7: .word 8                    @Offset do GPFSel do pino DB7
     .word 3                    @Posição dos bits de FSel do pino DB7
     .word 21                   @Posição do bit para clear e set register do pino DB7