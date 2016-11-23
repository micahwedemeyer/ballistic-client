#include "neopixel.h";

class LightshowController {
public:
  LightshowController(Adafruit_NeoPixel *strip);
  void playShow(String showId, std::function<void()> callback);
  void playIdleShow();
  void tick();
  void blank();
  void setLights();
  void setAll(uint32_t color);
  void advanceShow();

private:
  uint32_t wheel(byte wheelPos);
  void endShow();
  void hitShow();
  void goLiveShow();

  bool showPlaying;
  bool idleShowPlaying;
  bool isWaiting;
  int currentPos;
  Timer *timer;
  Timer *showTimer;
  std::function<void()> showEndCallback;
  Adafruit_NeoPixel *strip;
};




void pixelBlank(Adafruit_NeoPixel *strip);
void pixelTest(Adafruit_NeoPixel *strip);
