// Adafruit Circuit Playground - Light Theremin    Support Open Source, buy at Adafruit
//   2016-08-07 Version 1  by Mike Barela for Adafruit Industries
// Uses the CircuitPlayground library to easily use the full functionality of the board

#include <Adafruit_CircuitPlayground.h>

void setup() {
  CircuitPlayground.begin();   // initialize the Circuit Playground library
}
 
void loop() {
   int value , sound;
   if(CircuitPlayground.slideSwitch()) {      // if the slide switch is on
     value = CircuitPlayground.lightSensor(); //   read the light sensor
     sound = map(value, 5, 1000, 131, 1760);  //   map light values to music values
     CircuitPlayground.playTone(sound, 100);  //   play sound for 100 milliseconds
   }
}

