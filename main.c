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

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //#baundrate de 9600, #palabras de 8 bits, #permite leitura
    options.c_iflag = IGNPAR; //ignora bit de paridade
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH); //Funcao que descarta dados da descricao do arquivo que nao foram lidos
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void commando_tx(unsigned char com, unsigned char addr){ //Funcao para envio dos bytes, tem como parametro o byte do comando e o de endereco

    unsigned char tx_buffer[10]; //Cria array de bytes
    unsigned char *p_tx_buffer; //Cria ponteiro para apontar pra array

    p_tx_buffer = &tx_buffer[0]; //Ponteiro apontando para primeiro item da lista, de indice 0
    *p_tx_buffer++ = com; //No indice 1 coloca o parametro comando
    *p_tx_buffer++ = addr; //No indice 2 coloca o parametro endereco

    if (uart0_filestream != -1){ //Se encontrar o arquivo
        int cont = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0])); //Chama a funcao que enviar os dados via serial tendo como parametro a descricao do arquivo e quantidade de bytes

    } else { //Se não encontrar o arquivo printa o erro
        printf("Erro no envio de dados\n");
    }
}

unsigned char comando_rx(){ //Funcao responsavel por receber os dados via serial
    unsigned char rx_buffer[100]; //Criacao do buffer que recebe os dados
    int rx_length = read(uart0_filestream, (void*)rx_buffer, 100); //Chama funcao de leitura tendo como parametro a descricao do arquivo, um ponteiro que aponta o o primeiro elemento do buffer e o tamanho do mesmo

    if (rx_length < 0){ //Se houver problema na leitura retornará -1, printa o erro e escreve o problema no LCD
        printf("Erro na leitura\n");
        escrever_char("Status: Problema");
    }
    else if(rx_length == 0){ //Se não houver erro mas não tiver dados para ler printa a mensagem
        printf("Nenhum dado disponível\n");
    }
    else{ //Se não houver erro e tiver dados
        rx_buffer[rx_length] = '\0'; //Coloca o byte '\0' para finalizar a string no segundo byte
    }

    if(rx_buffer[0] == 0x00){ //Verifica se o NodeMCU esta recebendo dados do ESP
    escrever_char("Status: ok!"); //Escreve 'status: ok' no LCD
    }else if(rx_buffer[0] == 0x1F){ //Verifica se o NodeMCU esta com problema
    escrever_char("Status: Problema"); //Escreve o 'status:problema' no LCD
    }else if(rx_buffer[0] == 0x50){ //Verifica se a LED do NodeMCU esta acesa
    escrever_char("Led acesa"); //Escreve 'led acesa' no LED
    }else if(rx_buffer[0] == 0x51){ //Verifica se a LED do NodeMCU esta apagada
    escrever_char("Led apagada"); //Escreve 'led apagada' no LCD
    }else if(rx_buffer[0] == 0x02){ //Verifica se o sensor esta com nivel logico baixo
    escrever_char("LVL Sensor: 0"); //Escreve 'lvl sensor: o' no LCD
    }else if(rx_buffer[0] == 0x08){ //Verifica se o sensor esta com nivel logico alto
    escrever_char("LVL Sensor: 1"); //Escreve 'lvl sensor: 1' no LCD
    }else if(rx_buffer[0] == 0x01){ //Verifica se o comando de resposta é o código do sensor analógico
    escrever_char("Voltagem:"); //Escreve 'voltagem' no LCD
    rx_buffer[0] = ' '; //Substitui o primeiro elemento do buffer que é um '0x01' por um ' '
    escrever_char(rx_buffer); //Es reve o valor da ESP no LCD
    }
}

unsigned char addr(){ //Funcao para obter o endereco do sensor escolhido pelo usuario
        int valor = 0; //Variavel que aarmazena o valor que o usuario inserir
        printf("\n\nEscolha o sensor: \n"); //Menu de opcoes
        printf("[1] -> Sensor D0: \n");
        printf("[2] -> Sensor D1: \n");
        printf("[3] -> Sensor D2: \n");
        printf("[4] -> Sensor D3: \n");
        printf("[5] -> Sensor D4: \n");
        printf("[6] -> Sensor D5: \n");
        printf("[7] -> Sensor D6: \n");
        printf("[8] -> Sensor D7: \n");
        scanf("%d", &valor); //Leitura de dados do usuario
        switch(valor){ //A partir da opcao escolhida pelo usuario retornara uma das opcoes a seguir
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
        default:{ //Caso o usuario digite um valor que nao esta no menu de opcoes a mensagem 'valor invalido' aparecera
                printf("Valor inválido\n\n");
                break;
                }
        }
}

int main(int argc, const char * argv[]){ //Funcao principal do codigo

        mapear(); //Inicializando as bibliotecas importadas anteriormente
        iniciarLcd();
        setting_uart();
        escrever_char("    PBL-SD");

        int valor; //Variavel que armazena o valor digitado pelo usuario
        int enquanto = 1; //Condicao para o loop

        while(enquanto){
        printf("\n          Seleciona a operação:\n\n"); //Menu de opcoes a serem escolhidos pelo usuario
        printf("#----------------------------------------#\n");
        printf("[1] -> Estado do NodeMCU;\n");
        printf("[2] -> Estado do sensor analogico; \n");
        printf("[3] -> Estado do sensores digitais;\n");
        printf("[4] -> Ligar Led;\n");
        printf("[5] -> Apagar Led;\n");
        printf("[7] -> Sair.\n");
        printf("#----------------------------------------#\n");
        scanf("%d", &valor); //Entrada de dados do usuario

        if(valor == 6 ){ //Opcao para sair do programa
            break;
        }

        switch(valor){
            case 1:{ //Informa situacao NodeMCU
                commando_tx(GET_NODEMCU_SITUACAO, 0); //Funcao que envia o comando e o endereço
                clear(); //Funcao limpa o LCD
                sleep(2); //Delay de 2 segundos
                comando_rx(); //Funcao recebe os dados
                break;
            }
            case 2:{ //Informa sinal analogico do NodeMCU
                commando_tx(GET_NODEMCU_ANALOGICO_INPUT, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 3:{ //Informa sinal digital do NodeMCU
                unsigned char digital_addr = addr(); //Obtem o endereco do sensor digital especificado pelo usuario e envia como parametro a seguir
                commando_tx(GET_NODEMCU_DIGITAL_INPUT, digital_addr);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 4:{ //Liga led do NodeMCU
                commando_tx(SET_ON_LED_NODEMCU, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }
            case 5:{ //Desliga led do NodeMCU
                commando_tx(SET_OFF_LED_NODEMCU, 0);
                clear();
                sleep(2);
                comando_rx();
                break;
            }

            default: { //Finaliza operacao poe ser opcao invalida
                printf("Operação finalizada!\n");
                break;
            }
        }

    }

    close(uart0_filestream); //Fechando arquivo do terminal UART
    return 0;
}