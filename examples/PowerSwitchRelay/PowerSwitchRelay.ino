 /*
  Modify Blink demo to activate the Adafruit PowerSwitchTail II based on ambient light levels:
  https://www.adafruit.com/product/268 
 */

/***********************************************
  When you come home, you turn on a light. Here, Circuit playground serves as a light sensing sentinel,
  noticing you turned on a light and, through the PowerSwitchTail II relay, turning on other lights and
  devices for you. Just tune the light sensitivity for the light you always turn on compared to the
  ambient level, and put CircuitPlayground somewhere out of the way but still able to detect the change.
  NOTE: Don't have CircuitPlayground receive light from the same device(s) it turned on, or it will be
  stuck in a self-feedback loop.
  Also: The response time is necessarily slow to avoid rapid oscillations sent to the PowerSwitch II,
  which could conceivably damage some downstream AC devices. Set this with the Slide Switch (+) or (-)
  **********************************************
 */

#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>


// Define small, medium, large range of light sensor values for Dark, Dim, and Bright modes, respectively.
static int minLight[] = {  0,   51,    256};
static int maxLight[] = { 50, 255, 1023};

#define DARK_MODE 0
#define DIM_MODE 1
#define BRIGHT_MODE 2


// Define color for light sensor pixels.
#define DARK_RED   0xFF
#define DARK_GREEN 0x00
#define DARK_BLUE  0x00

#define DIM_RED   0x00
#define DIM_GREEN 0x00
#define DIM_BLUE  0xFF

#define BRIGHT_RED   0xFF
#define BRIGHT_GREEN 0xFF
#define BRIGHT_BLUE  0xFF


// Linear interpolation function is handy for all the modes to use.
float lerp(float x, float xmin, float xmax, float ymin, float ymax) {
  if (x >= xmax) {
    return ymax;
  }
  if (x <= xmin) {
    return ymin;
  }
  return ymin + (ymax-ymin)*((x-xmin)/(xmax-xmin));
}



// the setup function runs once when you press reset or power the board
void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("Circuit Playground test!");

  CircuitPlayground.begin();

  // initialize digital pin 6 as an output for our PowerSwitchTail II.
  // Do this AFTER CircuitPlayground.begin().
  pinMode(6, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {

    boolean delayTime;
    if (CircuitPlayground.slideSwitch()) {
      // If slide is to the Left (+) then Add Delay time to avoid flickering AC power!
    delayTime = true;
  } else {
    delayTime = false;
  }



/************* TEST LIGHT SENSOR */
    // Reset all lights to off.
    for (int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, 0);
    }
    
    // Measure the light level and use it to light up its LEDs (right half).
    uint16_t light = CircuitPlayground.lightSensor();
    Serial.print("Light sensor: ");

    
    int level;


    if (light > maxLight[DIM_MODE] + 10) {

      level = (int)lerp(light, minLight[BRIGHT_MODE], maxLight[BRIGHT_MODE], 0.0, 9.0);
      Serial.print("BRIGHT @ ");
      Serial.print("LEVEL: ");
      Serial.print(level);
      Serial.print(":       ");
      
      for (int i=0; i<level+1; ++i) {
        CircuitPlayground.strip.setPixelColor(i, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_BLUE);
      }

    digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (delayTime) {delay(1000);}              // wait for a second
  
    } else if (light >= maxLight[DARK_MODE] and light <= maxLight[DIM_MODE] + 10) {      

    level = (int)lerp(light, minLight[DIM_MODE], maxLight[DIM_MODE]-10, 0.0, 9.0);

      Serial.print("DIM @: ");
      Serial.print("LEVEL: ");
      Serial.print(level);
      Serial.print(":       ");
    for (int i=0; i<level+1; ++i) {
      CircuitPlayground.strip.setPixelColor(i, DIM_RED, DIM_GREEN, DIM_BLUE);
    }
    
    digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (delayTime) {delay(1000);}              // wait for a second
  
  } else {

    // If we made it to this code block, then we must be in the dark

    level = (int)lerp(light, minLight[DARK_MODE], maxLight[DARK_MODE]-10, 0.0, 9.0);
      Serial.print("DARK @: ");
      Serial.print("LEVEL: ");
      Serial.print(level);
      Serial.print(":       ");
    for (int i=0; i<=level; ++i) {
      CircuitPlayground.strip.setPixelColor(i, DARK_RED, DARK_GREEN, DARK_BLUE);
    }
 
    digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
    if (delayTime) {delay(1000);}              // wait for a second
  }

  
  Serial.println(light);

  
  // Light up the pixels!
  CircuitPlayground.strip.show();
 


  
}
