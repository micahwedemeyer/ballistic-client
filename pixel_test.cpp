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
  this->setLights();
  strip->show();
  timer->reset();
  isWaiting = true;
}

void LightshowController::advanceShow() {
  timer->stop();
  isWaiting = false;

  currentPos++;
  if(currentPos > 64) {
    //currentPos = 0;
    endShow();
  }
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

void LightshowController::blank() {
  int i;
  for(i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, strip->Color(0,0,0));
  }
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
