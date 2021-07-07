// Adafruit Circuit Playground - Melody    Support Open Source, buy at Adafruit
//   2016-08-06 Version 1  by Mike Barela for Adafruit Industries
//   Adapted from melody by Tom Igoe on arduino.cc
// Uses the CircuitPlayground library to easily use the full functionality of the board

#include <Adafruit_CircuitPlayground.h>
#include "pitches.h"

const int numNotes = 8;                     // number of notes we are playing
int melody[] = {                            // specific notes in the melody
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };

int noteDurations[] = {     // note durations: 4 = quarter note, 8 = eighth note, etc.:
  4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  CircuitPlayground.begin();  // initialize the CP library
}

void loop() {
  if(CircuitPlayground.rightButton()) {   // play when we press the right button
    for (int thisNote = 0; thisNote < numNotes; thisNote++) { // play notes of the melody
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      CircuitPlayground.playTone(melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      //   the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
    }
  }
}
