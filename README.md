# Fork of Adafruit_CircuitPlayground
library for Circuit Playground board that will disable the neopixel portions if FastLED is included before it.

### If this library is included before FastLED, you will have both available to use and possibly conflict in setting color, brightness, etc.

## I created this fork because I perfer to use FastLED to control addressable leds.

To use FastLED make sure the #include <FastLED.h> is before this library
