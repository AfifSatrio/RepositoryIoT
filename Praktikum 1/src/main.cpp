#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
  digitalWrite(23, HIGH);
  delay(3000);
  digitalWrite(23, LOW);
  delay(3000);
  digitalWrite(22, HIGH);
  delay(500);
  digitalWrite(22, LOW);
  delay(500);
  digitalWrite(21, HIGH);
  delay(2000);
  digitalWrite(21, LOW);
  delay(2000);
}
