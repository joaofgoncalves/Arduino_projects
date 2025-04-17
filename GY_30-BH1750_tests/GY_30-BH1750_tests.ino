#include <Wire.h>             // Include Wire library for I2C communication
#include <Adafruit_Sensor.h>  // Include Adafruit Sensor library
#include <Adafruit_BME280.h>  // Include BME280 sensor library
#include <RTClib.h>           // Include RTClib for RTC module
#include <SD.h>               // Include SD library for MicroSD card
#include <BH1750.h>           // Include BH1750 sensor library
#include <LowPower.h>         // Add Low power management library

// Define pin connections for MicroSD card reader
#define SD_CS_PIN 10
#define LED_PIN 7
#define BUTTON_PIN 2


/*
float delayTimeMins = 1;

long convFactor = 60000;

long delayTimeMilis = delayTimeMins * convFactor;
*/

long delayTimeMilis = 10000;

bool remPrevLogFile = false;

long count = 0;

bool loggingEnabled = false;


void goSleep(unsigned long sleepDuration) {
  unsigned long sleepTime = sleepDuration / 1000;  // Convert milliseconds to seconds
  for (unsigned long i = 0; i < sleepTime; i += 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds at a time
  }
}

// Create instances for RTC, BME280, and BH1750
RTC_DS3231 rtc;            // Create instance for RTC
Adafruit_BME280 bme;       // Create instance for BME280 sensor
BH1750 lightSensor(0x23);  // Create instance for BH1750 sensor

void setup() {
  Serial.begin(9600);  // Start serial communication
  Wire.begin();        // Initialize I2C communication
  delay(500);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  } else {
    Serial.println("[OK] RTC module found");
    delay(500);
  }

  //  set the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {  // Change the address to match your sensor's address if needed
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  } else {
    Serial.println("[OK] BME280 temperature/humidity sensor found");
    delay(500);
  }

  // Initialize BH1750 sensor
  if (!lightSensor.begin()) {
    Serial.println("Error initializing BH1750 sensor");
    while (1)
      ;
  } else {
    Serial.println("[OK] BH1750 Light sensor found");
    delay(500);
  }

  // Initialize MicroSD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed!");
    while (1)
      ;
  } else {
    Serial.println("[OK] SD card reader/writer module found");
    delay(500);
  }

  // Check for the file and delete it if it exists
  if (SD.exists("DATAFILE.CSV") & remPrevLogFile) {
    Serial.println("DATAFILE.CSV exists. Deleting...");
    if (SD.remove("DATAFILE.CSV")) {
      Serial.println("File deleted successfully.");
    } else {
      Serial.println("Error deleting file.");
    }
  } else {
    Serial.println("DATAFILE.CSV does not exist OR not removing it option is active");
  }

  // Set up headers for data logging
  File dataFile = SD.open("DATAFILE.CSV", FILE_WRITE);
  if (dataFile) {
    dataFile.println("ObsID, DateTime, Temperature(C), Humidity(%RH), Pressure(hPa), Light(lux)");
    dataFile.close();
  } else {
    Serial.println("Error opening DATAFILE.CSV");
  }


  // Set LED in pos 7 to OUTPUT
  pinMode(LED_PIN, OUTPUT);
  // Set button with pull up resistor
  pinMode(BUTTON_PIN, INPUT);  // Set button pin as input with pull-up resistor

  if (digitalRead(BUTTON_PIN) == HIGH) {
    digitalWrite(BUTTON_PIN, LOW);
  }
}


void loop() {

  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {

    Serial.println("STOPPPED data logging!!");

      // Button pressed, toggle logging state
      loggingEnabled = !loggingEnabled;

    // Flash LED 3 times
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
    }

    // Delay for 500ms after LED flashing
    delay(500);
  }


  if (loggingEnabled) {

    // Turn on the LED
    digitalWrite(LED_PIN, HIGH);
    count++;

    // Read temperature, humidity, and pressure from BME280 sensor
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure() / 100.0F;  // Convert pressure from Pa to hPa

    // Read light intensity from BH1750 sensor
    float lux = lightSensor.readLightLevel();

    // Read date and time from RTC module
    DateTime now = rtc.now();

    // Print data to serial monitor
    Serial.print(now.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.print(", ");
    Serial.print(temperature);
    Serial.print(", ");
    Serial.print(humidity);
    Serial.print(", ");
    Serial.print(pressure);
    Serial.print(", ");
    Serial.println(lux);

    // Log data onto MicroSD card
    File dataFile = SD.open("DATAFILE.CSV", FILE_WRITE);
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
      dataFile.print(", ");
      dataFile.println(lux);
      dataFile.close();
    } else {
      Serial.println("Error opening DATAFILE.CSV");
    }

    // Turn off the LED
    digitalWrite(LED_PIN, LOW);

    delay(delayTimeMilis);  // Delay for 10 seconds before the next reading
  } else{
    Serial.println("DATA LOGGING IS OFF!");
  }
}
