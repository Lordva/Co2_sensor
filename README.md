# Arduino Co2 sensor

This project is an arduino based Co2 and dangerous gas sensor, using the [ESP32 DevKitC V4](https://github.com/espressif/arduino-esp32) and the MQ gaz sensor
It connect to an MQTT broker witch you can host localy on a Rasberry pi using [Mosquitto](https://mosquitto.org/) and [OpenHab](https://www.openhab.org/)

it aims at being as simple as possible, just edit and rename default_config.h to config.h.
### Features :

1. Send data to an MQTT broker for home automation compatibility (OpenHab)
2. Arduino based code with wifi support, just flash it and your good to go
3. **TODO** Install Script for config, mosquitto and OpenHab
4. **TODO** Update ESP via wifi
### Notes

Reading analog while using wifi resulted in the reading not working, turns out the ADC2 pins are unusable while on wifi, so I switched to an ADC1 pin