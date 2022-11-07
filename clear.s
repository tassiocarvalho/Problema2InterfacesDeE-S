.global clear

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

clear:

    setLcd 0, 0, 0, 0, 0
    delay timespecnano150
    setLcd 0, 0, 0, 0, 1
    delay timespecnano150

   BX LR

.data
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