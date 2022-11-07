.equ sys_open, 5        @Valor da syscall de abertura de arquivos
.equ sys_map, 192       @Valor da syscall de mapeamento de memória (gera endereço virtual)
.equ page_len, 4096     @Tamanho da memória a ser utilizada para mapear, em número de páginas de memória (4096 = 1 página)
.equ prot_read, 1       @Flag para modo de leitura do arquivo "\dev\mem"
.equ prot_write, 2      @Flag para modo de escrita do arquivo "\dev\mem"
.equ map_shared, 1      @Libera compartilhamento de memória

.global iniciarLcd

.macro delay time       @Receve como parâmetro o tempo em segundos ou nanosegundos
    LDR R0, =\time      @Carrega em R0 o tempo que o processador deve "dormir"
    LDR R1, =\time
    MOV R7, #162        @Move para R7 (Registrador que define qual Syscall será executada) o valor 162 (valor da syscall nanosleep)
    SWI 0               @Executa a Syscall
.endm

.macro setLvl pin, lvl      @Recebe como parâmero as informações do pino e qual nível lógica colocar no pino
    MOV R0, #40             @Move #40 para R0 (40 é o offset do clear register)
    MOV R2, #12             @Move #12 para R2 (12 é a diferença entre os offsets do clear e do set registers)
    MOV R1, \lvl            @Move para R1 o valor do nível lógico desejado
    MUL R5, R1, R2          @Multiplica 12 pelo nível lógico recebido
    SUB R0, R0, R5          @Subtrai 40 pelo resultado obtido na operação anterior
    MOV R2, R8              @Move o endereço base dos GPIO obtido no mapeamento para o R2
    ADD R2, R2, R0          @Soma a esse endereço o offset calculado nas operações anteriores, podendo ser 28 (set register) ou 40 (clear register)
    MOV R0, #1              @Move #1 para R0
    LDR R3, =\pin           @Carrega o endereço de memória contendo o offset do GPFSel em R3
    ADD R3, #8              @Adiciona 8 a esse endereço, para obter o endereço que contém a posição do bit responsável por definir o nível lógico daquele pino específico
    LDR R3, [R3]            @Carrega o valor contido nesse endereço em R3
    LSL R0, R3              @Desloca o bit colocado em R0 para a posição obtida na operação anterior
    STR R0, [R2]            @Armazena no registrador de clear ou set o nível lógico do pino atualizado
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
    LDR R0, =fileName       @Carrega em R0 o endereço que contém o nome do arquivo ("\dev\mem")
    MOV R2, #0x1b0
    ORR R2, #0x006          @Armazena em R2 o hexadecimal #0x1b6 para determinar os modos de abertura (usamos no modo de leitura e escrita)
    MOV R1, R2
    MOV R7, #sys_open       @Armazena em R7 o valor da Syscall 5 (Para abertura de arquivos)
    SWI 0                   @A syscall é executada
    MOVS R4, R0             @A syscall retorna em R0 o file descriptor (será usado no mapeamento), o file descriptor foi movido para R4

    LDR R5, =gpioaddr       @Carrega em R5 o endereço de memória que contem o endereço base dos GPIO em páginas de memória (0x20200000 \ 4096 = 0x20200)
    LDR R5, [R5]            @carrega em R5 o endereço dos GPIO (0x20200)
    MOV R1, #page_len       @Move para R1 a quantidade de memória a usar em páginas de memória (foi usado 4096 bytes ou 1 página de memória)
    MOV R2, #(prot_read + prot_write)   @Move para R2 os modos de acesso ao arquivo (leitura e escrita)
    MOV R3, #map_shared     @Define que a memória será compartilhada
    MOV R0, #0              @Define que o SO poderá definir qual endereço de memória virtual será usado para mapear
    MOV R7, #sys_map        @Define a syscall de mapeamento no R7
    SWI 0                   @Executa a syscall de mapeamento
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