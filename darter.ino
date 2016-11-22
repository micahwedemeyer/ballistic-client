#include "definitions.h";
#include "neopixel.h";
#include "MusicPlayer.h";
#include "LightshowController.h";
#include "ImpactSensor.h";
#include "MQTTClient.h";
#include "MQTT.h";
#include "ArduinoJson.h";

bool showPlaying;
ImpactSensor *impactSensor;
MusicPlayer *player;
LightshowController *lightshowController;
Adafruit_NeoPixel *strip;
MQTT *mqttConnection;
MQTTClient *mqttClient;

void endHit();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void hitDetected();

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  impactSensor = new ImpactSensor(IMPACT_PIN, LED_PIN, IMPACT_THRESHOLD, HIT_DELAY_MS, &hitDetected);

  showPlaying = false;

  player = new MusicPlayer();

  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);
  strip->begin();
  strip->setBrightness(NEOPIXEL_BRIGHTNESS);

  lightshowController = new LightshowController(strip);
  lightshowController->playIdleShow();

  digitalWrite(LED_PIN, LOW);

  byte serverIP[] = {MQTT_BROKER_HOST_B1, MQTT_BROKER_HOST_B2, MQTT_BROKER_HOST_B3, MQTT_BROKER_HOST_B4};
  mqttConnection = new MQTT(serverIP, MQTT_BROKER_PORT, mqttCallback);
  mqttConnection->connect("darter-" + System.deviceID());
  mqttClient = new MQTTClient(mqttConnection, System.deviceID());
  mqttClient->subscribeToTopics();
  mqttClient->publishIntroduction();

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
void hitDetected() {
  mqttClient->publishHit();
}

void endShow() {
  showPlaying = false;
  lightshowController->playIdleShow();
}

void playShow(String showId) {
  if(showPlaying) {
    return;
  }
  showPlaying = true;
  lightshowController->playHitShow(&endShow);
  player->playTune();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(p);

  Particle.publish("MQTT message received: " + String(topic));

  String topicStr(topic);
  if(topicStr.endsWith("playShow")) {
    playShow("win");
  }
}
