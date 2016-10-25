#include "Particle.h";
#include "neopixel.h";
#include "definitions.h";

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos) {
  if(WheelPos < 85) {
   return (*strip).Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return (*strip).Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return (*strip).Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void rainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<(*strip).numPixels(); i++) {
      (*strip).setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    (*strip).show();
    delay(wait);
  }
}

void pixelTest(Adafruit_NeoPixel *strip) {
  (*strip).begin();
  (*strip).show(); // Initialize all pixels to 'off'

  while(true) {
    rainbow(strip, 20);
  }
}
