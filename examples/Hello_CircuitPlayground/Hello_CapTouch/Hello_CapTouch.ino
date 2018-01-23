#include <Adafruit_CircuitPlayground.h>

// Circuit Playground Express pads
uint8_t CPX_pads[] = {A1, A2, A3, A4, A5, A6, A7};

// Circuit Playground Classic pads
uint8_t CPC_pads[] = {3, 2, 0, 1, 12, 6, 9, 10};

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {
  if (CircuitPlayground.isExpress()) {
    //
    // Circuit Playground Express
    //
    for (int i=0; i<7; i++) {
      Serial.print(CircuitPlayground.readCap(CPX_pads[i]));
      Serial.print("\t");    
    }
  } else {
    //
    // Circuit Playground Classic
    //    
    for (int i=0; i<8; i++) {
      Serial.print(CircuitPlayground.readCap(CPC_pads[i]));
      Serial.print("\t");    
    }
  }
  
  Serial.println();
  delay(1000);
}
