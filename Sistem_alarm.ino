#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
// #define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// char auth[] = "";//Enter your Auth token
// char ssid[] = "";//Enter your WIFI name
// char pass[] = "";//Enter your WIFI password

#define DHTPIN D4
#define DHTTYPE DHT11
#define MQ2PIN A0
#define buzz D5

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN,DHTTYPE);
int suhuTerakhir = 0.0;
int batasGas=300;
int suhuDarurat=31;//40
bool buzzeract=false;

void setup() {
  
  // Blynk.begin(auth, ssid, pass);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Start");
  delay(2000);
  dht.begin();
  pinMode(MQ2PIN, INPUT);
  pinMode(buzz, OUTPUT);
  lcd.clear();

}

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzz, HIGH);
    delay(200); // Durasi bunyi
    digitalWrite(buzz, LOW);
    delay(200); // Interval antar bunyi
  }
}



void loop() {

  float temp=dht.readTemperature();
  float hum=dht.readHumidity();
  int gasV=analogRead(MQ2PIN);

  // Tampilan DHT11
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("S:");
  lcd.print(temp);
  lcd.print("C");
  // Tampilan MQ2
  lcd.setCursor(9,0);
  lcd.print("G:");
  lcd.print(gasV);

  lcd.setCursor(0,1);
  lcd.print("K:");
  lcd.print(hum);
  lcd.print("%");
  delay(1000);


  if (temp >= suhuTerakhir + 1) 
  {
    beep(1);
    suhuTerakhir = temp;
    buzzeract=true;
  }
  else if (temp < suhuTerakhir)
  {
    beep(2);
  }
  
  else if (batasGas<=gasV && temp>=suhuDarurat)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GAS TERDETEKSI!!!");
    lcd.setCursor(0,1);
    lcd.print("Suhu Bahaya!!!");
    beep(10);
  }

  else if (batasGas<=gasV)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gas Terdeteksi!!!");
    beep(10);
  }

  else if (temp>=suhuDarurat)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Suhu Bahaya!!!");
    beep(10);
  }


  if (temp >= suhuTerakhir + 2 || temp < suhuTerakhir) 
  {
    suhuTerakhir = temp;
    buzzeract = false;
  }

}


