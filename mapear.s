.equ sys_open, 5        @Valor da syscall de abertura de arquivos
.equ sys_map, 192       @Valor da syscall de mapeamento de memória (gera endereço virtual)
.equ page_len, 4096     @Tamanho da memória a ser utilizada para mapear, em número de páginas de memória (4096 = 1 página)
.equ prot_read, 1       @Flag para modo de leitura do arquivo "\dev\mem"
.equ prot_write, 2      @Flag para modo de escrita do arquivo "\dev\mem"
.equ map_shared, 1      @Libera compartilhamento de memória

.global mapear

.macro setOut pin
        ldr r2, =\pin
        ldr r2, [r2]
        ldr r1, [r8, r2]
        ldr r3, =\pin
        add r3, #4
        ldr r3, [r3]
        mov r0, #0b111
        lsl r0, r3
        bic r1, r0
        mov r0, #1
        lsl r0, r3
        orr r1, r0
        str r1, [r8, r2]
.endm


mapear:

    ldr r0, =fileName
    mov r2, #0x1b0
    orr r2, #0x006
    mov r1, r2
    mov r7, #sys_open
    swi 0
    movs r4, r0

    ldr r5, =gpioaddr
    ldr r5, [r5]
    mov r1, #page_len
    mov r2, #(prot_read + prot_write)
    mov r3, #map_shared
    mov r0, #0
    mov r7, #sys_map
    swi 0
    movs r8, r0

    setOut rs
    setOut e
    setOut db4
    setOut db5
    setOut db6
    setOut db7

    bx lr

.data
fileName: .asciz "/dev/mem"     @Diretório usado para o mapeamento de memória
gpioaddr: .word 0x20200         @Endereço dos GPIO / 4096
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