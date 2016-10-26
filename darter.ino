#include "definitions.h";
#include "neopixel.h";
#include "tone_test.h";
#include "pixel_test.h";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);

int impact_sensor_reading;
bool hit_detected;
bool processing_hit;
MusicPlayer *player;

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(IMPACT_PIN, INPUT);

  impact_sensor_reading = 0;
  processing_hit = false;
  hit_detected = false;
  Particle.publish("Setup Complete");

  player = new MusicPlayer();
}

bool is_hit(int impact_reading) {
  return impact_reading >= IMPACT_THRESHOLD;
}

void check_hit_sensor() {
  if(hit_detected || processing_hit) {
    return;
  }

  impact_sensor_reading = analogRead(IMPACT_PIN);
  if(is_hit(impact_sensor_reading)) {
    hit_detected = true;
  }
}

void register_hit() {
  if(processing_hit) {
    return;
  }

  processing_hit = true;
  Particle.publish("Hit!", String(impact_sensor_reading));
  delay(1000);
  hit_detected = false;
  processing_hit = false;
}

void play_hit_tone() {
  // ???
}

void loop() {
  //pixelTest(&strip);
  player->playTune();
  /*check_hit_sensor();
  if(hit_detected) {
    register_hit();
  }*/
}
