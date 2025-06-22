// Blynk credentials (HARUS DI ATAS INCLUDE)
#define BLYNK_TEMPLATE_ID "TMPL6UKJvkBgO"
#define BLYNK_TEMPLATE_NAME "UASIOT"
#define BLYNK_AUTH_TOKEN "SSjhcJUeZ4ebqZ0SrV_809s-FU9dILME"

// Include library
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MQUnifiedsensor.h>

// WiFi credentials
char ssid[] = "Apip";
char pass[] = "";

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT11
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ-2
#define Board "ESP32"
#define Voltage_Resolution 3.3
#define ADC_Bit_Resolution 12
#define RatioMQ2CleanAir 9.83
#define MQ2_PIN 34

MQUnifiedsensor mq2(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ2_PIN, "MQ-2");

// Timer Blynk
BlynkTimer timer;

// Variabel sensor global
float lpg_ppm, co_ppm, smoke_ppm, h2_ppm, temperature, humidity;

// Threshold limit
#define LPG_LIMIT 1000
#define CO_LIMIT 50
#define SMOKE_LIMIT 300
#define H2_LIMIT 1000

bool kondisiBahaya = false;

void readSensors() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca DHT11!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    return;
  }

  mq2.update();

  mq2.setRegressionMethod(1);

  mq2.setA(574.25); mq2.setB(-2.222);
  lpg_ppm = mq2.readSensor();

  mq2.setA(36974); mq2.setB(-3.109);
  co_ppm = mq2.readSensor();

  mq2.setA(1000); mq2.setB(-2.186);
  smoke_ppm = mq2.readSensor();

  mq2.setA(987); mq2.setB(-2.162);
  h2_ppm = mq2.readSensor();

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, lpg_ppm);
  Blynk.virtualWrite(V3, co_ppm);
  Blynk.virtualWrite(V4, smoke_ppm);
  Blynk.virtualWrite(V5, h2_ppm);

  Serial.print("Suhu: "); Serial.print(temperature); Serial.print(" C, ");
  Serial.print("LPG: "); Serial.print(lpg_ppm); Serial.print(" ppm, ");
  Serial.print("CO: "); Serial.print(co_ppm); Serial.print(" ppm, ");
  Serial.print("Smoke: "); Serial.print(smoke_ppm); Serial.print(" ppm, ");
  Serial.print("H2: "); Serial.print(h2_ppm); Serial.println(" ppm");

  // Cek kondisi bahaya
  kondisiBahaya = (lpg_ppm > LPG_LIMIT) || (co_ppm > CO_LIMIT) || (smoke_ppm > SMOKE_LIMIT) || (h2_ppm > H2_LIMIT);

  // Update LED di Blynk
  if (kondisiBahaya) {
    Blynk.setProperty(V10, "color", "#FF0000"); // Merah
    Blynk.virtualWrite(V10, 255); // ON
  } else {
    Blynk.setProperty(V10, "color", "#00FF00"); // Hijau
    Blynk.virtualWrite(V10, 255); // ON
  }
}

void tampilkanKeLCD() {
  if (kondisiBahaya) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GAS BAHAYA!!");
    lcd.setCursor(0, 1);
    lcd.print("Segera Cek!");
    delay(3000);
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu:");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Lembab:");
  lcd.print(humidity, 1);
  lcd.print("%");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LPG:");
  lcd.print(lpg_ppm, 1);
  lcd.print("ppm");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO:");
  lcd.print(co_ppm, 1);
  lcd.print("ppm");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smoke:");
  lcd.print(smoke_ppm, 1);
  lcd.print("ppm");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("H2:");
  lcd.print(h2_ppm, 1);
  lcd.print("ppm");
  delay(2000);
}

void sendSensor() {
  readSensors();
  tampilkanKeLCD();
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  mq2.setRegressionMethod(1);
  mq2.init();

  Serial.print("Calibrating MQ-2 ...");
  float calcR0 = 0;
  for (int i = 0; i < 10; i++) {
    mq2.update();
    calcR0 += mq2.calibrate(RatioMQ2CleanAir);
    delay(500);
  }
  mq2.setR0(calcR0 / 10);
  Serial.print("Done! R0 = "); Serial.println(mq2.getR0());

  if (isnan(mq2.getR0())) {
    Serial.println("Kalibrasi gagal, cek koneksi MQ-2!");
    while (1);
  }

  timer.setInterval(5000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
