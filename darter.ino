#include "definitions.h";
#include "neopixel.h";
#include "tone_test.h";
#include "pixel_test.h";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_TYPE);

int impact_sensor_reading;
bool processing_hit;

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);

  // Not sure about this yet...perhaps PULLDOWN is wrong.
  pinMode(IMPACT_PIN, INPUT_PULLDOWN);

  impact_sensor_reading = 0;
  attachInterrupt(IMPACT_PIN, on_impact_sensor_change, CHANGE);

  processing_hit = false;
}

void on_impact_sensor_change() {
  impact_sensor_reading = analogRead(IMPACT_PIN);
}

bool is_hit(int impact_reading) {
  return impact_reading >= IMPACT_THRESHOLD;
}

void register_hit() {
  processing_hit = true;
  Particle.publish("Hit!");
  processing_hit = false;
}

void play_hit_tone() {
  // ???
}

void loop() {
  //toneTest();
  //pixelTest(strip);
  if(is_hit(impact_sensor_reading)) {
    register_hit();
  }
}
