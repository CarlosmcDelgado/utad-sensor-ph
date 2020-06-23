# Sensor pH

> Para mais fácil leitura deste ficheiro, usar o vscode (ou similar) e colocar em modo "__preview__".

## Obter o EUI da placa

O código necessário para essa tarefa esta na pasta _get_eui_

__Resultado:__

```plan
Your device EUI is: 70B3D57ED002BDB3
```

## Register Devices

```plan
Application ID: sensorph
Device EUI:70B3D57ED002BDB3
```

__Resultado:__

```js
const char *appEui = "70B3D57ED002BDB3";
const char *appKey = "ABE4C6BD14B088059BC3821BFA09536F";
```

## Medições

O código encontra-se nos ficheiros `Sensor_pH.ino` e `arduino_secrets.h`

Como não tenho sensor o meu estudo foi baseado neste sensor:
[ph sensor Arduino, how do ph sensors work, application of ph meter, ph sensor calibration](https://l.facebook.com/l.php?u=https%3A%2F%2Fwww.electroniclinic.com%2Fph-sensor-arduino-how-do-ph-sensors-work-application-of-ph-meter-ph-sensor-calibration%2F%3Ffbclid%3DIwAR0yZ8gWZA3pGury7-Da0lxKvcQJ_ngAyi7NTKs2I3Bqw4DwT8Ys7A-4b6g&h=AT0s3DiNV5qM0nEPs7Pb15BrJRur8h-_yhr4MnEKi-5SiwkrJ_SL-0CPaXKASustfwJ_gYqCiBGgjp7fn1pInczWnbPk2SmgcEw0kuX-J5yZeA4h3bJWWbrkiIgikMO8QWAQqA)

### Obter valor

Apresentação das partes mais importantes do código e uma breve explicação de cada uma.

```c
  // Processamento da leitura - Calculo da media de ph
  voltage = analogRead(A0);
  avgV = avgV * 0.6 + voltage * 0.4;

  // Enviar valores para "the network of things" de minuto a minuto (60000ms)
  if (millis() - timeUpload > 60000)
  {

    // Tratamento de dados
    int aux = (int)avgV * (5.0 / 1023.0) * 3.5 * 10 + 0.55; 
    txBuffer[0] = aux & 0xff;

    // Envio de dados
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

    // Redefinir temporizador
    timeUpload = millis();
  }
}
  
```

## Payload Formats

```js
function Decoder(bytes, port) {
  var decoded = {};
  
  if (port === 1) decoded.ph = ((bytes[0]) / 10).toFixed(1);

  return{
    field1:decoded.ph
  }
}
```

### Teste do Payload

__pH:__ 65

```plan
65
```

### Resultado

__Valor previsto:__ 10.1

```json
{
  "field1": "10.1"
}
```

## Trabalho Realizado por

- __Carlos Delgado - _66451___

- __Maria Inês - _66426___
