# Arduino Projects

This repository contains a collection of Arduino sketches and experiments demonstrating sensor integration, data logging, and low-power techniques. You'll find projects interfacing with environmental sensors (BME280, DHT11, DS18B20, SHT4x, VEML7700, BH1750), real-time clocks, microSD logging, and custom circuitry such as a rock conductivity meter.


## Projects

### BME280_Logger_tests
A data logger sketch that reads temperature, humidity, and pressure from an Adafruit BME280 sensor paired with a DS3231 RTC, writes timestamped CSV entries to a microSD card every 10 seconds, and demonstrates file initialization and header setup.

### BmC_THPL_Logger_v1
The first version of a THPL (Temperature, Humidity, Pressure, Light) logger combining BME280, VEML7700, DS3231 RTC, and microSD logging. It captures environmental readings with sequence numbering and prints them over serial.

### BmC_THPL_Logger_v1.1
An enhanced THPL logger that formats timestamps manually and enters deep sleep for 10 minutes between readings. It logs temperature, humidity, pressure, and light data to microSD in CSV format.

### DataloggerNano
A microSD datalogger sketch for Arduino Nano that reads humidity and temperature from a DHT22 sensor, timestamps each entry using a DS1307 RTC, and writes CSV entries (datalog.csv) to the SD card every minute, with an LED status indicator.

### DHT11_VEML7700-tests-v1
A demonstration of DHT11 temperature/humidity and VEML7700 light sensors driving an I2C LCD and serial output, showcasing real-time sensor reading and display.

### DHT11_sensor_tests
A DHT11 sensor test and CSV logger that measures temperature, humidity, calculates heat index, logs data with index numbers to a microSD card, and optionally uses low-power sleep modes.

### DS18B20_sensor_multi_test-v1
Experiments with multiple DS18B20 OneWire temperature sensors, reading each sensor’s Celsius and Fahrenheit temperature and printing values to the serial console at regular intervals.

### GY_30-BH1750_tests
A versatile data logger combining BME280 for temperature, humidity, pressure, BH1750 for light, DS3231 RTC, microSD logging, and user control via a push button and LED indicator, illustrating interactive logging and power management.

### GY_30_BH1750_tests_v2
A streamlined version of the BH1750/BME280 logger that forgoes button input and deep sleep, continuously capturing and writing sensor data to a CSV file with LED status signaling.

### LowPowerAlways-test
A minimal sketch proving periodic deep sleep using the LowPower library, repeatedly powering down the MCU for 8-second intervals to demonstrate energy-saving techniques.

### RockConductivityMeter-v1
A custom rock conductivity meter that applies voltage across electrode pairs, measures analog readings through known resistor values, computes resistance and conductivity (mS/cm), and outputs results via serial port.

### SHT40_Dta_Logger_v1
An environmental data logger using an Adafruit SHT4x temperature/humidity sensor, VEML7700 light sensor, DS3231 RTC, and microSD card to record timestamped sensor readings with low-power intervals between logs.
