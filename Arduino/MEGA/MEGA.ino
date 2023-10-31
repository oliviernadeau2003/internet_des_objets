/*
  Communications bi-directionnelles entre le ARDUINO MEGA et le ARDUINO UNO

  Olivier Nadeau 
  05/09/2023
  03/10/2023

  Port Série Sérial pour communiquer avec le UNO Pin 19(RX) et 18(TX)
*/
#include "SHT31.h"

//#define DEBUG


SHT31 sht31 = SHT31();

char charMaison = 0;
char charConsole = 0;
char charHM10 = 0;


void setup() {


  Serial.begin(9600);   // Console
  Serial1.begin(9600);  // Maison
  Serial2.begin(9600);  // HM-10
  sht31.begin();
}

void loop() {

  if (Serial1.available()) {
    charMaison = Serial1.read();
    Serial.print("de la maison: ");
    Serial.println(charMaison);
  }
  if (Serial.available()) {
    charConsole = Serial.read();
    Serial.print("de la console: ");
    Serial.println(charConsole);
    Serial1.print(charConsole);
    Serial2.print(charConsole);
  }
  if (Serial2.available()) {
    charHM10 = Serial2.read();
    Serial.print("de Bluetooth: ");
    Serial.println(charHM10);
    Serial1.print(charHM10);
  }
  if (millis() % 1000 == 0) {  //à chaque seconde
    float temp = sht31.getTemperature();
    float hum = sht31.getHumidity();
    int temperature = (temp * 100);
    int humid = (hum * 100);
#ifdef DEBUG
    Serial.print("Temp = ");
    Serial.print(temp);
    Serial.print(" C   ");  //The unit for  Celsius because original arduino don't support special symbols
    Serial.print("Hum = ");
    Serial.print(hum);
    Serial.println("%");
//Serial.print(temperature);
//Serial.print(" , ");
//Serial.println(humid);
#endif
    // L'une ou l'autre des méthodes qui suivent fonctionnent
    Serial2.print('x' + (String)temperature + ',' + (String)humid + '\n');  //on peut concaténer des strings
    // ou
    // Serial2.print('x');
    // Serial2.print(temperature);
    // Serial2.print(',');
    // Serial2.print(humid);
    // Serial2.print('\n');
    // Serial2.flush();  //Juste pour être certain d'avoir enoyé tous les charactères dans la deuxième méthode1,1#
    delay(1);
  }
}