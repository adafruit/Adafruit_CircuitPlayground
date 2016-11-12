#include <Adafruit_CircuitPlayground.h>

uint8_t pads[] = {3, 2, 0, 1, 12, 6, 9, 10};

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {
  for (int i=0; i<8; i++) {
    Serial.print(CircuitPlayground.readCap(pads[i]));
    Serial.print("\t");
  }
  Serial.println();
   
  delay(1000);
}
