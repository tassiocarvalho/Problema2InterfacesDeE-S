<h1 align="center">PROBLEMA 2 TEC 499 - Sitemas Digitas- Interface de entrada e saída</h1>
Dando prosseguimento ao problema 1 de um sistema digital baseado em um processador ARM, implementamos um protótipo de sistema de sensoriamento genérico, utilizando o NodeMCU para confecção das unidadesde sensoriamento analógicos e digitais. A comunicação UART entre a Raspberry Pi Zero o NodeMCU (ESP8266) foram essenciais, além de o LCD exibir os resultados.

<h1 align="center"> Sumário </h1>  

• <a href="#recursos">Recursos utilizados/a> 

• <a href="#script-de-compilacao">Script de compilação</a> 

• <a href="#metodologia-e-tecnicas-aplicadas">Metodologia e técnicas aplicadas no projeto</a> 

• <a href="#descricao-do-sistema">Descrição em alto nível do sistema proposto</a> 

• <a href="#descricao-do-protocolo-de-comunicacao">Descrição do protocolo de comunicação desenvolvido</a> 

• <a href="#descricao-e-analise-dos-testes">Descrição e análise dos testes e simuações</a> 

• <a href="#membros">Membros</a> 

• <a href="#referencias">Referências</a> 
  
<h1 id="recursos" align="center">Recursos utilizados</h1> 
• Arduino IDE: Nesta plataforma é possível realizar o envio do código desenvolvido na linguagem C, para a nodeMCU esp8266 através da comunicação via wifi.

• GNU Makefile: O makefile determina automaticamente quais partes de um programa grande precisam ser recompiladas e emite comandos para compilar novamente. Inicialmente deve ser escrito um arquivo chamado makefile que descreve os relacionamentos entre os arquivos do programa e fornece comandos para atualizar cada arquivo. Em um programa, normalmente, o arquivo executável é atualizado a partir de arquivos de objeto, que por sua vez são feitos pela compilação de arquivos de origem Uma vez que existe um makefile adequado, cada vez que alguns arquivos de origem são alterados, apenas o comando “make” é suficiente para realizar todas as recompilações necessárias.

• GNU Binutils: Software que se encontra o GNU assembler (as) que foi utilizado para montar os códigos assembly, além do GNU linker(ld) que combina vários arquivos objetos, realoca seus dados e vincula referências de símbolos, fazendo por último a criação do executável do programa.

• GDB: É o depurador de nível de fonte GNU que é padrão em sistemas linux (e muitos outros unix). O propósito de um depurador como o GDB é permitir ver o que está acontecendo “dentro” de outro programa enquanto ele é executado, ou o que outro programa estava fazendo no momento em que travou. Ele pode ser usado tanto para programas escritos em linguagens de alto nível como C e C++ quanto para programas de código assembly.
  
• Raspberry Pi Zero: Módulo responsável pelo sistema operacional em que o problema deve ser feito e testado.

• Incluso no brotoboard além do Rapsberry também usamos: Display LCD: HD44780U (LCD-II); Botão tipo push-button

• Visual Studio Code: Software utilizado para melhor visualização do código e alterações do mesmo.
  

<h1 id="script-de-compilacao" align="center">Script de compilação</h1> 

<h1 id="metodologia-e-tecnicas-aplicadas" align="center">Metodologia e técnicas aplicadas no projeto</h1> 

<h1 id="descricao-do-sistema" align="center">Descrição em alto nível do sistema proposto</h1> 

<h1 id="descricao-do-protocolo-de-comunicacao" align="center">Descrição do protocolo de comunicação desenvolvido</h1> 

<h1 id="descricao-e-analise-dos-testes" align="center">Descrição e análise dos testes e simuações</h1> 

<h1 id="membros" align="center">Membros</h1> 

* <a href="https://github.com/AlanaSampaio">Alana Sampaio</a>  
* <a href="https://github.com/tassiocarvalho">Tassio Carvalho</a>

<h1 id="referencias" align="center">Referências</h1> 

