# Friendly Soil Moisture Sensor esp32

Exemplo de como fazer leituras amigáveis e seguras com um sensor de solo capacitivo utilizando uma esp32.

'''
void soilTimerEvent() {
  vasoA = 0;
  vasoB = 0;
  vasoC = 0;
  vasoD = 0;

  // fazemos 100 leituras e pegamos a media para 
  // ter uma leitura mais estavel
  for (int i = 0; i <= 100; i++) {
    vasoA = vasoA + analogRead(VASO_A_PIN);
    vasoB = vasoB + analogRead(VASO_B_PIN);
    vasoC = vasoC + analogRead(VASO_C_PIN);
    vasoD = vasoD + analogRead(VASO_D_PIN);
    Alarm.delay(1);
  }

  vasoA = vasoA/100;
  vasoB = vasoB/100;
  vasoC = vasoC/100;
  vasoD = vasoD/100;

  // mapeamos o valor analogico para um valor mais amigavel
  float mapVasoA = map(vasoA, dryA, wetA, dryMap, wetMap);
  float mapVasoB = map(vasoB, dryB, wetB, dryMap, wetMap);
  float mapVasoC = map(vasoC, dryC, wetC, dryMap, wetMap);
  float mapVasoD = map(vasoD, dryD, wetD, dryMap, wetMap);

  mapVasoA = capValue(mapVasoA);
  mapVasoB = capValue(mapVasoB);
  mapVasoC = capValue(mapVasoC);
  mapVasoD = capValue(mapVasoD);

  Blynk.virtualWrite(V11, mapVasoA);
  Blynk.virtualWrite(V12, mapVasoB);
  Blynk.virtualWrite(V13, mapVasoC);
  Blynk.virtualWrite(V14, mapVasoD);
}
'''
