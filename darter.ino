#include "definitions.h";
#include "neopixel.h";
#include "MusicPlayer.h";
#include "LightshowController.h";
#include "ImpactSensor.h";
#include "MQTTClient.h";
#include "MQTT.h";
#include "ArduinoJson.h";
#include "papertrail.h";

ImpactSensor *impactSensor;
MusicPlayer *player;
LightshowController *lightshowController;
Adafruit_NeoPixel *strip;
MQTT *mqttConnection;
MQTTClient *mqttClient;

void endHit();
void nullCallback() {}
void mqttCallback(char* topic, byte* payload, unsigned int length);
void hitDetected(int reading);

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  PapertrailLogHandler papertailHandler(SYSLOG_HOST, SYSLOG_PORT, "ballistic-client-" + System.deviceID(), LOG_LEVEL_TRACE);

  impactSensor = new ImpactSensor(IMPACT_PIN, LED_PIN, IMPACT_THRESHOLD, HIT_DELAY_MS, &hitDetected);

  player = new MusicPlayer();

  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);
  strip->begin();
  strip->setBrightness(NEOPIXEL_BRIGHTNESS);

  lightshowController = new LightshowController(strip);
  lightshowController->playShow("idle", true, &nullCallback);

  digitalWrite(LED_PIN, LOW);

  Log.info("Initial MQTT setup");

  byte serverIP[] = {MQTT_BROKER_HOST_B1, MQTT_BROKER_HOST_B2, MQTT_BROKER_HOST_B3, MQTT_BROKER_HOST_B4};
  mqttConnection = new MQTT(serverIP, MQTT_BROKER_PORT, mqttCallback);
  mqttClient = new MQTTClient(mqttConnection, System.deviceID());
  mqttClient->connect();

  Log.info("Setup Complete");
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
  Log.info("Hit: %d", reading);
  mqttClient->publishHit();
}

void endShow() {
  Log.trace("Ending show. Back to idle.");
  lightshowController->playShow("idle", true, &nullCallback);
}

void playShow(String showId) {
  Log.info("Playing show: %s", showId.c_str());

  if(showId.equals("win") || showId.equals("lose")) {
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

  Log.trace("MQTT message received: %s", topic);

  String topicStr(topic);
  if(topicStr.endsWith("playShow")) {
    const char* showId = root["showId"];
    playShow(String(showId));
  } else if(topicStr.endsWith("requestIntroduction")) {
    mqttClient->publishIntroduction();
  }
}
