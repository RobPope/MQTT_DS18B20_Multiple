# MQTT_DS18B20_Multiple
Arduino sketch for ESP8266 to log temperature from multiple DS18B20 probes connected to the same pin

You'll need to have the following libraries installed:
ESP8266WiFi
PubSubClient
OneWire
DallasTemperature

Every minute the ESP will log the temperature for each probe connected to the specified pin, plus internal voltage, to topics on your MQTT Server

**ToDo**
Deep Sleep
WiFi Manager
