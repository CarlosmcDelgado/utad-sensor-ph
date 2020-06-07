/*
  Carlos Delgado 
  Maria Inês

  SENSOR PH
*/

#include <MKRWAN.h>

LoRaModem modem;

#include"arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;
void setup() {
 
  Serial.begin(115200);
  while (!Serial);

  if (!modem.begin(EU868)) {
    Serial.println("Falha ao iniciar o módulo");
    while (1) {}
  };
  Serial.print("A versão do seu módulo é: ");
  Serial.println(modem.version());
  Serial.print("O EUI do seu dispositivo é: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Algo deu errado");
    while (1){}
  }

  modem.minPollInterval(60);
}

 void loop() {
  Serial.println();
  Serial.println("Digite uma mensagem para enviar à rede");
  Serial.println("(verifique se 'NL' de final de linha está ativado)");

  while(!Serial.available());
  String msg = Serial.readStringUntil('\n');

  Serial.println();
  Serial.print("Sending:" + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++){
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
    }
  Serial.println();

  int err;
  modem.beginPacket();
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0){
    Serial.println("Mensagem enviada corretamente!");
    }else{
      Serial.println("Erro ao enviar mensagem :(");
      Serial.println("(Você pode enviar uma quantidade limitada de mensagens por minuto, dependendo da intensidade do sinal)");
      Serial.println("Pode variar de 1 mensagem a cada dois segundos");
         }
   
  delay(1000);
  if (!modem.available()){
      Serial.println("Nenhuma mensagem de downlink recebida no momento.");
      return;
      }
  char rcv[64];
  int i = 0;
  while (modem.available()){
        rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  for (unsigned int j = 0; j < i; j++){
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX );
    Serial.print(" ");
  }
  Serial.println();
     
}
