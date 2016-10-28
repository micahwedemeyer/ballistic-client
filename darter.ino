#include "definitions.h";
#include "neopixel.h";
#include "tone_test.h";
#include "pixel_test.h";

int impactSensorReading;
volatile bool hitDetected;
bool processingHit;
MusicPlayer *player;
LightshowController *lightshowController;
Adafruit_NeoPixel *strip;
Timer *hitDelayer;

void endHit();

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  impactSensorReading = 0;
  processingHit = false;
  hitDetected = false;
  Particle.publish("Setup Complete");

  player = new MusicPlayer();

  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);
  lightshowController = new LightshowController(strip);
  lightshowController->blank();

  digitalWrite(LED_PIN, LOW);

  hitDelayer = new Timer(HIT_DELAY_MS, endHit, true);
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
  lightshowController->blank();

  hitDelayer->reset();
  hitDelayer->stop();
}

void registerHit() {
  if(isHitProcessing()) {
    return;
  }

  processingHit = true;

  player->playTune();
  lightshowController->playShow();

  digitalWrite(LED_PIN, HIGH);

  hitDelayer->start();

  Particle.publish("Hit", String(impactSensorReading));
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
}
