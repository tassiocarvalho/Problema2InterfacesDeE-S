#include <stdio.h> //Importando bibliotecas (linha 1 a 7)
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define GET_NODEMCU_SITUACAO 0x03 //Definindo um valor que nao pode ser alterado, nesse caso a situacao do NodeMCU
#define GET_NODEMCU_ANALOGICO_INPUT 0x04 //Define constante do sinal analogico do NodeMCU
#define GET_NODEMCU_DIGITAL_INPUT 0x05 //Define constante do sinal digital do NodeMCU
#define SET_ON_LED_NODEMCU 0x06 //Constante que liga a LED do NodeMCU
#define SET_OFF_LED_NODEMCU 0x07 //Constante que desliga a LED do NodeMCU

extern void mapear(); //Biblioteca que possibilita mapear o LCD
extern void iniciarLcd(); //Biblioteca responsavel por iniciar o LCD
extern void clear(); //Biblioteca responsavel por limpar o LCD
extern void escreverLcd(int a); //Biblioteca responsavel por escrever caracteres no LCD

int uart0_filestream = -1; // Define tipo e valor a variavel uart0_filestream

void escrever_char(char word[]){ //Funcao responsavel por escrever caracter no LCD

    for (int i=0; i<strlen(word); i++){ //Percorre a string
        escreverLcd(word[i]); //Escreve no LCD
    }
}

void setting_uart(){ //Funcao responsavel por configurar o UART

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY); //Abre arquivo da UART e recebe sua descrição

    if(uart0_filestream == -1){ //uart0_filestream retorna -1 caso não encontre o arquivo e informa o erro
        printf("Erro na abertura da UART\n");
    }

    struct termios options; //Cria uma struct para configurar o funcionamento da UART
    tcgetattr(uart0_filestream, &options); //Obtem os parametros associados ao descritor de arquivo e os armazena na struct termios criado anteriormente

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void commando_tx(unsigned char com, unsigned char addr){
    //printf("com: %d\n", com);
    //printf("addr: %d\n", addr);
    unsigned char tx_buffer[10];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = com;
    *p_tx_buffer++ = addr;

if (uart0_filestream != -1){
    int cont = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        //printf("cont: %d\n", cont);
    if(cont < 0){
        printf("Erro no envio de dados\n");
                }
        }
}

unsigned char comando_rx(){
    unsigned char rx_buffer[100];
    int rx_length = read(uart0_filestream, (void*)rx_buffer, 100);

        //printf("tamanho do buffer: %d\n", rx_length);
    if (rx_length < 0){
        printf("Erro na leitura\n");
        escrever_char("Status: Problema");
    }
    else if(rx_length == 0){
        printf("Nenhum dado disponível\n");
    }
    else{
        rx_buffer[rx_length] = '\0';
        //printf("%s",rx_buffer);
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
        escrever_char("LVL Sensor: 0");
        }else if(rx_buffer[0] == 0x08){
        escrever_char("LVL Sensor: 1");
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
        printf("[4] -> Sensor D3: \n");
        printf("[5] -> Sensor D4: \n");
        printf("[6] -> Sensor D5: \n");
        printf("[7] -> Sensor D6: \n");
        printf("[8] -> Sensor D7: \n");
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
        case 4:{
                return 0x21;
        }
        case 5:{
                return 0x22;
        }
        case 6:{
                return 0x23;
        }
        case 7:{
                return 0x24;
        }
        case 8:{
                return 0x25;
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