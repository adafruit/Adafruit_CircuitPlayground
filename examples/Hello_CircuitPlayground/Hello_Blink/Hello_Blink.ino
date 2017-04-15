#include <Adafruit_CircuitPlayground.h>

void setup() {
  CircuitPlayground.begin();
}

void loop() {

  // Blink the Red LED
  CircuitPlayground.redLED(HIGH);
  delay(500);
  CircuitPlayground.redLED(LOW);
  delay(500);
}
