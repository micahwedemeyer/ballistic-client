#include "definitions.h";
#include "neopixel.h";
#include "MusicPlayer.h";
#include "LightshowController.h";
#include "ImpactSensor.h";
#include "MQTTClient.h";
#include "MQTT.h";
#include "ArduinoJson.h";

ImpactSensor *impactSensor;
MusicPlayer *player;
LightshowController *lightshowController;
Adafruit_NeoPixel *strip;
MQTT *mqttConnection;
MQTTClient *mqttClient;
SerialLogHandler logHandler(LOG_LEVEL_TRACE);

void endHit();
void nullCallback() {}
void mqttCallback(char* topic, byte* payload, unsigned int length);
void hitDetected(int reading);

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  impactSensor = new ImpactSensor(IMPACT_PIN, LED_PIN, IMPACT_THRESHOLD, HIT_DELAY_MS, &hitDetected);

  player = new MusicPlayer();

  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);
  strip->begin();
  strip->setBrightness(NEOPIXEL_BRIGHTNESS);

  lightshowController = new LightshowController(strip);
  lightshowController->playShow("idle", true, &nullCallback);

  digitalWrite(LED_PIN, LOW);

  byte serverIP[] = {MQTT_BROKER_HOST_B1, MQTT_BROKER_HOST_B2, MQTT_BROKER_HOST_B3, MQTT_BROKER_HOST_B4};
  mqttConnection = new MQTT(serverIP, MQTT_BROKER_PORT, mqttCallback);
  mqttConnection->connect("darter-" + System.deviceID());
  mqttClient = new MQTTClient(mqttConnection, System.deviceID());
  mqttClient->subscribeToTopics();
  mqttClient->publishIntroduction();

  Log.info("Setup Complete");
  Particle.publish("Setup Complete");
}

void ticks() {
  impactSensor->tick();
  player->tick();
  lightshowController->tick();
  mqttClient->tick();
}

void loop() {
  ticks();
}

// Callback for the ImpactSensor
void hitDetected(int reading) {
  Particle.publish("Hit: " + String(reading));
  mqttClient->publishHit();
}

void endShow() {
  Particle.publish("Ending show. Back to idle.");
  lightshowController->playShow("idle", true, &nullCallback);
}

void playShow(String showId) {
  Log.info("Playing show: " + showId);
  Particle.publish("Playing show: " + showId);

  if(showId.equals("win")) {
    lightshowController->playShow(showId, false, &endShow);
  } else if(showId.equals("live")) {
    lightshowController->playShow(showId, true, &endShow);
  }
  player->playTune(showId);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(p);

  Log.trace("MQTT message received: " + String(topic));
  Particle.publish("MQTT message received: " + String(topic));

  String topicStr(topic);
  if(topicStr.endsWith("playShow")) {
    const char* showId = root["showId"];
    playShow(String(showId));
  } else if(topicStr.endsWith("requestIntroduction")) {
    mqttClient->publishIntroduction();
  }
}
