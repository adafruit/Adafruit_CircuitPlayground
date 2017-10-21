# Adafruit Circuit Playground Library
Library to program [Circuit Playground Classic](https://www.adafruit.com/product/3000) and [Circuit Playground Express](https://www.adafruit.com/product/3333) in C++ with the Arduino IDE.

## Installation
On the Arduino IDE (v. 1.8.5+) use the Library Manager in order to install this library. Documentation can be found on the Adafruit's learning

## Usage

This Readme is not as exhaustive as the example code or the official documentation that can be found [here (CPX)](https://learn.adafruit.com/adafruit-circuit-playground-express) and [here (CPC)](https://learn.adafruit.com/introducing-circuit-playground).
It is intended to be a reference where to find the listing of the library's functions at a glance.


### List of the methods in the `Adafruit_CircuitPlayground` class


- **Red LED**
  - `void redLED(boolean v)`


- **Slide Switch**
  - `boolean slideSwitch(void)`


- **Push Buttons**
  - Left : `boolean leftButton(void)`
  - Right : `boolean rightButton(void)`


- **Light Sensor**
  - `uint16_t lightSensor(void)`


- **Neopixels**
  - `void clearPixels(void)`
  - `void setPixelColor(uint8_t p, uint32_t c)`
  - `void setPixelColor(uint8_t p, uint8_t r, uint8_t g, uint8_t b)`
  - `void setBrightness(uint16_t b)`
  - `uint32_t colorWheel(uint8_t x)`
  - Basic color detection:
    - `void senseColor(uint8_t& red, uint8_t& green, uint8_t& blue)`
    - ` uint32_t senseColor()`



- **Capacitive touch**
  - `uint16_t readCap(uint8_t p, uint8_t samples=10)`


- **Temperature sensor**
  - Celsius : `float temperature(void)`
  - Farenheit `float temperatureF(void)`


- **Microphone**
  - `uint16_t soundSensor(void)`


- **Buzzer**
  - `void playTone(uint16_t freq, uint16_t time, boolean wait=true)`


- **Accelerometer**
  - `float motionX(void)`
  - `float motionY(void)`
  - `float motionZ(void)`


- **Infra Red** _[Circuit Playground Express only]_    
  IR send and receive LEDs are used via the [`IRLib2`](https://github.com/cyborg5/IRLib2) Library and are accessible via the `irSend` and `irReceiver` properties of the `Adafruit_CircuitPlayground` class
