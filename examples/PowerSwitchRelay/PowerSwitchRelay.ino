 /*
  Modify Blink demo to activate the Adafruit PowerSwitchTail II:
  https://www.adafruit.com/product/268 
 */


#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>


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

/***********************************************
  When you come home, you turn on a light. Here, Circuit playground serves as a light sensing sentinel,
  noticing you turned on a light and, through the PowerSwitchTail II relay, turning on other lights and
  devices for you. Just tune the light sensitivity for the light you always turn on compared to the
  ambient level, and put CircuitPlayground somewhere out of the way but still able to detect the change.
  NOTE: Don't have CircuitPlayground receive light from the same device(s) it turned on, or it will be
  stuck in a self-feedback loop.
  **********************************************
 */

/************* TEST LIGHT SENSOR */
  Serial.print("Light sensor: ");
  Serial.println(CircuitPlayground.lightSensor());


  if (CircuitPlayground.lightSensor() >= 25) {  
    digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
  } else {  
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  }
}
