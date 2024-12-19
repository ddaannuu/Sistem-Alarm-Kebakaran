// Library

#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


// Port kabel
#define PIR D8
#define DHTPIN D4
#define DHTTYPE DHT11
#define MQ2PIN A0
#define buzz D5
#define rLed D6
#define bLed D7

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

// Variabel sensor
int sensor=0;
float temp, gasV, hum;

int suhuTerakhir = 0.0;
int batasGas = 300;
int suhuDarurat = 40;
bool buzzeract = false;
int entitas = 0;

const char* ssid = "danu";
const char* password = "danudanu";

const char* botToken = "7623678633:AAElXiW88Qxx4ZAjC7cKsWUghqnPR4G3qwU";
const char*  chatID= "5344849595";
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

unsigned long lastTelegramTime = 0;         // Waktu terakhir pengiriman pesan
const unsigned long telegramInterval = 1000; // Interval pengiriman pesan (30 detik)

// Flag untuk memastikan pesan hanya terkirim sekali
bool gasAlertSent = false;
bool suhuAlertSent = false;
bool motionAlertSent = false;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");

  client.setInsecure(); // Nonaktifkan verifikasi SSL

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting....");
  lcd.setCursor(0, 1);
  lcd.print("Kelompok 1");
  delay(2000);
  dht.begin();
  pinMode(PIR, INPUT);
  pinMode(MQ2PIN, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(rLed, OUTPUT);
  pinMode(bLed, OUTPUT);

  lcd.clear();
}

void sendTelegramAlert(String message) {
  if (millis() - lastTelegramTime > telegramInterval) {
    bot.sendMessage("5344849595", message, "Markdown");
    lastTelegramTime = millis();
    Serial.println("Telegram Alert Sent: " + message);
  }
}

void bareng(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzz, HIGH);
    digitalWrite(rLed, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    digitalWrite(rLed, LOW);
    delay(100);
  }
}

void bareng2(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzz, HIGH);
    digitalWrite(bLed, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    digitalWrite(bLed, LOW);
    delay(100);
  }
}

void sensorData(){
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  gasV = analogRead(MQ2PIN);
  sensor = digitalRead(PIR);
}

void displayDataOnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: " + String(temp) + "C");
  lcd.setCursor(0, 1);
  lcd.print("Gas : "+ String(gasV) + "%");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lembab : " + String(hum) + "%");
  lcd.setCursor(0, 1);
  lcd.print("Entitas: "+ String(entitas));
  delay(1000);
}

void checking(){
  if (gasV >= batasGas && !gasAlertSent) {
    sendTelegramAlert("⚠️ Peringatan: Gas berbahaya terdeteksi!");
    gasAlertSent = true;
    bareng(60);
  } else if (gasV < batasGas) {
    gasAlertSent = false; // Reset flag jika kondisi aman
  }

  // Deteksi suhu berbahaya
  if (temp >= suhuDarurat && !suhuAlertSent) {
    sendTelegramAlert("⚠️ Peringatan: Suhu terlalu tinggi! Suhu: " + String(temp) + "°C");
    suhuAlertSent = true;
    bareng(60);
  } else if (temp < suhuDarurat) {
    suhuAlertSent = false; // Reset flag jika suhu kembali normal
  }

  // Deteksi gerakan
  if (sensor == HIGH && !motionAlertSent) {
    sendTelegramAlert("⚠️ Peringatan: Gerakan terdeteksi!");
    entitas++;
    motionAlertSent = true;
    bareng2(3);
  } else if (sensor == LOW) {
    motionAlertSent = false;
  }

    if (temp >= suhuTerakhir + 2 || temp < suhuTerakhir) 
  {
    suhuTerakhir = temp;
    buzzeract = false;
  }

  if (temp >= suhuTerakhir + 1) 
  {
    bareng(1);
    suhuTerakhir = temp;
    buzzeract=true;
  }
  else if (temp < suhuTerakhir)
  {
    bareng(2);
  }

}

void loop() {
  
  sensorData();
  displayDataOnLCD();
  checking();
  delay(1000); // Delay 1 detik untuk loop

}
