# Arduino Co2 sensor

The code for an arduino based Co2 sensor station, using the [ESP32 DevKitC V4](https://github.com/espressif/arduino-esp32) and the MQ gaz sensor
It connect to an MQTT broker witch you can host localy on a Rasberry pi using [Mosquitto](https://mosquitto.org/) and [OpenHab](https://www.openhab.org/)

### Features

1. Send data to an MQTT broker for home automation compatibility (OpenHab)
2. Arduino code with support for Wifi (to communicate with api)

### Notes

Reading analog while using wifi resulted in the reading not working, turns out the ADC2 pins are unusable while on wifi, so I switched to an ADC1 pin