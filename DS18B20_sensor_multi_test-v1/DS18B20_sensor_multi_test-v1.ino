#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 7

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC;

void setup(void) {

  // Start up the library
  sensors.begin();

  // Set the resolution to 12 bit (maximum precision)
  sensors.setResolution(12);

  Serial.begin(9600);

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
}

void loop(void) {
  // Send command to all the sensors for temperature conversion
  sensors.requestTemperatures();

  // Display temperature from each sensor
  for (int i = 0; i < deviceCount; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(" : ");
    tempC = sensors.getTempCByIndex(i);
    Serial.print(tempC);
    //Serial.print((char)223); // Alternative for the degree sign
    Serial.print("°C  |  ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
    //Serial.print((char)223); // Alternative for the degree sign
    Serial.println("°F");
  }

  Serial.println("");
  delay(10000);
}