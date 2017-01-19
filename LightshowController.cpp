#include "Particle.h";
#include "neopixel.h";
#include "definitions.h";
#include "LightshowController.h";

LightshowController::LightshowController(Adafruit_NeoPixel *strip) {
  this->strip = strip;

  timer = new Timer(20, &LightshowController::advanceShow, *this);
  isWaiting = false;
  showPlaying = false;

  currentShow = "";
  loopShow = false;
  showSteps = 0;
  currentStep = 0;
}

void LightshowController::playShow(String showId, bool loop, std::function<void()> callback) {
  showEndCallback = callback;
  loopShow = loop;
  currentShow = showId;
  currentStep = 0;

  showPlaying = true;
}

void LightshowController::tick() {
  if(isWaiting) {
    return;
  }

  if(currentShow.equals("idle")) {
    idleShow();
  } else if(currentShow.equals("live")) {
    liveShow();
  } else if(currentShow.equals("win")) {
    winShow();
  } else if(currentShow.equals("lose")) {
    loseShow();
  }
}

void LightshowController::idleShow() {
  showSteps = 255;

  this->setLights();
  strip->show();
  timer->changePeriod(20);
  timer->reset();

  isWaiting = true;
}

void LightshowController::liveShow() {
  showSteps = 1;

  uint32_t c = strip->Color(255, 0, 0);
  setAll(c);
  strip->setBrightness(255);
  strip->show();

  // WARNING!!! These software timers really seem to interfere with the
  // tone library. If we change the period or the timer ends/interrupts
  // while tone is playing, it will cause stuttering and/or tone stops.
  timer->changePeriod(2000);
  timer->reset();

  isWaiting = true;
}

void LightshowController::winShow() {
  showSteps = 1;

  uint32_t c = strip->Color(255, 0, 255);
  setAll(c);
  strip->show();

  timer->changePeriod(2000);
  timer->reset();

  isWaiting = true;
}

void LightshowController::loseShow() {
  showSteps = 1;

  blank();

  timer->changePeriod(2000);
  timer->reset();

  isWaiting = true;
}

void LightshowController::advanceShow() {
  timer->stop();
  isWaiting = false;
  currentStep += 1;

  if(currentStep > showSteps) {
    currentStep = 0;
    if(!loopShow) {
      endShow();
    }
  }
}

void LightshowController::endShow() {
  currentShow = "";
  showPlaying = false;
  strip->setBrightness(128);
  showEndCallback();
}

void LightshowController::setLights() {
  int i;
  for(i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, this->wheel((i+currentStep) & 255));
  }
}

void LightshowController::setAll(uint32_t color) {
  for(int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, color);
  }
}

void LightshowController::blank() {
  setAll(strip->Color(0,0,0));
  strip->show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LightshowController::wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
