
#include "DHT.h"
#include <SPI.h>
#include <SD.h>
#include <LowPower.h>

#define DHTPIN 7       // Set the pin connected to the DHT11 data pin
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

const int chipSelect = 4;

bool remPrevLogFile = true;

/*
float delayTimeMins = 1 / 6;

long convFactor = 60000;

long delayTimeMilis = delayTimeMins * convFactor;
*/

long delayTimeMilis = 600000;

long count = 0;

void goSleep(unsigned long sleepDuration) {
  unsigned long sleepTime = sleepDuration / 1000;  // Convert milliseconds to seconds
  for (unsigned long i = 0; i < sleepTime; i += 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds at a time
  }
}

void setup() {
  Serial.begin(9600);  //set the baud bit to 9600bps
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  dht.begin();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");


  // Check for the file and delete it if it exists
  if (SD.exists("DATALOG.TXT") & remPrevLogFile) {
    Serial.println("DATALOG.TXT exists. Deleting...");
    if (SD.remove("DATALOG.TXT")) {
      Serial.println("File deleted successfully.");
    } else {
      Serial.println("Error deleting file.");
    }
  } else {
    Serial.println("DATALOG.TXT does not exist.");
  }

  // Start log file
  File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

  // if the file is available, write the header row to it:
  if (dataFile) {
    String logFileHeader = "Log_Index,Temperature(C),Humidity(%),Heat_Index";
    Serial.println("Printing header row:");
    Serial.println(logFileHeader);
    dataFile.println(logFileHeader);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error opening DATALOG.TXT");
  }
}



void loop() {
  //Serial.println(delayTimeMilis);
  count++;

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  float hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();
  // Compute heat index in Celsius (isFahreheit = false)
  float hi = dht.computeHeatIndex(temp, hum, false);

  float temp_r = round(temp * 10) / 10.0;
  float hum_r = round(hum * 10) / 10.0;
  float hi_r = round(hi * 10) / 10.0;

  // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(temp)) {
    return;
  } else {
    Serial.println(" ");
    Serial.print("[");
    Serial.print(count);
    Serial.print("]\n");
    Serial.print("Temperature: ");
    Serial.print(temp_r);
    Serial.print(" C\nHumidity: ");
    Serial.print(hum_r);
    Serial.print(" %\nHeat Index: ");
    Serial.print(hi_r);
    Serial.println(" ");
  }

  String dataString = "";

  dataString = String(count) + "," + String(temp_r) + "," + String(hum_r) + "," + String(hi_r);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error opening DATALOG.TXT");
  }

  // Wait a few seconds between measurements.
  delay(delayTimeMilis);

  // Enter sleep mode between readings.
  //goSleep(delayTimeMilis);
}
