# Tutorial: Como usar o sensor capacitivo de umidade do solo com ESP32

Esse tutorial irá mostrar como usar o sensor capacitivo de umidade do solo com o microcontrolador ESP32. O objetivo é fornecer um exemplo de como obter leituras precisas e amigáveis de umidade do solo usando um sensor capacitivo.

## Materiais necessários

- ESP32
- Sensor capacitivo de umidade do solo
- Jumpers
- Protoboard
- Cabo USB


## Como Funciona

O projeto consiste em utilizar um sensor capacitivo de umidade do solo para obter leituras precisas e confiáveis. O código faz 100 leituras e tira a média para ter uma leitura mais estável. Para isso, utilizamos um loop for que lê o valor analógico do sensor em quatro pinos diferentes, correspondentes a quatro sensores diferentes.

```cpp
for (int i = 0; i <= 100; i++) {
  vasoA = vasoA + analogRead(VASO_A_PIN);
  vasoB = vasoB + analogRead(VASO_B_PIN);
  vasoC = vasoC + analogRead(VASO_C_PIN);
  vasoD = vasoD + analogRead(VASO_D_PIN);
  Alarm.delay(1);
}
```

Depois, normalizamos o valor lido, dividindo-o por 100. Dessa forma, obtemos a média das 100 leituras.

```cpp
vasoA = vasoA/100;
vasoB = vasoB/100;
vasoC = vasoC/100;
vasoD = vasoD/100;
```

A próxima etapa é mapear o valor analógico do sensor para um valor mais amigável. Para isso, utilizamos a função `map()`, que recebe como parâmetros o valor lido, o valor mínimo, o valor máximo, o valor mínimo mapeado e o valor máximo mapeado. Por exemplo, se o valor lido está entre 4096 e 1, podemos mapeá-lo para um valor entre 0 e 100. O resultado dessa operação é atribuído a uma variável do tipo float.

```cpp
float mapVasoA = map(vasoA, dryA, wetA, dryMap, wetMap);
float mapVasoB = map(vasoB, dryB, wetB, dryMap, wetMap);
float mapVasoC = map(vasoC, dryC, wetC, dryMap, wetMap);
float mapVasoD = map(vasoD, dryD, wetD, dryMap, wetMap);
```

Por fim, o valor obtido é exibido no monitor serial.

## Montagem do circuito

A montagem do circuito é bastante simples. Conecte o ESP32 à protoboard e, em seguida, conecte o sensor capacitivo de umidade do solo aos pinos do ESP32 de acordo com o seguinte esquema:

```
Sensor -> ESP32
VCC -> 3.3V
GND -> GND
OUT -> GPIO 34
```

Certifique-se de que todos os componentes estão conectados corretamente antes de continuar.

## Configuração do ambiente de desenvolvimento

Antes de começar a programar, você precisará configurar o ambiente de desenvolvimento. Siga os seguintes passos:

1. Baixe o Arduino IDE em https://www.arduino.cc/en/software.
2. Instale o ESP32 na IDE do Arduino. Para isso, siga as instruções em https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/.
3. Conecte o ESP32 ao computador usando o cabo USB.

## Programação

Com o ambiente de desenvolvimento configurado, é hora de começar a programar. Siga os seguintes passos:

1. Abra o Arduino IDE e crie um novo sketch.
2. Copie e cole o código do arquivo "friendly_soil_moisture_sensor_esp32.ino" neste novo sketch.
3. Compile e envie o código para o ESP32.
4. Abra o Serial Monitor do Arduino IDE para visualizar as leituras de umidade do solo.

## Funcionamento

Com o programa carregado no ESP32 e o sensor capacitivo de umidade do solo conectado, você pode visualizar as leituras de umidade do solo no Serial Monitor do Arduino IDE. As leituras são exibidas em porcentagem, variando de 0% (muito seco) a 100% (muito úmido).

## Contribuição

Contribuições são sempre bem-vindas! Se você quiser adicionar alguma melhoria ou correção ao código, fique à vontade para enviar um pull request.
