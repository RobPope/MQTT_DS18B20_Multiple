//Required for VCC
ADC_MODE(ADC_VCC);

//Import Libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
// WiFi parameters
const char* ssid = "YOUR_SSIS";
const char* password = "YOUR_PASSWORD";

//Create client
WiFiClient client;

//MQTT Configuration
PubSubClient mosq(client);
char msg[50];

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
int numSensors = 0;
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // start serial port
  Serial.begin(115200);

  // Connect to WiFi  
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  //Connect to Mosquitto Server
  mosq.setServer("YOUR_MQTT_SERVER",1883);

  // Start up the library
  sensors.begin();
  numSensors = sensors.getDeviceCount();
}
 
 
void loop(void)
{
  if(!mosq.connected()){
    mosq.connect("client1");
    getTemperature();
    getVoltage();
  }
  delay(60000);
}

void getTemperature() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  for (int i = 0; i < numSensors; i++) {
    const char* topicHome = "hab/livingroom/temperature/";
    String sInstance = String(i);
    const char* instance = sInstance.c_str();
    char topic[100];
    strcpy(topic, topicHome);
    strcat(topic, instance);
    
    float fTemp = sensors.getTempCByIndex(i);
    char cTemp[5];
    dtostrf(fTemp,5,3,cTemp);    
    //mosq.publish("hab/livingroom/temperature/",cTemp);
    mosq.publish(topic,cTemp);
    Serial.println(cTemp);
  }
}

void getVoltage() {
  int iVcc = ESP.getVcc();
  float fVcc = (float)ESP.getVcc() / 1000;
  char cVcc[5];
  dtostrf(fVcc,5, 3, cVcc);
  
  mosq.publish("hab/livingroom/voltage",cVcc);
  Serial.println(cVcc);
}
