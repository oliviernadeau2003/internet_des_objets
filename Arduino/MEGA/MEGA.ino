/*
  Communications bi-directionnelles entre le ARDUINO MEGA et le ARDUINO UNO

  Olivier Nadeau 
  05/09/2023

  Port Série Sérial pour communiquer avec le UNO Pin 19(RX) et 18(TX)
*/

#include "SHT31.h"

SHT31 sht31 = SHT31();

#define DEBUG

char console;
char maison;
char bluetooth;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  Serial1.begin(9600);
  Serial2.begin(9600);
  sht31.begin();
}

void loop() {

  if (Serial.available() > 0) {
    console = Serial.read();
    Serial.print(" de la console ");
    Serial.print(console);
    Serial1.write(console);
  }

  if (Serial1.available() > 0) {
    maison = Serial1.read();
    Serial.print(" de la maison ");
    Serial2.write(maison);
  }

  if (Serial2.available() > 0) {
    bluetooth = Serial2.read();
    Serial.print(" du bluetooth ");
    Serial.print(bluetooth);
    Serial1.write(bluetooth);
  }

  if (millis() % 1000 == 0) {
    float temp = sht31.getTemperature();
    float humidity = sht31.getHumidity();
    Serial.print("Temperature : ");
    Serial.println(temp);
    Serial.print("Humidity :    ");
    Serial.println(humidity);
    if (temp >= 23.0) {
      Serial.println("Température Élevée !");
      Serial1.write("r");
    } else{
      Serial1.write("s");
    }
  }
}
