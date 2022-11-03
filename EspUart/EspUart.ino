#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

// Comandos de resposta
unsigned char comResposta = 0x00;
unsigned char addrResposta = 0x00;

byte byte_read;
float voltagem;

// Definições de rede
IPAddress local_IP(10, 0, 0, 109);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// Nome do ESP na rede
const char* host = "ESP-10.0.0.109";


const char* ssid = STASSID;
const char* password = STAPSK;



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
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(A0, INPUT);
  code_uploaded();
  OTA_setup(); 
  byte_read = -1;
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(9600);
}

void loop() {
  ArduinoOTA.handle();

  if(Serial.available() > 0){
    byte_read = Serial.read();
    
  }


    switch(byte_read){
    case 0x03:
      byte_read = 0x00;
      Serial.write(byte_read);
      break;
    case 0x04: //analógica
      digitalWrite(LED_BUILTIN,HIGH);
      voltagem = analogRead(A0)*(3.3/1023.0);
      byte_read = 0x01;
      Serial.write(byte_read);
      Serial.print(voltagem);
      break;
    case 0x05:  //digitais
      if (digitalRead(D0)==0){
        byte_read = 0x02;
        Serial.write(byte_read);
      }else{
        byte_read = 0x08;
        Serial.write(byte_read);
      }
      break;
    case 0x11:  //digitais
      if (digitalRead(D1)==0){
        byte_read = 0x12;
        Serial.write(byte_read);
      }else{
        byte_read = 0x13;
        Serial.write(byte_read);
      }
      break;
    case 0x06:
      digitalWrite(LED_BUILTIN,LOW);
      byte_read = 0x50;
      Serial.write(byte_read);
      break;
    case 0x07:
      digitalWrite(LED_BUILTIN,HIGH);
      byte_read = 0x51;
      Serial.write(byte_read);
      break;
  }
}