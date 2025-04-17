#include <Wire.h>
#include <Adafruit_VEML7700.h>
#include <DHT.h>
// #include <RTClib.h>
// #include <SD.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 7
#define DHTTYPE DHT11
#define SD_CS 10

Adafruit_VEML7700 veml;
DHT dht(DHTPIN, DHTTYPE);
// RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//File dataFile;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // lcd.setCursor(0, 0);
  // lcd.print("Temperature:");
  // lcd.setCursor(0, 1);
  // lcd.print("Humidity:   Lux:");
  // delay(3000);

  if (!veml.begin()) {
    Serial.println("Failed to communicate with VEML7700 sensor, check wiring?");
    while (1);
  }
  //Serial.println("VEML7700 sensor found!");

  dht.begin();

  // if (!rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   while (1);
  // }
  
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // if (!rtc.isrunning()) {
  //   Serial.println("RTC is not running, setting time...");
  // }

  // if (!SD.begin(SD_CS)) {
  //   Serial.println("Initialization failed!");
  //   while (1);
  // }
}

void loop() {
  //DateTime now = rtc.now();

  // Read the ambient light intensity in lux
  float lux = veml.readLux();
  delay(250);

  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // // Open the data file in write mode
  // dataFile = SD.open("datalog.csv", FILE_WRITE);

  // // If the file is available, write to it:
  // if (dataFile) {
  //   dataFile.print(now.year());
  //   dataFile.print("/");
  //   dataFile.print(now.month());
  //   dataFile.print("/");
  //   dataFile.print(now.day());
  //   dataFile.print(" ");
  //   dataFile.print(now.hour());
  //   dataFile.print(":");
  //   dataFile.print(now.minute());
  //   dataFile.print(":");
  //   dataFile.print(now.second());
  //   dataFile.print(",");
  //   dataFile.print(temperature, 1);
  //   dataFile.print(",");
  //   dataFile.print(humidity, 1);
  //   dataFile.print(",");
  //   dataFile.println(lux);
  //   dataFile.close();
  // } else {
  //   Serial.println("Error opening datalog.csv");
  // }

  // Print data to Serial Monitor
  // Serial.print(now.year());
  // Serial.print("/");
  // Serial.print(now.month());
  // Serial.print("/");
  // Serial.print(now.day());
  // Serial.print(" ");
  // Serial.print(now.hour());
  // Serial.print(":");
  // Serial.print(now.minute());
  // Serial.print(":");
  // Serial.print(now.second());
  // Serial.print(",");
  Serial.print(temperature, 1);
  Serial.print(",");
  Serial.print(humidity, 1);
  Serial.print(",");
  Serial.println(lux);

  // Display data on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 1);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(lux);


  delay(20000); // Log data every 5 seconds
}
