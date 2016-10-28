#include "Particle.h";
#include "neopixel.h";
#include "definitions.h";
#include "pixel_test.h";

LightshowController::LightshowController(Adafruit_NeoPixel *strip) {
  this->strip = strip;

  timer = new Timer(20, &LightshowController::advanceShow, *this);
  currentPos = 0;
  isWaiting = false;
  showPlaying = false;
}

void LightshowController::playShow() {
  showPlaying = true;
  currentPos = 0;
  this->tick();
}

void LightshowController::tick() {
  if(!showPlaying || isWaiting) {
    return;
  }

  if(currentPos <= 64) {
    this->setLights();
    strip->show();
    timer->reset();
    isWaiting = true;
  } else {
    endShow();
  }
}

void LightshowController::advanceShow() {
  timer->stop();
  isWaiting = false;
  currentPos++;
}

void LightshowController::endShow() {
  showPlaying = false;
}

void LightshowController::setLights() {
  int i;
  for(i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, this->wheel((i+currentPos) & 255));
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
