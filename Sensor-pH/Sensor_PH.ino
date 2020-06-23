/*
Carlos Delgado
Maria Inês
*/

#include <MKRWAN.h>
#include "arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

// Select your region (AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band region = EU868;

LoRaModem modem(Serial1);

unsigned long timeUpload;
uint16_t voltage = 0;
float avgV = 0;
uint8_t txBuffer[1];

void setup()
{

  Serial.begin(115200);
  pinMode(A0, INPUT);
  time = millis();
  timeUpload = millis();

  while (!Serial)
  {
  }

  if (!modem.begin(region))
  {
    Serial.println("Falha ao iniciar o módulo");
    while (1)
    {
    }
  };

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected)
  {
    Serial.println("Algo deu errado; você é interior? Mova-se para perto de uma janela e tente novamente");
    while (1)
    {
    }
  }

  modem.minPollInterval(60);
  // NOTA: modem só permite enviar msg no mínimo a cada 2 min
}

void loop()
{

  // Processar leitrura - calculo da media de ph
  voltage = analogRead(A0);
  avgV = avgV * 0.6 + voltage * 0.4;
  
  // Send values to "the network of things" minuto a minuto (60000ms)
  if (millis() - timeUpload > 60000)
  {
    int aux = (int)avgV * (5.0 / 1023.0) * 3.5 * 10 + 0.55; 
    txBuffer[0] = aux & 0xff;
    //Serial.println(txBuffer[0], HEX);

    // Sending data
    int err;
    modem.beginPacket();
    modem.write(txBuffer);
    err = modem.endPacket(true);
    if (err > 0)
    {
      Serial.println("Mensagem enviada corretamente!");
    }
    else
    {
      Serial.println("Erro ao enviar mensagem:(");
      Serial.println("(Você pode enviar uma quantidade limitada de mensagens por minuto, dependendo da intensidade do sinal");
      Serial.println("Pode variar de 1 mensagem a cada dois segundos a 1 mensagem a cada minuto)");
    }

    // Reset timer
    timeUpload = millis();
  }
}
