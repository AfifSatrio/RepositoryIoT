#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 33
#define DHTTYPE DHT22
#define LDRPIN 32  // Pindahkan ke pin ADC yang benar
#define SDA_PIN 21 // Ganti SDA ke GPIO 21 (default ESP32)
#define SCL_PIN 22 // Ganti SCL ke GPIO 22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(115200);
    
    // Inisialisasi I2C dengan pin yang benar
    Wire.begin(SDA_PIN, SCL_PIN);

    // Inisialisasi LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Mengukur...");

    // Inisialisasi sensor DHT
    dht.begin();
    
    // Set LDR sebagai input
    pinMode(LDRPIN, INPUT);
}

void loop() {
    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();
    int nilaiLDR = analogRead(LDRPIN);

    // Validasi pembacaan sensor DHT
    if (isnan(suhu) || isnan(kelembaban)) {
        Serial.println("Gagal membaca sensor DHT!");
        lcd.clear();
        delay(10);
        lcd.setCursor(0, 0);
        lcd.print("DHT Error!");
        delay(2000);
        return;
    }

    float intensitasCahaya = map(nilaiLDR, 0, 4095, 0, 100);
    intensitasCahaya = constrain(intensitasCahaya, 0, 100);

    Serial.print("Suhu: "); Serial.print(suhu); Serial.println(" C");
    Serial.print("Kelembaban: "); Serial.print(kelembaban); Serial.println(" %");
    Serial.print("Cahaya: "); Serial.print(intensitasCahaya); Serial.println(" %");
    Serial.println("----------------------");

    lcd.clear();
    delay(10);  // Tambahkan delay setelah clear
    lcd.setCursor(0, 0);
    lcd.print("Suhu: "); lcd.print(suhu, 1); lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: "); lcd.print(kelembaban, 1); lcd.print("% C:");
    lcd.print(intensitasCahaya);

    delay(2000);
}
