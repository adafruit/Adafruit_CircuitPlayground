// Adafruit Circuit Playground speaker library
// by Phil Burgess / Paint Your Dragon.

#include <Arduino.h>
#include "Adafruit_CPlay_Speaker.h"

// -------------------------------------------------------------------------

// Sets up Circuit Playground speaker for PWM audio output: enables 48 KHz
// high-speed PWM mode, configures Timer/Counter 4, sets PWM duty cycle to
// 50% (speaker idle position).

void Adafruit_CPlay_Speaker::begin(void) {
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
  pinMode(5, OUTPUT);                // Enable output
}

// -------------------------------------------------------------------------

// Turns off PWM output to the speaker.

void Adafruit_CPlay_Speaker::end(void) {
  pinMode(5, INPUT);
  TCCR4A  = 0; // PWMA off
  started = false;
}

// -------------------------------------------------------------------------

// Sets speaker position (0-255; 127=idle), enables PWM output if needed.

void Adafruit_CPlay_Speaker::set(uint8_t value) {
  if(!started) begin();
  TCCR4A = value;
}

// -------------------------------------------------------------------------

// Plays digitized 8-bit audio from a PROGMEM (flash memory) buffer.  Maybe
// 1-3 seconds tops depending on sampling rate (e.g. 8000 Hz data =
// 8 Kbytes/second).  Max ~20K space avail on Circuit Playground.
// This function currently "blocks" -- it will not play sounds in the
// background while other code runs.

void Adafruit_CPlay_Speaker::playSound(
  const uint8_t *data, uint16_t bytesToGo, uint16_t sampleRate) {

  if(sampleRate < 7620) sampleRate = 7620; // Because 8-bit delay counter

  uint8_t interval = (F_CPU / 4 + sampleRate / 2) / sampleRate - 6,
          x, counter;

  if(!started) begin();

  // Trying cycle counting here (instead of interrupt) to avoid ISR
  // conflicts (some sketches may need their own timers or related
  // interrupts -- a precious resource!).  Could also try this using
  // micros(), but that might lack adequate resolution.
  asm volatile(
   "movw   r30         , %[data]"     "\n\t" // Z = data
   "audioSampleLoop:"
    "lpm   %[x]        , Z+"          "\n\t" // x = pgm_read_byte(data++)
    "sts   %[pwmReg]   , %[x]"        "\n\t" // OCR4A = x
    "mov   %[counter]  , %[interval]" "\n\t" // counter = interval
    "audioDelayLoop:"                 "\n\t"
     "nop"                            "\n\t"
     "dec  %[counter]"                "\n\t" // counter--
     "brne audioDelayLoop"            "\n\t" // if(counter) goto audioDelay
    "sbiw  %[bytesToGo], 1"           "\n\t" // bytesToGo--
    "brne  audioSampleLoop"           "\n"   // if(bytesToGo) goto audioSample
   : [x]         "+r" (x),
     [counter]   "+r" (counter),
     [bytesToGo] "+w" (bytesToGo)
   : [data]       "e" (data),
     [pwmReg]     "M" (_SFR_ADDR(OCR4A)),
     [interval]   "r" (interval)
   : "r30", "r31" ); // Z is clobbered

  OCR4A = 127; // Idle position for next sound
}
