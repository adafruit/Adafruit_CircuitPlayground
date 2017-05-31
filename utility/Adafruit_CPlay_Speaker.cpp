// Adafruit Circuit Playground speaker library
// by Phil Burgess / Paint Your Dragon.

#include <Arduino.h>
#include "Adafruit_CPlay_Speaker.h"

// -------------------------------------------------------------------------

// Sets up Circuit Playground speaker for PWM audio output: enables 48 KHz
// high-speed PWM mode, configures Timer/Counter 4, sets PWM duty cycle to
// 50% (speaker idle position).

void Adafruit_CPlay_Speaker::begin(void) {
#ifdef __AVR__
  // Set up Timer4 for fast PWM on !OC4A
  PLLFRQ  = (PLLFRQ & 0xCF) | 0x30;   // Route PLL to async clk
  TCCR4A  = _BV(COM4A0) | _BV(PWM4A); // Clear on match, PWMA on
  TCCR4B  = _BV(PWM4X)  |_BV(CS40);   // PWM invert, 1:1 prescale
  TCCR4D  = 0;                        // Fast PWM mode
  TCCR4E  = 0;                        // Not enhanced mode
  TC4H    = 0;                        // Not 10-bit mode
  DT4     = 0;                        // No dead time
  OCR4C   = 255;                      // TOP
  OCR4A   = 127;                      // 50% duty (idle position) to start
  started = true;
  pinMode(5, OUTPUT);                 // Enable output
#else
  // PWM/timer not needed on CPlay Express, has true analog out.
  // Set analogWrite resolution to 8 bits to match AVR calls.
  analogWriteResolution(8);
  pinMode(A0, OUTPUT);                // Enable output
#endif
}


void Adafruit_CPlay_Speaker::enable(boolean e)  { 
#ifdef __AVR__
#else             // circuit playground express has nicer amp w/shutdown
  digitalWrite(CPLAY_SPEAKER_SHUTDOWN, e);
#endif 
}

// -------------------------------------------------------------------------

// Turns off PWM output to the speaker.

void Adafruit_CPlay_Speaker::end(void) {
#ifdef __AVR__
  TCCR4A  = 0; // PWMA off
  pinMode(5, INPUT);
#else
  pinMode(A0, INPUT);
#endif
  started = false;
}

// -------------------------------------------------------------------------

// Sets speaker position (0-255; 127=idle), enables PWM output if needed.

void Adafruit_CPlay_Speaker::set(uint8_t value) {
  if(!started) begin();
#ifdef __AVR__
  TCCR4A = value;
#else
  analogWrite(A0, value);
#endif
}

// -------------------------------------------------------------------------

// Plays digitized 8-bit audio (optionally 10 bits on Express board) from
// a PROGMEM (flash memory) buffer.  Maybe 1-3 seconds tops depending on
// sampling rate (e.g. 8000 Hz = 8 Kbytes/second).  Max ~20K space avail on
// Circuit Playground, lots more on Circuit Playground Express.
// This function currently "blocks" -- it will not play sounds in the
// background while other code runs.
void Adafruit_CPlay_Speaker::playSound(
  const uint8_t *data, uint32_t len, uint16_t sampleRate, boolean tenBit) {

  if(!started) begin();

  uint32_t i;
#ifdef __AVR__
  uint16_t interval = 1000000L / sampleRate;
#else
  uint32_t r2 = sampleRate / 2;
  uint32_t startTime = micros();
#endif

  if(tenBit) { // 10-bit audio samples?
    uint8_t loIdx = 4;
#ifdef __AVR__
    // Because it uses 8-bit PWM for output, the AVR code must filter
    // 10-bit data down to 8 bits.  This is ONLY here for compatibility
    // with sketches with 10-bit samples.  If targeting a project for AVR,
    // it's best to produce and use optimized 8-bit audio, else it's just
    // wasted space!  Timer/Counter 4 DOES offer a 10-bit mode, but it's
    // not used in this library, just not worth it in the limited flash
    // space of the 32U4 chip.
    uint16_t idx = 0;
    uint8_t  hiBits;
    for(i=0; i<len; i++) {
      if(++loIdx >= 4) {
        hiBits = pgm_read_byte(&data[idx++]);
        loIdx  = 0;
      }
      OCR4A    = ((hiBits & 0xC0) | (pgm_read_byte(&data[idx++]) >> 2));
      hiBits <<= 2; // Do this after write, because of masking op above
      delayMicroseconds(interval);
    }
    OCR4A = 127;
#else
    // Circuit Playground Express -- use 10-bit analogWrite()
    uint32_t idx = 0;
    uint16_t hiBits;
    analogWriteResolution(10);
    for(i=0; i<len; i++) {
      if(++loIdx >= 4) {
        hiBits = (uint16_t)pgm_read_byte(&data[idx++]);
        loIdx  = 0;
      }
      hiBits <<= 2; // Do this before write, because of masking op below
      analogWrite(A0, (hiBits & 0x300) | pgm_read_byte(&data[idx++]));
      while(((micros()-startTime+50)/100) < ((i*10000UL+r2)/sampleRate));
    }
    analogWriteResolution(8); // Return to 8 bits for set() compatibility
    analogWrite(A0, 127);
#endif

  } else { // 8-bit audio samples

#ifdef __AVR__
    for(i=0; i<len; i++) {
      OCR4A = pgm_read_byte(&data[i]);
      delayMicroseconds(interval);
    }
    OCR4A = 127;
#else
    for(i=0; i<len; i++) {
      analogWrite(A0, pgm_read_byte(&data[i]));
      while(((micros()-startTime+50)/100) < ((i*10000UL+r2)/sampleRate));
    }
    analogWrite(A0, 127);
#endif
  }
}
