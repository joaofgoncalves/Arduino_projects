#include <Wire.h>             // Include Wire library for I2C communication
#include <Adafruit_Sensor.h>  // Include Adafruit Sensor library
#include <Adafruit_BME280.h>  // Include BME280 sensor library
//#include <BH1750.h>           // Include BH1750 sensor library
#include <Adafruit_VEML7700.h>  // Include VEML7700 sensor library
#include <RTClib.h>             // Include RTClib for RTC module
#include <SD.h>                 // Include SD library for MicroSD card
#include <LowPower.h>

// Define pin connections for MicroSD card reader
#define SD_CS_PIN 10

long delayTimeMilis = 10000;

//bool remPrevLogFile = true;

long count = 0;

//bool serialPrint = true;

void goSleep(unsigned long sleepDuration) {
  unsigned long sleepTime = sleepDuration / 1000;  // Convert milliseconds to seconds
  for (unsigned long i = 0; i < sleepTime; i += 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds at a time
  }
}

// Create instances for RTC and BME280
RTC_DS3231 rtc;       // Create instance for RTC
Adafruit_BME280 bme;  // Create instance for BME280 sensor
//BH1750 lightSensor(0x23);  // Create instance for BH1750 sensor
Adafruit_VEML7700 veml;  // Create instance for VEML7700 sensor

void setup() {
  Serial.begin(9600);  // Start serial communication
  Wire.begin();        // Initialize I2C communication
  delay(500);

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }
  /*
  else {
    Serial.println("[OK] BME280 found");
    delay(500);
  }
  */

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  /*
  else {
    Serial.println("[OK] RTC found");
    delay(500);
  }
  */

  //  set the RTC to the date & time this sketch was compiled
  //Serial.println("Setting up the RTC clock to the compile time");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  /*
  // Initialize BH1750 sensor
  if (!lightSensor.begin()) {
    Serial.println("Error initializing BH1750 sensor");
    while (1)
      ;
  } else {
    Serial.println("[OK] BH1750 Light sensor found");
    delay(500);
  }
  */

  // Initialize VEML7700 sensor
  if (!veml.begin()) {
    Serial.println("Failed to communicate with VEML7700 sensor, check wiring!");
    while (1)
      ;
  }

  /*else {
    Serial.println("[OK] VEML7700 found");
    delay(500);
  }
  */

  // == OPTIONAL =====
  // Can set non-default gain and integration time to
  // adjust for different lighting conditions.
  // =================
  // veml.setGain(VEML7700_GAIN_1_8);
  // veml.setIntegrationTime(VEML7700_IT_100MS);

  // Initialize MicroSD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed!");
    while (1)
      ;
  }

  /*else {
    Serial.println("[OK] SD card reader/writer found");
    delay(500);
  }
  */

  /*
  // Check for the file and delete it if it exists
  if (SD.exists("datalog.csv") & remPrevLogFile) {
    Serial.println("datalog.csv exists. Deleting...");
    if (SD.remove("datalog.csv")) {
      Serial.println("Log file deleted successfully.");
    } else {
      Serial.println("Error deleting file.");
    }
  } else {
    Serial.println("datalog.csv does not exist.");
  }
*/
  // Set up headers for data logging
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("ObsSeqID, DateTime, Temperature(C), Humidity(%RH), Pressure(hPa), Light(lux)");

    dataFile.close();
  } else {
    Serial.println("Error opening datalog.csv");
  }

  // Set LED to OUTPUT
  //pinMode(LED_PIN, OUTPUT);

  //pinMode(13, OUTPUT);
  //digitalWrite(13, LOW);
}

void loop() {

  //digitalWrite(LED_PIN, HIGH);
  count++;

  // Read temperature, humidity, and pressure from BME280 sensor
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;  // Convert pressure from Pa to hPa
  // Read light intensity from BH1750 sensor
  //float lux = lightSensor.readLightLevel();

  // Read light intensity from VEML7700 sensor
  float lux = veml.readLux();

  // Read date and time from RTC module
  DateTime now = rtc.now();

  // Print data to serial monitor

  Serial.print("[");
  Serial.print(count);
  Serial.print("] ");
  Serial.print(now.timestamp(DateTime::TIMESTAMP_FULL));
  Serial.print(", ");
  Serial.print(temperature);
  Serial.print(", ");
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print(pressure);
  //Serial.println(pressure);
  Serial.print(", ");
  Serial.println(lux);

  // Log data onto MicroSD card
  /*File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(count);
    dataFile.print(", ");
    dataFile.print(now.timestamp(DateTime::TIMESTAMP_FULL));
    dataFile.print(", ");
    dataFile.print(temperature);
    dataFile.print(", ");
    dataFile.print(humidity);
    dataFile.print(", ");
    dataFile.print(pressure);
    //dataFile.println(pressure);
    dataFile.print(", ");
    dataFile.println(lux);
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.csv");
  }
*/

  String dataString = "";

  dataString = String(count) + "," + 
              String(now.timestamp(DateTime::TIMESTAMP_FULL)) + "," + 
              String(temperature) + "," + 
              String(humidity) + "," +
              String(pressure) + "," + 
              String(lux);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error opening datalog.csv");
  }


  //delay(1000);
  // Turn off the LED
  //digitalWrite(LED_PIN, LOW);

  delay(delayTimeMilis);  // Delay for 10 seconds before the next reading

  //goSleep(delayTimeMilis);
}