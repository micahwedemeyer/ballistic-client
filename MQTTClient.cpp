#include "Particle.h";
#include "MQTT.h";
#include "MQTTClient.h";
#include "ArduinoJson.h";

MQTTClient::MQTTClient(MQTT *conn, String devID) {
  connection = conn;
  deviceID = devID;
}

void MQTTClient::tick() {
  if(connection->isConnected()) {
    connection->loop();
  } else {
    Log.info("No MQTT connection. (Re)connecting.");
    reconnect();
  }
}

void MQTTClient::connect() {
  Log.info("Connecting to MQTT broker");

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  char deviceId[32];
  System.deviceID().toCharArray(deviceId, 32);
  root["deviceId"] = deviceId;

  char jsonString[255];
  root.printTo(jsonString, 255);

  // Connect to the MQTT broker and register a LWT to fire if connectivity is lost
  connection->connect("darter-" + System.deviceID(), darterTopic("lwt"), MQTT::QOS0, 0, jsonString);
  delay(50);

  if(connection->isConnected()) {
    Log.info("MQTT connection established. Subscribing to topics.");
    subscribeToTopics();
    publishIntroduction();
  } else {
    Log.warn("Failed to connect to MQTT broker.");
  }
}

void MQTTClient::reconnect() {
  connect();
}

bool MQTTClient::isConnected() {
  return connection->isConnected();
}

void MQTTClient::subscribeToTopics() {
  connection->subscribe(darterTopic("playShow"));
  connection->subscribe("darter/all/#");
}

void MQTTClient::publishIntroduction() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  char deviceId[32];
  System.deviceID().toCharArray(deviceId, 32);
  root["deviceId"] = deviceId;

  char jsonString[255];
  root.printTo(jsonString, 255);

  connection->publish(darterTopic("introduction"), jsonString);
}

void MQTTClient::publishHit() {
  if(connection->isConnected()) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    char deviceId[32];
    System.deviceID().toCharArray(deviceId, 32);
    root["deviceId"] = deviceId;
    root["timestamp"] = Time.now();

    char jsonString[255];
    root.printTo(jsonString, 255);

    connection->publish(darterTopic("hits"), jsonString);
  }
}

String MQTTClient::darterTopic(const char* lastElement) {
  return "darter/" + deviceID + "/" + lastElement;
}
