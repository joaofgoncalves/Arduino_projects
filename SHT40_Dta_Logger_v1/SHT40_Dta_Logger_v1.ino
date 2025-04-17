#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_VEML7700.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <LowPower.h>

// Initialize the sensors, RTC, and SD card
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_VEML7700 veml = Adafruit_VEML7700();
RTC_DS3231 rtc;
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100); // Wait for Serial monitor to open

  // Initialize SHT4x sensor
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x sensor!");
    while (1) delay(10);
  }

  // Initialize VEML7700 sensor
  if (!veml.begin()) {
    Serial.println("Couldn't find VEML7700 sensor!");
    while (1) delay(10);
  }
  
  veml.setGain(VEML7700_GAIN_1); // Set gain (1x is the default)
  veml.setIntegrationTime(VEML7700_IT_100MS); // Set integration time (100ms is the default)

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1) delay(10);
  }

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1) delay(10);
  }

  // Create the file and write the header if it doesn't exist
  /*if (!SD.exists("datalog.csv")) {
    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println("DateTime,Temperature,Humidity,Lux");
      dataFile.close();
    }
  }*/
}

void loop() {
  DateTime now = rtc.now();

  // Get sensor readings
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);
  float lux = veml.readLux();

  // Create a string for data logging
  String dataString = "";
  dataString += String(now.year()) + "-";
  dataString += String(now.month()) + "-";
  dataString += String(now.day()) + "T";
  dataString += String(now.hour()) + ":";
  dataString += String(now.minute()) + ":";
  dataString += String(now.second()) + ",";
  dataString += String(temp.temperature, 2) + ",";
  dataString += String(humidity.relative_humidity, 2) + ",";
  dataString += String(lux, 2);

  // Open the file
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // Print to Serial Monitor for debugging
    //Serial.println(dataString);
  } else {
    // If the file isn't open, print an error
    Serial.println("Error opening datalog.csv");
  }

  //delay(5000);
  goSleep(900000);
}

void goSleep(unsigned long sleepDuration) {
  unsigned long sleepTime = sleepDuration / 1000;  // Convert milliseconds to seconds
  for (unsigned long i = 0; i < sleepTime; i += 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds at a time
  }
}
