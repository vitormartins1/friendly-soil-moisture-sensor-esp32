/*
Codigo copiado do repositorio: https://github.com/bitluni/ESP32CamTimeLapse
Creditos ao bitluni pelo fantastico trabalho
Credits to bitluni for the awesome work. Thanks
Visite https://bitluni.net/esp32camtimelapse 
para mais informacoes do projeto

Copiado e colado sem alteracoes do repositorio do bitluni para o uso
de timelapses na esp32cam.

Futuramente devera ser integrado com o Blynk app para que nao tire 
fotos durante o periodo de escuridao, ja que a esp32cam aciona o flash
quando escreve dados no cartao de memoria, podendo assim interferir no 
fotoperiodo. 

Outra solucao seria durante o periodo escuro, ativar um rele que ligue
uma luz verde antes de bater a foto, como uma forma de  flash nao 
invasivo (confirmar se a luz verde nao interfere mesmo no fotoperiodo). 
*/

#include <WiFi.h>
#include "file.h"
#include "camera.h"
#include "lapse.h"

const char *ssid = "seu wifi";
const char *password = "sua senha do wifi";

void startCameraServer();

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.println();
	initFileSystem();
	initCamera();

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	startCameraServer();
	Serial.print("Camera Ready! Use 'http://");
	Serial.print(WiFi.localIP());
	Serial.println("' to connect");
}

void loop() {
	unsigned long t = millis();
	static unsigned long ot = 0;
	unsigned long dt = t - ot;
	ot = t;
	processLapse(dt);
}
