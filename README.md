# Air quality sensor


# How to join
## Reset to FN rebooting device 5 times with interval less than 10 seconds, led will start flashing during reset
## Reset to FN by pressing and holding button(1) 5 seconds
![Device](./images/CO2_SenseAir_S8.png)


# How to add device into zigbe2mqtt
Use [external converters](https://www.zigbee2mqtt.io/information/configuration.html#external-converters-configuration) feature

Converter file located [here](./converters/DIYRuZ_AirSense.js)

# User interface

LED1 blinks when accessing the CO2 sensor.

LED2 blinks if the CO2 value is higher than the first set point, while LED3 is off.

LED3 blinks if the CO2 value is higher than the second set point, while LED2 is on.

SW2 reset and join E18

SW1 CO2 calibration
