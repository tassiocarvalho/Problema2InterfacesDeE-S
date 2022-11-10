#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//Definicoes
#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

// Comandos de resposta
unsigned char comResposta = 0x00;
unsigned char addrResposta = 0x00;

//Variaveis
byte byte_com;
byte byte_addr;
float voltagem;

// Definições de rede
IPAddress local_IP(10, 0, 0, 109);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// Nome do ESP na rede
const char* host = "ESP-10.0.0.109";

//Constantes e variaveis
const char* ssid = STASSID;
const char* password = STAPSK;

//Funcao para ligar e desligar a LED do NodeMCU em loop
void code_uploaded(){
  for(int i=0;i<2;i++){
    digitalWrite(LED_BUILTIN,LOW);
    delay(150);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(150);
  }
}

void OTA_setup(){
  
  Serial.begin(115200);
  Serial.println("Booting");

  // Configuração do IP fixo no roteador, se não conectado, imprime mensagem de falha
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(host);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup() {
  //definindo pinos de entradas
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(A0, INPUT);
  code_uploaded();
  OTA_setup(); 
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(9600);
}

void loop() {
  ArduinoOTA.handle();

  if(Serial.available() > 0){
    byte_com = Serial.read();   //byte de comando
    delay(500);
    byte_addr = Serial.read();
  }

    //switch case de comando
    switch(byte_com){
    //caso que verifica se o comando de status ok do nodeMCU
    case 0x03:
      byte_com = 0x00;
      Serial.write(byte_com);    //escreve o byte de resposta ao raspberry
      break;
    case 0x04: //analógica
      digitalWrite(LED_BUILTIN,HIGH);
      voltagem = analogRead(A0)*(3.3/1023.0); //covertendo o valor de 1024 para voltagem 
      byte_com = 0x01;
      Serial.write(byte_com);
      Serial.print(voltagem);
      break;
    case 0x05:  //digitais
      switch(byte_addr){
        case 0x18:
          if (digitalRead(D0)==0){ //verificando o nivel logico do sensor
            byte_com = 0x02; //byte de comando para o Raspberry
            Serial.write(byte_com);
          }else{ //se for diferente de 0 manda outro byte de comando
            byte_com = 0x08; //byte de comando para o Raspberry
            Serial.write(byte_com);
          }
          break;
        case 0x19:
          if (digitalRead(D1)==0){ //verificando o nivel logico do sensor
            byte_com = 0x02; //byte de comando para o Raspberry
            Serial.write(byte_com);
          }else{ //se for diferente de 0 manda outro byte de comando
            byte_com = 0x08; //byte de comando para o Raspberry
            Serial.write(byte_com);
          }
          break;
        case 0x20:
          if (digitalRead(D2)==0){
            byte_com = 0x02;
            Serial.write(byte_com);
          }else{
            byte_com = 0x08;
            Serial.write(byte_com);
          }
          break;
        case 0x21:
          if (digitalRead(D3)==0){
            byte_com = 0x02;
            Serial.write(byte_com);
          }else{
            byte_com = 0x08;
            Serial.write(byte_com);
          }
          break;
          case 0x22:
          if (digitalRead(D4)==0){
            byte_com = 0x02;
            Serial.write(byte_com);
          }else{
            byte_com = 0x08;
            Serial.write(byte_com);
          }
          break;
          case 0x23:
          if (digitalRead(D5)==0){
            byte_com = 0x02;
            Serial.write(byte_com);
          }else{
            byte_com = 0x08;
            Serial.write(byte_com);
          }
          break;
          case 0x24:
          if (digitalRead(D6)==0){
            byte_com = 0x02;
            Serial.write(byte_com);
          }else{
            byte_com = 0x08;
            Serial.write(byte_com);
          }
          break;
          case 0x25:
          if (digitalRead(D7)==0){
            byte_com = 0x02;
            Serial.write(byte_com);
          }else{
            byte_com = 0x08;
            Serial.write(byte_com);
          }
          break;
      }
      break;
    case 0x06: //LED acesa
      delay(1);
      digitalWrite(LED_BUILTIN,LOW); //acende led
      byte_com = 0x50; //byte de comando para o Raspberry
      Serial.write(byte_com);
      break;
    case 0x07: //LED Apagada
      delay(1);
      digitalWrite(LED_BUILTIN,HIGH); //apaga led
      byte_com = 0x51; //byte de comando para o Raspberry
      Serial.write(byte_com);
      break;
  }
}