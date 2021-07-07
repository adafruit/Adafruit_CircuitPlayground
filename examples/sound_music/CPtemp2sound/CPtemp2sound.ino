// Adafruit Circuit Playground - Temperature to Sound  Support Open Source, buy Adafruit
//   2016-08-07 Version 1  by Mike Barela for Adafruit Industries
// Uses the CircuitPlayground library to easily use the full functionality of the board

#include <Adafruit_CircuitPlayground.h>

const float alertTemp = 90.0;  // temperature to alert on (use 32.0 for a freezer etc.)

void setup() {
  CircuitPlayground.begin();   // initialize the Circuit Playground library
  Serial.begin(9600);
}
 
void loop() {
   float temp; 
   uint16_t sound;
   if(CircuitPlayground.slideSwitch()) {       // if the slide switch is at "+"
     temp = CircuitPlayground.temperatureF();  //   read the light sensor
     Serial.println(temp);
     sound = (int) map(temp, 70.0, 100.0, 131.0, 1760.0);   // map light to music values
     CircuitPlayground.playTone(sound, 100);   //   play sound for 100 milliseconds
   }
   else {   // switch set to "-" for absolute temperature measurement
     temp = CircuitPlayground.temperatureF(); // read the light sensor
     Serial.println(temp);
     if( temp > alertTemp ) {   // if the read temperature is > your prepicked alartTemp
       CircuitPlayground.playTone(3520, 100); // play sound for 100 milliseconds   
     }
   }
}

