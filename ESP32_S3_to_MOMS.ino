#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ArduinoJson.h>

JsonDocument doc;

// Replace with your network credentials (STATION)
const char* ssid = "MyRouter4";
const char* password = "mimosian";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
const char* server = "192.168.68.10";    // MQTT server (of your choice)

/********** For any JSON packet creation ************/
char msg[120];
/************* end for JSON packet ******************/


void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  mqttClient.setServer(server, 1883);
  //mqttClient.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.println();
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (mqttClient.connect("","94BEGwkBCTGq0i7LwchM","")) {
      Serial.println(F("connected"));
      // Once connected, publish an announcement...
      mqttClient.publish("", "hello world");
      // ... and resubscribe
      //mqttClient.subscribe("LoadCell_01_Rx");
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(mqttClient.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  long randNumberTemp;
  long randNumberHumid;

  if (!mqttClient.connected()) {
    reconnect();
  }
  delay(500);
  mqttClient.loop();

  randNumberTemp = random(20, 80); 
  randNumberHumid = random(40, 100);

  /******* reading ***************/
  doc["Temperature"] = randNumberTemp;
  doc["Humidity"] = randNumberHumid;
  /**************************************/
  serializeJson(doc, msg);

  Serial.println();
  Serial.println(F("Publish message : "));
  Serial.println(msg);
  mqttClient.publish("v1/devices/me/telemetry", msg);

  delay(5000);

}
