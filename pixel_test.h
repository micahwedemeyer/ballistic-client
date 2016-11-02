#include "neopixel.h";

class LightshowController {
public:
  LightshowController(Adafruit_NeoPixel *strip);
  void playHitShow();
  void playIdleShow();
  void tick();
  void blank();
  void setLights();
  void setAll(uint32_t color);
  void advanceShow();

private:
  uint32_t wheel(byte wheelPos);
  void endShow();

  bool hitShowPlaying;
  bool idleShowPlaying;
  bool isWaiting;
  int currentPos;
  Timer *timer;
  Adafruit_NeoPixel *strip;
};




void pixelBlank(Adafruit_NeoPixel *strip);
void pixelTest(Adafruit_NeoPixel *strip);
