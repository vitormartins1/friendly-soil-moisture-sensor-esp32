#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <WidgetRTC.h>

// BLYNK AUTHENTICATION
char auth[] = "seu blynk auth";
char ssid[] = "seu wifi";
char pass[] = "sua senha wifi";

// DHT TEMPERATURE AND HUMIDIDTY
#define DHTPIN 15
#define DHT11PIN 5
#define DHTTYPE DHT22
#define DHTSUPERIORTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
DHT dhtSuperior(DHT11PIN, DHTSUPERIORTYPE);

// TIMER, ALARMS AND RTC
BlynkTimer timer;
WidgetRTC rtc;

// RELAY
const int LED_50W_RELAY_PIN = 26;
const int LED_36W_RELAY_PIN = 25;
const int LED_12W_RELAY_PIN = 33;

// SOIL MOISURE SENSOR
const int VASO_A_PIN = 35;  // fio verde
const int VASO_B_PIN = 34;  // fio azul
const int VASO_C_PIN = 32;  // fio roxo
const int VASO_D_PIN = 36;  // fio marrom
float dry = 3500;
float wet = 1455;
float dryA = 2500;
float wetA = 1500;
float dryB = 2500;
float wetB = 1300;
float dryC = 2500;
float wetC = 1450;
float dryD = 2150;
float wetD = 1350;
float dryMap = 0.0;
float wetMap = 100.0;
float vasoA = 0;
float vasoB = 0;
float vasoC = 0;
float vasoD = 0;

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

void alarmTimerEvent() {
  digitalClockDisplay();
  Alarm.delay(0);

  // getting alarms in millis i think..
  String tempstr = String(Alarm.read(0));
  Serial.println(tempstr);
  String tempstr2 = String(Alarm.read(1));
  Serial.println(tempstr2);
  
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  Blynk.virtualWrite(V0, currentTime);
}

BLYNK_CONNECTED() {
  rtc.begin();
  Alarm.delay(1000);
}

BLYNK_WRITE(V7) {
  TimeInputParam t(param);

  // Process start time
  if (t.hasStartTime()) {
    Alarm.free(0);
    Alarm.alarmRepeat(t.getStartHour(),
                      t.getStartMinute(),
                      t.getStartSecond(), 
                      LowerGrowInitialAlarm);
    
    Serial.println(String("Start: ") + t.getStartHour() + ":" + t.getStartMinute() + ":" + t.getStartSecond());
  }

  // Process stop time
  if (t.hasStopTime()) {
    Alarm.free(1);
    Alarm.alarmRepeat(t.getStopHour(),
                      t.getStopMinute(),
                      t.getStopSecond(), 
                      LowerGrowFinalAlarm);
    
    Serial.println(String("Stop: ") + t.getStopHour() + ":" + t.getStopMinute() + ":" + t.getStopSecond());
  }
}

BLYNK_WRITE(V8) {
  TimeInputParam t(param);

  // Process start time
  if (t.hasStartTime()) {
    Alarm.free(2);
    Alarm.alarmRepeat(t.getStartHour(),
                      t.getStartMinute(),
                      t.getStartSecond(), 
                      UpperGrowInitialAlarm);
    
    Serial.println(String("Start: ") + t.getStartHour() + ":" + t.getStartMinute() + ":" + t.getStartSecond());
  }

  // Process stop time
  if (t.hasStopTime()) {
    Alarm.free(3);
    Alarm.alarmRepeat(t.getStopHour(),
                      t.getStopMinute(),
                      t.getStopSecond(), 
                      UpperGrowFinalAlarm);
    
    Serial.println(String("Stop: ") + t.getStopHour() + ":" + t.getStopMinute() + ":" + t.getStopSecond());
  }
}

void dhtTimerEvent() {
  // dht 22 inferior
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  // dht 11 inferior
  float hS = dhtSuperior.readHumidity();
  float tS = dhtSuperior.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(hS) || isnan(tS)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V3, tS);
  Blynk.virtualWrite(V4, hS);
}

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  setSyncInterval(10 * 60);   // Sync interval in seconds (10 minutes)
//  timer.setInterval(1000L, dhtTimerEvent);       // Setup a function to be called every second
//  timer.setInterval(5000L, alarmTimerEvent);     // Display digital clock every 5 seconds
//  timer.setInterval(10000L, soilTimerEvent);     // solo event 10 seconds
  setTime(hour(),minute(),second(),month(),day(),year());
  Serial.print("setup RTC Updated Time ");
  timer.setInterval(1000L, dhtTimerEvent);       // Setup a function to be called every second
  timer.setInterval(5000L, alarmTimerEvent);     // Display digital clock every 5 seconds
  timer.setInterval(10000L, soilTimerEvent);     // solo event 10 seconds
  digitalClockDisplay();

  dht.begin();          // Setup DHT22
  dhtSuperior.begin();  // Setup DHT11

  setupAlarms();
  setupRelays();
}

void loop() {
  Blynk.run();
  timer.run();
}

void LowerGrowInitialAlarm() {
  digitalWrite(LED_50W_RELAY_PIN, HIGH);
}

void LowerGrowFinalAlarm() {
  digitalWrite(LED_50W_RELAY_PIN, LOW);
}

void UpperGrowInitialAlarm() {
  digitalWrite(LED_36W_RELAY_PIN, HIGH);
  digitalWrite(LED_12W_RELAY_PIN, HIGH);
}

void UpperGrowFinalAlarm() {
  digitalWrite(LED_36W_RELAY_PIN, LOW);
  digitalWrite(LED_12W_RELAY_PIN, LOW);
}

void setupAlarms() {
  Blynk.virtualWrite(V1, "Not defined");
  Blynk.virtualWrite(V2, "Not defined");
  Blynk.virtualWrite(V9, "Not defined");
  Blynk.virtualWrite(V10, "Not defined");
  
  Alarm.alarmRepeat(0, 0, 0, LowerGrowInitialAlarm);
  Alarm.disable(0);
  
  Alarm.alarmRepeat(0, 0, 0, LowerGrowFinalAlarm);
  Alarm.disable(1);
  
  Alarm.alarmRepeat(0, 0, 0, UpperGrowInitialAlarm);
  Alarm.disable(2);
  
  Alarm.alarmRepeat(0, 0, 0, UpperGrowFinalAlarm);   
  Alarm.disable(3);
}

void setupRelays() {
  pinMode(LED_50W_RELAY_PIN, OUTPUT);
  digitalWrite(LED_50W_RELAY_PIN, LOW);
  
  pinMode(LED_36W_RELAY_PIN, OUTPUT);
  digitalWrite(LED_36W_RELAY_PIN, LOW);

  pinMode(LED_12W_RELAY_PIN, OUTPUT);
  digitalWrite(LED_12W_RELAY_PIN, LOW);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

float capValue(float value) {
  if (value > 100) {
    return 100;
  } else if (value < 0) {
    return 0;
  } else {
    return value;
  }
}

void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
