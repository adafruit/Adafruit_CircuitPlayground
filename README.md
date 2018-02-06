# Fork of Adafruit_CircuitPlayground
library for Circuit Playground board that will disable the neopixel portions if FastLED is included before it.

### If this library is included before FastLED, you will have both available to use and possibly conflict in setting color, brightness, etc.

## I created this fork because I perfer to use FastLED to control addressable leds.

To use FastLED make sure the #include <FastLED.h> is before this library

=======
# Adafruit CircuitPlayground Library [![Build Status](https://travis-ci.org/adafruit/Adafruit_CircuitPlayground.svg?branch=master)](https://travis-ci.org/adafruit/Adafruit_CircuitPlayground)

<img src="https://cdn-shop.adafruit.com/970x728/3333-01.jpg" height="300"/>

This is a library for the Adafruit CircuitPlayground boards:
  * https://www.adafruit.com/products/3333
  * https://www.adafruit.com/product/3000
 
Check out the links above for our tutorials and wiring diagrams.

Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

Written by Limor Fried/Ladyada and others for Adafruit Industries.
MIT license, all text above must be included in any redistribution
