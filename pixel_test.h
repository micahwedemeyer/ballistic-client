#include "neopixel.h";

class LightshowController {
public:
  LightshowController(Adafruit_NeoPixel *strip);
  void playShow();
  void tick();
  void blank();
  void setLights();
  void advanceShow();

private:
  uint32_t wheel(byte wheelPos);
  void endShow();

  bool showPlaying;
  bool isWaiting;
  int currentPos;
  Timer *timer;
  Adafruit_NeoPixel *strip;
};




void pixelBlank(Adafruit_NeoPixel *strip);
void pixelTest(Adafruit_NeoPixel *strip);
