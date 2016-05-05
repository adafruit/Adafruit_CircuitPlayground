#include <Adafruit_CircuitPlayground.h>


boolean Adafruit_CircuitPlayground::begin(uint8_t brightness) {
  pinMode(CPLAY_REDLED, OUTPUT);
  pinMode(CPLAY_SLIDESWITCHPIN, INPUT);
  pinMode(CPLAY_LEFTBUTTON, INPUT);
  pinMode(CPLAY_RIGHTBUTTON, INPUT);
  pinMode(CPLAY_BUZZER, OUTPUT);
  pinMode(CPLAY_CAPSENSE_SHARED, OUTPUT);

  strip = Adafruit_CPlay_NeoPixel(10, CPLAY_NEOPIXELPIN, NEO_GRB + NEO_KHZ800);
  lis = Adafruit_CPlay_LIS3DH(CPLAY_LIS3DH_CS);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(brightness);

  cap[0] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 0);
  cap[1] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 1);
  cap[2] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 2);
  cap[3] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 3);
  cap[4] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 6);
  cap[5] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 9);
  cap[6] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 10);
  cap[7] = CPlay_CapacitiveSensor(CPLAY_CAPSENSE_SHARED, 12);
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    return false;
  }
  return true;
}

uint16_t Adafruit_CircuitPlayground::readCap(uint8_t p, uint8_t samples) {
  switch (p) {
  case 0:
    return cap[0].capacitiveSensor(samples);
  case 1:
    return cap[1].capacitiveSensor(samples);
  case 2:
    return cap[2].capacitiveSensor(samples);
  case 3:
    return cap[3].capacitiveSensor(samples);
  case 6:
    return cap[4].capacitiveSensor(samples);
  case 9:
    return cap[5].capacitiveSensor(samples);
  case 10:
    return cap[6].capacitiveSensor(samples);
  case 12:
    return cap[7].capacitiveSensor(samples);
  default:
    return 0;
  }
}


// just turn on/off the red #13 LED
void Adafruit_CircuitPlayground::redLED(boolean v) {
  digitalWrite(CPLAY_REDLED, v);
}

// just read the slide switch
boolean Adafruit_CircuitPlayground::slideSwitch(void) {
  return digitalRead(CPLAY_SLIDESWITCHPIN);
}

// just read the left button
boolean Adafruit_CircuitPlayground::leftButton(void) {
  return digitalRead(CPLAY_LEFTBUTTON);
}

// just read the right button
boolean Adafruit_CircuitPlayground::rightButton(void) {
  return digitalRead(CPLAY_RIGHTBUTTON);
}

void Adafruit_CircuitPlayground::playTone(uint16_t freq, uint16_t time) {
  tone(CPLAY_BUZZER, freq, time);
}

uint16_t Adafruit_CircuitPlayground::lightSensor(void) {
  return analogRead(CPLAY_LIGHTSENSOR);
}

uint16_t Adafruit_CircuitPlayground::soundSensor(void) {
  return analogRead(CPLAY_SOUNDSENSOR);
}


float Adafruit_CircuitPlayground::motionX(void) {
  sensors_event_t event;
  CircuitPlayground.lis.getEvent(&event);
  return event.acceleration.x;
}

float Adafruit_CircuitPlayground::motionY(void) {
  sensors_event_t event;
  CircuitPlayground.lis.getEvent(&event);
  return event.acceleration.y;
}

float Adafruit_CircuitPlayground::motionZ(void) {
  sensors_event_t event;
  CircuitPlayground.lis.getEvent(&event);
  return event.acceleration.z;
}

float Adafruit_CircuitPlayground::temperature(void) {
   // Thermistor test
  float reading;

  reading = analogRead(CPLAY_THERMISTORPIN);

  //Serial.print("Thermistor reading: "); Serial.println(reading);

  // convert the value to resistance
  reading = ((1023.0 * SERIESRESISTOR) / reading);
  reading -= SERIESRESISTOR;

  //Serial.print("Thermistor resistance: "); Serial.println(reading);

  float steinhart;
  steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  return steinhart;
}
  
// Get the temperature in degrees Fahrenheit
float Adafruit_CircuitPlayground::temperatureF(void) {
  
   // Thermistor test
  float reading;

  reading = analogRead(CPLAY_THERMISTORPIN);

  //Serial.print("Thermistor reading: "); Serial.println(reading);

  // convert the value to resistance
  reading = ((1023.0 * SERIESRESISTOR) / reading);
  reading -= SERIESRESISTOR;

  //Serial.print("Thermistor resistance: "); Serial.println(reading);

  float steinhart;
  steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  
  steinhart = steinhart * 1.8 + 32;

  return steinhart;
  
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Adafruit_CircuitPlayground::colorWheel(uint8_t WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// instantiate static
Adafruit_CircuitPlayground CircuitPlayground;
