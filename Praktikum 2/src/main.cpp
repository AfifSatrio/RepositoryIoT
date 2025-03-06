#include <Arduino.h>

int button1 = 23;
int button2 = 22;
int button3 = 21;
int redlamp = 25;
int yellowlamp = 26;
int greenlamp = 27;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(redlamp, OUTPUT);
  pinMode(yellowlamp, OUTPUT);
  pinMode(greenlamp, OUTPUT);
}

void loop() {
  if (digitalRead(button1) == LOW) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(redlamp, HIGH);
      delay(500);
      digitalWrite(redlamp, LOW);
      delay(500);
    }
  }

  if (digitalRead(button2) == LOW) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(redlamp, HIGH);
      digitalWrite(greenlamp, LOW);
      delay(500);
      digitalWrite(redlamp, LOW);
      digitalWrite(greenlamp, HIGH);
      delay(500);
    }
    digitalWrite(greenlamp, LOW); // Matikan lampu setelah berkedip
  }

  if (digitalRead(button3) == LOW) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(redlamp, HIGH);
      digitalWrite(yellowlamp, LOW);
      digitalWrite(greenlamp, LOW);
      delay(500);

      digitalWrite(redlamp, LOW);
      digitalWrite(yellowlamp, HIGH);
      digitalWrite(greenlamp, LOW);
      delay(500);

      digitalWrite(redlamp, LOW);
      digitalWrite(yellowlamp, LOW);
      digitalWrite(greenlamp, HIGH);
      delay(500);
    }
    digitalWrite(redlamp, LOW);
    digitalWrite(yellowlamp, LOW);
    digitalWrite(greenlamp, LOW); // Matikan semua lampu setelah berkedip
  }
}
