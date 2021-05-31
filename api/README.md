# Arduino Co2 sensor

The code for an arduino based Co2 sensor station, using the [ESP32 DevKitC V4](https://github.com/espressif/arduino-esp32) and the MQ gaz sensor
The server is a rasberryPi

### Features

1. flask api for retriving data from one or multiple stations
2. Arduino code with support for Wifi (to communicate with api)

### Notes

Reading analog while using wifi resulted in the reading not working, turns out the ADC2 pin are unusable while on wifi, so I switch to an ADC1 pin