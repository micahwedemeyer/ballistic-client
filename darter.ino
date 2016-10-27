#include "definitions.h";
#include "neopixel.h";
#include "tone_test.h";
#include "pixel_test.h";

int impactSensorReading;
volatile bool hitDetected;
bool processingHit;
MusicPlayer *player;
Adafruit_NeoPixel *strip;
Timer *hitDelayer;

void endHit();

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);

  impactSensorReading = 0;
  processingHit = false;
  hitDetected = false;
  Particle.publish("Setup Complete");

  player = new MusicPlayer();

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
  hitDelayer->reset();
  hitDelayer->stop();
}

void registerHit() {
  if(isHitProcessing()) {
    return;
  }

  processingHit = true;
  Particle.publish("Hit", String(impactSensorReading));
  playHitTune();
  digitalWrite(LED_PIN, HIGH);

  hitDelayer->start();
}

void playHitTune() {
  player->playTune();
}

void loop() {
  //pixelTest(strip);

  checkHitSensor();
  if(hitDetected && !isHitProcessing()) {
    registerHit();
  }
  player->tick();
}
