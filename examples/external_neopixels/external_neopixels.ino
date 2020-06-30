#include <Adafruit_CircuitPlayground.h>
// do NOT include the standard NeoPixel library

#define NEOPIX_PIN    A2
#define NUM_PIXELS    5

// use Adafruit_CPlay_NeoPixel to create a separate external NeoPixel strip
Adafruit_CPlay_NeoPixel strip = Adafruit_CPlay_NeoPixel(NUM_PIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // initialize the Circuit Playground as usual
  // this will initialize the onboard NeoPixels as well
  CircuitPlayground.begin();

  // initialize external NeoPixel strip separately
  strip.begin();
}

void loop() {
  // for the on board NeoPixels, use the CircuitPlayground functions
  CircuitPlayground.clearPixels();
  
  // for the external NeoPixels, must use the Adafruit_CPlay_NeoPixel functions directly
  strip.clear();
  strip.show();

  delay(500);

  CircuitPlayground.setPixelColor(0, 255,   0,   0);
  CircuitPlayground.setPixelColor(1, 128, 128,   0);
  CircuitPlayground.setPixelColor(2,   0, 255,   0);
  CircuitPlayground.setPixelColor(3,   0, 128, 128);
  CircuitPlayground.setPixelColor(4,   0,   0, 255);
  
  CircuitPlayground.setPixelColor(5, 0xFF0000);
  CircuitPlayground.setPixelColor(6, 0x808000);
  CircuitPlayground.setPixelColor(7, 0x00FF00);
  CircuitPlayground.setPixelColor(8, 0x008080);
  CircuitPlayground.setPixelColor(9, 0x0000FF);

  // some functions are the same
  strip.setPixelColor(0, 255,   0,   0);
  strip.setPixelColor(1, 128, 128,   0);
  strip.setPixelColor(2,   0, 255,   0);
  strip.setPixelColor(3,   0, 128, 128);
  strip.setPixelColor(4,   0,   0, 255);
  
  // but for the external strip, must call show()
  strip.show();

  delay(5000);
}
