<h1 align="center">PROBLEMA 2 TEC 499 - Sitemas Digitas- Interface de entrada e saída</h1>

<h2> Sumário </h2>  

• <a href="#introducao">Introdução</a> 

• <a href="#recursos">Recursos utilizados</a> 

• <a href="#script-de-compilacao">Script de compilação</a> 

• <a href="#metodologia-e-tecnicas-aplicadas">Metodologia e técnicas aplicadas no projeto</a> 

• <a href="#descricao-do-sistema">Descrição em alto nível do sistema proposto</a> 

• <a href="#descricao-do-protocolo-de-comunicacao">Descrição do protocolo de comunicação desenvolvido</a> 

• <a href="#descricao-e-analise-dos-testes">Descrição e análise dos testes e simuações</a> 

• <a href="#membros">Membros</a> 

• <a href="#referencias">Referências</a> 
  
<h1 id="introducao" align="center">Introdução</h1> 
  Dando prosseguimento ao problema 1 da disciplina sistema digital baseado em um processador ARM, implementamos um protótipo de sistema de sensoriamento genérico. Utilizando o NodeMCU (ESP8266) para confecção das unidades de sensoriamento analógicos e digitais. A comunicação UART entre a Raspberry Pi Zero o NodeMCU (ESP8266) foram essenciais, além de o LCD exibir os resultados melhorando a experiência do usuário.
Este sistema além de controlar os sensores faz o monitoramento dos resultados de forma automatizada tendo eficiência e eficácia nos seus resultados.
  
<h1 id="recursos" align="center">Recursos utilizados</h1> 
• Arduino: Nesta plataforma utilizamos para o envio do código desenvolvido na linguagem C, para a nodeMCU esp8266 através da comunicação via wifi.

• GNU Makefile: O makefile foi essencial para determinar automaticamente quais partes de um programa grande precisam ser recompiladas e emite comandos para compilar novamente. Ademais, foi possível através dele converter o problema anterior em formato de biblioteca para ser aproveitado neste novo sistema.

• GNU Binutils: Software que se encontramos o GNU assembler (as) que foi utilizado para montar os códigos assembly, além do GNU linker(ld) que combina vários arquivos objetos, realoca seus dados e vincula referências de símbolos, fazendo por último a criação do executável do programa.

• GDB: É o depurador de nível de fonte GNU que é padrão em sistemas linux (e muitos outros unix). O propósito de um depurador como o GDB é permitir ver o que está acontecendo “dentro” de outro programa enquanto ele é executado, ou o que outro programa estava fazendo no momento em que travou. Ele pode ser usado tanto para programas escritos em linguagens de alto nível como C e C++ quanto para programas de código assembly.
  
• Raspberry Pi Zero: Módulo responsável pelo sistema operacional em que o problema deve ser feito e testado.

• Incluso no brotoboard além do Rapsberry também usamos: Display LCD: HD44780U (LCD-II); Botão tipo push-button

• Visual Studio Code: Software utilizado para melhor visualização do código e alterações do mesmo.
  

<h1 id="script-de-compilacao" align="center">Script de compilação</h1> 
Indispensável para conseguir executar o programa é ter em mãos Raspberry Pi Zero, a plataforma Arduino e o NodeMCU.
  • Para executar é necessário clonar o projeto na sua máquina usando o comando a seguir;
  
```
$ https://github.com/tassiocarvalho/Problema2InterfacesDeEntrada-Saida.git
```
  
• Transfira os seguintes arquivos baixados para o Raspberry Pi Zero;

```
clear.s
escreverLCD.s
iniciarLCD.s
main.c
makefile
mapear.s
```

• A pasta nomeada como *EspUart* deverá ser aberta no software Arduino e em seguida compilado na plataforma e faz o upload.

• Por fim, execute os seguintes comandos: 

```
$ make
```

```
$ sudo ./main
```

<h1 id="metodologia-e-tecnicas-aplicadas" align="center">Metodologia e técnicas aplicadas no projeto</h1> 

<h1 id="descricao-do-sistema" align="center">Descrição em alto nível do sistema proposto</h1> 
O código solicitado no problema pediu um protótipo de um sistema sensorial genérico, utilizando da NodeMCU para confecção das unidades de sensoriamento, podendo serem utilizados tanto sensores digitais quanto analógicos tendo a possibilidade de permitir a substituição. Ademais, o sistema deve ser capaz de controlar o acionamento de sensores como também de fazer o seu monitoramento de forma automatizada, sendo essencial utilizar a comunicação UART para o processo.
O protótipo entregue neste trabalho atende os requisitos principais como ser capaz de interligar até 32 sensores, informa o status de funcionamento, somente o SBC é capaz de iniciar a comunicação, os comandos são compostos por palavras de 8 bits e as requisições por 2 bytes, além dos resultados estarem disponíveis no LCD facilitando a visualização do usuário dos resultados.


<h1 id="descricao-do-protocolo-de-comunicacao" align="center">Descrição do protocolo de comunicação desenvolvido</h1> 
O protocolo de comunicação final consiste em:


<h1 id="descricao-e-analise-dos-testes" align="center">Descrição e análise dos testes e simuações</h1> 

<h1 id="membros" align="center">Membros</h1> 

* <a href="https://github.com/AlanaSampaio">Alana Sampaio</a>  
* <a href="https://github.com/tassiocarvalho">Tassio Carvalho</a>

<h1 id="referencias" align="center">Referências</h1> 
[Stephen Smith - Raspberry Pi Assembly Language Programming](https://link.springer.com/book/10.1007/978-1-4842-5287-1)


[HD44780U (LCD-16x2)](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

[Datasheet - ESP8266](https://www.alldatasheet.com/view.jsp?Searchword=ESP8266&sField=4&gclid=Cj0KCQiAmaibBhCAARIsAKUlaKRSP5JzpmlF9JPnfCkdjKYD79a6Dcb_OL1NOG1STKnfcAP_e4Yg6s4aAjbzEALw_wcB)

