#include "neopixel.h";

class LightshowController {
public:
  LightshowController(Adafruit_NeoPixel *strip);
  void playShow(String showId, bool loop, std::function<void()> callback);

  void tick();
  void blank();
  void setLights();
  void setAll(uint32_t color);

private:
  uint32_t wheel(byte wheelPos);
  void idleShow();
  void liveShow();
  void winShow();
  void endShow();
  void advanceShow();

  bool showPlaying;
  bool isWaiting;

  String currentShow;
  bool loopShow;
  int showSteps;
  int currentStep;

  Timer *timer;
  std::function<void()> showEndCallback;
  Adafruit_NeoPixel *strip;
};




void pixelBlank(Adafruit_NeoPixel *strip);
void pixelTest(Adafruit_NeoPixel *strip);
