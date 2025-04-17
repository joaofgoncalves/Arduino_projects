/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header
 		Pin 4 used here for consistency with other Arduino examples
 
 created  24 Nov 2010
 modified 9 Apr 2012 by Tom Igoe
 
 This example code is in the public domain.
 
 ** Demo para multiplos DHT (JNeiva)
- Arduno Nano
- uSD + ds1307
- DHT22 	 
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Time.h"
#include "DS1307RTC.h"
#include <DHT.h>

#define DHTPIN1 3     // what pin we're connected to
#define DHTTYPE1 DHT22   // DHT 22 
DHT dht(DHTPIN1, DHTTYPE1);

// Led Status
#define Status 5

// SD library chipSelect
const int chipSelect = 10;

File dataFile;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  pinMode(Status, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.csv");
    // Wait forever since we cant write data
    while (1) ;
  }
}

void loop()
{
  tmElements_t tm;
  // make a string for assembling the data to log:
  String dataString = "";

  digitalWrite(Status, HIGH); // Led ON

  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  RTC.read(tm);
  
  dataString += String(tm.Day);
  dataString += "/";
  dataString += String(tm.Month);
  dataString += "/";
  dataString += String(tmYearToCalendar(tm.Year));
  dataString += " ; ";
  dataString += String(tm.Hour);
  dataString += ":";
  dataString += String(tm.Minute);
  dataString += ":";
  dataString += String(tm.Second);
  dataString += " ; ";

  dataString += String(h);
  dataString += " ; ";  
  dataString += String(t);
  dataString += " ; ";
  
  dataFile.println(dataString);

  // print to the serial port too:
  Serial.println(dataString);
  
  // The following line will 'save' the file to the SD card after every
  // line of data - this will use more power and slow down how much data
  // you can read but it's safer! 
  // If you want to speed up the system, remove the call to flush() and it
  // will save the file only every 512 bytes - every time a sector on the 
  // SD card is filled with data.
  dataFile.flush();
  
  // Take 1 measurement every 1s -> 1000 milliseconds
  // Led OFF
  delay(1000);
  digitalWrite(Status, LOW); // Led OFF
  
  // Wait to next Sample
  delay(59000);
}









