Digital-Electronics

Initially the example project did the following:

* read the sensor data, if connected.
* i2c master(ESP32) will write data to i2c slave(ESP32).
* i2c master(ESP32) will read data from i2c slave(ESP32).

For my project I will be reading this data, saving it to Flash on the ESP32 and then logging it in some way onto a PC using USB.
