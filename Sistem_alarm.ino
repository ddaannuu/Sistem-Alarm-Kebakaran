#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTPIN D4
#define DHTTYPE DHT11
#define MQ2PIN A0

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN,DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Start");
  dht.begin();
  pinMode(MQ2PIN, INPUT);
}

void loop() {
  float temp=dht.readTemperature();
  float hum=dht.readHumidity();
  int gasV=analogRead(MQ2PIN);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Hum : ");
  lcd.print(hum);
  lcd.print("%");
  delay(1000);
   lcd.setCursor(0,0);
  lcd.print("Gas  : ");
  lcd.print(gasV);
  delay(1000);

}
