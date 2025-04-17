#include <Wire.h>               // Include Wire library for I2C communication
#include <Adafruit_Sensor.h>    // Include Adafruit Sensor library
#include <Adafruit_BME280.h>    // Include BME280 sensor library
#include <Adafruit_VEML7700.h>  // Include VEML7700 sensor library
#include <RTClib.h>             // Include RTClib for RTC module
#include <SD.h>                 // Include SD library for MicroSD card
#include <LowPower.h>

// Define pin connections for MicroSD card reader
#define SD_CS_PIN 10

long delayTimeMilis = 600000; // 10 minutes test

void goSleep(unsigned long sleepDuration) {
  unsigned long sleepTime = sleepDuration / 1000;  // Convert milliseconds to seconds
  for (unsigned long i = 0; i < sleepTime; i += 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds at a time
  }
}

// Create instances for RTC and BME280
RTC_DS3231 rtc;          // Create instance for RTC
Adafruit_BME280 bme;     // Create instance for BME280 sensor
Adafruit_VEML7700 veml;  // Create instance for VEML7700 sensor

void setup() {

  Serial.begin(9600);  // Start serial communication
  Wire.begin();        // Initialize I2C communication
  delay(500);

  // Initialize MicroSD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed!");
    while (1)
      ;
  }

  // Set up headers for data logging
  /*File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("DateTime, Temperature(C), Humidity(%RH), Pressure(hPa), Light(lux)");

    dataFile.close();
  } else {
    Serial.println("Error opening datalog.csv! Header data not written.");
  }
  */

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  //  set the RTC to the date & time this sketch was compiled
  //Serial.println("Setting up the RTC clock to the compile time");

  if(rtc.lostPower()){
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  // Initialize VEML7700 sensor
  if (!veml.begin()) {
    Serial.println("Failed to communicate with VEML7700 sensor, check wiring!");
    while (1)
      ;
  }
}

void loop() {

  // Read temperature, humidity, and pressure from BME280 sensor
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;  // Convert pressure from Pa to hPa

  delay(100);
  // Read light intensity from VEML7700 sensor
  float lux = veml.readLux();

  delay(100);
  // Read date and time from RTC module
  DateTime now = rtc.now();

  // Print data to serial monitor
  delay(100);
/*
  Serial.print(now.timestamp(DateTime::TIMESTAMP_FULL));
  Serial.print(", ");
  Serial.print(temperature);
  Serial.print(", ");
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print(pressure);
  Serial.print(", ");
  Serial.println(lux);
  
  delay(100);
*/
  // Manually format the timestamp
  char timestamp[20];
  sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", 
          now.year(), now.month(), now.day(), 
          now.hour(), now.minute(), now.second());

  // Log data onto MicroSD card
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(", ");
    dataFile.print(temperature);
    dataFile.print(", ");
    dataFile.print(humidity);
    dataFile.print(", ");
    dataFile.print(pressure);
    dataFile.print(", ");
    dataFile.println(lux);
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.csv");
  }

  //delay(delayTimeMilis);  // Delay for 10 seconds before the next reading

  goSleep(delayTimeMilis);
}
