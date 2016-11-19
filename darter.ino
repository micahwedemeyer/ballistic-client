#include "definitions.h";
#include "neopixel.h";
#include "tone_test.h";
#include "pixel_test.h";
#include "MQTTClient.h";
#include "MQTT.h";
#include "ArduinoJson.h";

int impactSensorReading;
volatile bool hitDetected;
bool processingHit;
MusicPlayer *player;
LightshowController *lightshowController;
Adafruit_NeoPixel *strip;
Timer *hitDelayer;
MQTT *mqttConnection;
MQTTClient *mqttClient;

void endHit();
void mqttCallback(char* topic, byte* payload, unsigned int length);

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  impactSensorReading = 0;
  processingHit = false;
  hitDetected = false;

  player = new MusicPlayer();

  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);
  strip->begin();
  strip->setBrightness(NEOPIXEL_BRIGHTNESS);

  lightshowController = new LightshowController(strip);
  lightshowController->playIdleShow();

  digitalWrite(LED_PIN, LOW);

  hitDelayer = new Timer(HIT_DELAY_MS, endHit, true);


  byte serverIP[] = {MQTT_BROKER_HOST_B1, MQTT_BROKER_HOST_B2, MQTT_BROKER_HOST_B3, MQTT_BROKER_HOST_B4};
  mqttConnection = new MQTT(serverIP, MQTT_BROKER_PORT, mqttCallback);
  mqttConnection->connect("darter-" + System.deviceID());
  mqttClient = new MQTTClient(mqttConnection, System.deviceID());
  mqttClient->subscribeToTopics();
  mqttClient->publishIntroduction();

  Particle.publish("Setup Complete");
}

bool isHit(int impactReading) {
  return impactReading >= IMPACT_THRESHOLD;
}

void checkHitSensor() {
  impactSensorReading = analogRead(IMPACT_PIN);
  if(isHit(impactSensorReading) && !isHitProcessing()) {
    hitDetected = true;
  }
}

bool isHitProcessing() {
  //return player->playInProgress();
  return processingHit;
}

void endHit() {
  hitDetected = false;
  processingHit = false;

  digitalWrite(LED_PIN, LOW);
  lightshowController->playIdleShow();

  hitDelayer->reset();
  hitDelayer->stop();
}

void registerHit() {
  if(isHitProcessing()) {
    return;
  }
  hitShow();
  mqttClient->publishHit();
}

void hitShow() {
  processingHit = true;
  hitDelayer->start();

  lightshowController->playHitShow();
  digitalWrite(LED_PIN, HIGH);

  player->playTune();
}

void ticks() {
  player->tick();
  lightshowController->tick();
  mqttClient->tick();
}

void loop() {
  checkHitSensor();
  if(hitDetected && !isHitProcessing()) {
    registerHit();
  }

  ticks();
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

  }
}
