#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define GET_NODEMCU_SITUACAO 0x03
#define GET_NODEMCU_ANALOGICO_INPUT 0x04
#define GET_NODEMCU_DIGITAL_INPUT 0x05
#define SET_ON_LED_NODEMCU 0x06
#define SET_OFF_LED_NODEMCU 0x07

extern void mapear();
extern void iniciarLcd();
extern void clear();
extern void escreverLcd(int a);

int uart0_filestream = -1;

void delay(int seconds){
    int mili = 1000*seconds;
    clock_t start_time = clock();
    while(clock() < start_time + mili);
}

void escrever_char(char word[]){

    for (int i=0; i<strlen(word); i++){
        escreverLcd(word[i]);
    }
}

void setting_uart(){

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(uart0_filestream == -1){
        printf("Erro na abertura da UART\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void commando_tx(unsigned char com, unsigned char addr){
    printf("com: %d\n", com);
    printf("addr: %d\n", addr);
    unsigned char tx_buffer[10];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = com;
    *p_tx_buffer++ = addr;

if (uart0_filestream != -1){
    int cont = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        printf("cont: %d\n", cont);
    if(cont < 0){
        printf("Erro no envio de dados\n");
                }
        }
}

unsigned char comando_rx(){
    unsigned char rx_buffer[100];
    int rx_length = read(uart0_filestream, (void*)rx_buffer, 100);

        printf("tamanho do buffer: %d\n", rx_length);
    if (rx_length < 0){
        printf("Erro na leitura\n");
        escrever_char("Status: Problema");
    }
    else if(rx_length == 0){
        printf("Nenhum dado disponível\n");
    }
    else{
        rx_buffer[rx_length] = '\0';
        printf("%s",rx_buffer);
    }
        if(rx_buffer[0] == 0x00){
        escrever_char("Status: ok!");
        }else if(rx_buffer[0] == 0x1F){
        escrever_char("Status: Problema");
        }else if(rx_buffer[0] == 0x50){
        escrever_char("Led acesa");
        }else if(rx_buffer[0] == 0x51){
        escrever_char("Led apagada");
        }else if(rx_buffer[0] == 0x02){
        escrever_char("LVL Sensor: 1");
        }else if(rx_buffer[0] == 0x08){
        escrever_char("LVL Sensor: 0");
        }else if(rx_buffer[0] == 0x01){
        escrever_char("Voltagem:");
        rx_buffer[0] = ' ';
        escrever_char(rx_buffer);
        }
}

unsigned char addr(){
        int valor = 0;
        printf("\n\nEscolha o sensor: \n");
        printf("[1] -> Sensor D0: \n");
        printf("[2] -> Sensor D1: \n");
        printf("[3] -> Sensor D2: \n");
        scanf("%d", &valor);
        switch(valor){
        case 1:{
                return 0x18;
                }
        case 2:{
                return 0x19;
                }
        case 3:{
                return 0x20;
        }
        default:{
                printf("Valor inválido\n\n");
                break;
                }
        }
}
int main(int argc, const char * argv[]){

        mapear();
        iniciarLcd();
        setting_uart();
        escrever_char("    PBL-SD");

        int valor;
        int enquanto = 1;

        while(enquanto){
        printf("\n          Seleciona a operação:\n\n");
        printf("#----------------------------------------#\n");
        printf("[1] -> Estado do NodeMCU;\n");
        printf("[2] -> Estado do sensor analogico; \n");
        printf("[3] -> Estado do sensores digitais;\n");
        //printf("[4] -> Estado do Sensor digital D1;\n");
        printf("[4] -> Ligar Led;\n");
        printf("[5] -> Apagar Led;\n");
        printf("[6] -> Limpar display;\n");
        printf("[7] -> Sair.\n");
        printf("#----------------------------------------#\n");
        scanf("%d", &valor);

        if(valor == 7 ){
            break;
        }

        switch(valor){
            case 1:{
                commando_tx(GET_NODEMCU_SITUACAO, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 2:{
                commando_tx(GET_NODEMCU_ANALOGICO_INPUT, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 3:{
                unsigned char digital_addr = addr();
                commando_tx(GET_NODEMCU_DIGITAL_INPUT, digital_addr);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 4:{
                commando_tx(SET_ON_LED_NODEMCU, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 5:{
                commando_tx(SET_OFF_LED_NODEMCU, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 6:{
                clear();
                break;
            }

            default: {
                printf("Operação finalizada!\n");
                break;
            }
        }

    }

    close(uart0_filestream);
    return 0;
}