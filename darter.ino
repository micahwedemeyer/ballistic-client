#include "definitions.h";
#include "neopixel.h";
#include "tone_test.h";
#include "pixel_test.h";
#include "MQTT.h";
#include "ArduinoJson.h";

int impactSensorReading;
volatile bool hitDetected;
bool processingHit;
MusicPlayer *player;
LightshowController *lightshowController;
Adafruit_NeoPixel *strip;
Timer *hitDelayer;
MQTT *mqttClient;

void endHit();
void mqttCallback(char* topic, byte* payload, unsigned int length) {

}

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
  mqttClient = new MQTT(serverIP, MQTT_BROKER_PORT, mqttCallback);
  mqttClient->connect("darter-" + System.deviceID());

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
  processingHit = true;

  lightshowController->playHitShow();
  digitalWrite(LED_PIN, HIGH);

  hitDelayer->start();

  Particle.publish("Hit", String(impactSensorReading));
  player->playTune();

  if(mqttClient->isConnected()) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    char deviceId[32];
    System.deviceID().toCharArray(deviceId, 32);
    root["deviceId"] = deviceId;
    root["timestamp"] = Time.now();

    char jsonString[255];
    root.printTo(jsonString, 255);

    Particle.publish(jsonString);

    mqttClient->publish(MQTT_HITS_TOPIC, jsonString);
  }
}
void ticks() {
  player->tick();
  lightshowController->tick();
}

void loop() {
  checkHitSensor();
  if(hitDetected && !isHitProcessing()) {
    registerHit();
  }

  ticks();

  if(mqttClient->isConnected()) {
    mqttClient->loop();
  }
}
