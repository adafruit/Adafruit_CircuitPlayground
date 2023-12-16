// Adafruit Circuit Playground - Simon Game    Support Open Source, buy Adafruit
//   2016-08-07 Version 1  by Mike Barela for Adafruit Industries
// Uses the CircuitPlayground library to easily use the full functionality of the board
// Based on Simon Sings by @TheRealDod with permission http://goo.gl/ea4VDf

#include <Adafruit_CircuitPlayground.h>  // Library for Circuit Playground functions
#include "pitches.h"                     // File for musical tones

const int NLEDS = 4; 
const int LEDPINS[NLEDS] = {1,3,6,8};    // The NeoPixels used (counterclockwise from USB)
const int SWITCHPINS[NLEDS] = {2,0,6,9}; // Capacitive inputs 1-4 (match the NeoPixel positions)

const int FADESTEPS = 8;
const int FADEINDURATION = 200;
const int FADEOUTDURATION = 150;
const int SEQDELAY = 50;    // Millis between led flashes.
const int PAUSEB4SEQ = 500; // Millis before starting the sequence.
const int MINLEVEL =  2;   
const int MAXLEVEL = 16;
int gameLevel;
int simonSez[MAXLEVEL]; // sequence of 0..NLEDS-1

const int16_t CAP_SAMPLES   = 20;      // number of samples for each capacitive input pad
const int16_t CAP_THRESHOLD = 300;     // Threshold for a capacitive touch (higher = less sensitive)

// -- song-array note fields --
// Tone
const int NOTETONE = 0;
const int SILENCE = 0;
const int ENDOFSONG = -1;
// Duration
const int NOTEDURATION = 1;
const int SINGLEBEAT = 125; // Note duration (millis) is multiplied by this
const float PAUSEFACTOR=0.2; // relative length of silence after playing a note
// LED
const int NOTELED = 2;
const int ALLLEDS = -1;

const int NOTES[NLEDS] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4}; // Notes for each LED/Switch

int CORRECTSONG[][3] = {    // song to play if you mimiced the sequence correctly
   {SILENCE,2,ALLLEDS}
  ,{NOTE_G4,1,ALLLEDS}
  ,{NOTE_G4,1,ALLLEDS}
  ,{NOTE_A4,2,ALLLEDS}
  ,{ENDOFSONG,ENDOFSONG,ENDOFSONG}
};

int WINSONG[][3] = {       // song to play if you win the entire game
   {SILENCE,2,ALLLEDS}
  ,{NOTE_E4,1,2}
  ,{NOTE_E4,1,2}
  ,{NOTE_E4,1,2}
  ,{NOTE_F4,1,3}
  ,{NOTE_E4,1,2}
  ,{NOTE_D4,3,1}
  ,{NOTE_G4,1,ALLLEDS}
  ,{NOTE_G4,1,ALLLEDS}
  ,{NOTE_G4,1,ALLLEDS}
  ,{NOTE_A4,2,ALLLEDS}
  ,{NOTE_G4,5,ALLLEDS}
  ,{ENDOFSONG,ENDOFSONG,ENDOFSONG}
};

int LOSESONG[][3] = {   // notes to play if you don't mimic correctly and lose the game
  {NOTE_B5,2,3},{NOTE_A5,2,2},{NOTE_GS5,2,1},{NOTE_G5,8,ALLLEDS},{ENDOFSONG,ENDOFSONG,ENDOFSONG}
};

//===========================================================================================

void setup() {
  CircuitPlayground.begin();           // initialize the Circuit Playground library
  CircuitPlayground.setBrightness(30); // we don't want the NeoPixels too bright
  Serial.begin(9600);                  // Serial monitor for debugging and info
  randomSeed(analogRead(4));           // random value based on sounds
  gameLevel=MINLEVEL;                  // start game at the minimum guess level
  playWinSequence();                   // Visual feedback after reset. 
}
 
void loop() {
  int done;
  initGameSequence(gameLevel);        // Set up moves for new game
  done = 0;
  while (!done) {                     // set up to loop while playing
    while( !CircuitPlayground.leftButton() && !CircuitPlayground.rightButton() ) ; // wait to start
    delay(PAUSEB4SEQ);
    playGameSequence(gameLevel);      // Play the sequence to user
    if (playerGuess(gameLevel)) {     // See if person repeated the same sequence
      playCorrectSequence();          //   You did it right, make it harder by 1 move
      done = 1;
      if(gameLevel < MAXLEVEL) {       //   Increasing level by 1
        gameLevel++;
      }
      else {                // You played all the levels
        playWinSequence();  // You won the entire game!
        while(1) ;          // Press Circuit Playground Reset to restart  
      }
    } 
    else {
      playLoseSequence();    // You didn't get it right, sorry
      gameLevel = MINLEVEL;  // Reset to the starting level
    }
  }
}

void initGameSequence(int gameLevel) {    // Set the values for the values to mimic
  // assertion: gameLevel<=MAXLEVEL
  if( gameLevel == MINLEVEL ) {           // Minimum level all random
    for( int i=0; i < gameLevel; i++ ) {
      simonSez[i] = random(NLEDS);        // Min - select all new random pattern
    }
  }
  else {
    simonSez[gameLevel-1] = random(NLEDS); // add one more random value for next level
  }                                        //  which is different than some variations of game
}

void playGameSequence(int gameLevel) {     // Play the sequence to mimic
  Serial.print("Try this: ");
  for (int i=0; i < gameLevel; i++) {
    playLed(simonSez[i]);                  // Light LED and play its note
    Serial.print(SWITCHPINS[simonSez[i]]);
    if( i != gameLevel-1 )
       Serial.print(", ");
    else
       Serial.println(" ");
  }
}

void fadeLed(int theLed,int val,int duration) {  // fade NeoPixels in or out
  int fadeStep  = 256/FADESTEPS;
  int fadeDelay = duration/FADESTEPS;
  for(int i=0; i < 256; i+=fadeStep) {
    if( theLed >= 0 ) {
      lightPixel(theLed, val?i:255-i);
    } 
    else { // ALLLEDS
      for(int j=0; j < NLEDS; j++) {
         lightPixel(j, val?i:255-i);
      }
    }
    delay(fadeDelay);
  }
  // force val (in case fadeStep doesn't divide 256)
  if( theLed >= 0 ) {
      lightPixel(theLed, val);
  }
  else {
    for(int j=0; j < NLEDS; j++) {
      lightPixel(j, val);
    }
  }
}

void playLed(int theLed) {                            // Fade LED and play its note
  if(CircuitPlayground.slideSwitch()) {               // Only play song if slide switch on "+"
     CircuitPlayground.playTone(NOTES[theLed],100);
  }
  fadeLed(theLed,HIGH,FADEINDURATION);               // Fade in LED
  fadeLed(theLed,LOW,FADEOUTDURATION);               // Fade out LED
}

int playerGuess(int gameLevel) {         // Get the user's guess and compare to Simon's sequence
  for (int i=0 ; i < gameLevel ; i++) {
    int guess=getSwitchStroke();
    //Serial.print(guess,DEC);
    //Serial.print(",");
    //Serial.println(simonSez[i]);
    if (guess!=simonSez[i]) {
      return 0;
    } 
    else {
      playLed(guess);                   // Fade LED and play its note
    }
  }
  return 1;
}

void playSong(int song[][3]) {         // Play a predefined song sequence on CP speaker
  for (int note=0; song[note][NOTETONE]!=ENDOFSONG; note++) {
    int theDuration=SINGLEBEAT*song[note][NOTEDURATION];
    int theTone=song[note][NOTETONE];
    if ( theTone && CircuitPlayground.slideSwitch() ) {
       CircuitPlayground.playTone(theTone, 500);
    }
    int theLed=song[note][NOTELED];
    fadeLed(theLed,HIGH,theDuration);            // Fade in
    fadeLed(theLed,LOW,theDuration*PAUSEFACTOR); // Fade out + silence between note
  }
}

int playWinSequence() {     // Play the winning song and light sequence
  playSong(WINSONG);
  Serial.println("Win!");
}

int playLoseSequence() {     // Play the loosing song and light sequence
  playSong(LOSESONG);
  Serial.println("Loss");
}

int playCorrectSequence() {  // Play the song and light sequence for a correct move
  playSong(CORRECTSONG);
  Serial.println("Correct Repeat");
}

int getSwitchStroke() {     // Code to get a switch entry
  while (get1stPressedSwitch()>=0) {
    // flush everything until no switch is pressed
    delay(50);
  }
  while (get1stPressedSwitch()<0) {
    // wait for next press
    delay(50);
  }
  return get1stPressedSwitch();
}

int get1stPressedSwitch() {   // Poll the switches, return the switch being pressed or -1 for none
  for (int i=0; i<NLEDS; i++) {
    if(CircuitPlayground.readCap(SWITCHPINS[i], CAP_SAMPLES) >= CAP_THRESHOLD) { // read CP capacitive
//      Serial.print("Switch ");                                                 // switchpad
//      Serial.print(SWITCHPINS[i]); // print switch presses if you like 
//      Serial.print(", ");
//      Serial.println(i);
      return i;                      // return number of the switch pressed
    }
  }
  return -1;                         // no switch pressed this poll cycle
}

void lightPixel(uint8_t ledToLight, uint8_t intensity) {  // light the NeoPixel at passed intensity
   switch(ledToLight) {
     case(0):
       CircuitPlayground.setPixelColor(LEDPINS[ledToLight],intensity,0,0);  // red
       break;
     case(1):
       CircuitPlayground.setPixelColor(LEDPINS[ledToLight],0,intensity,0);  // green
       break;
     case(2):
       CircuitPlayground.setPixelColor(LEDPINS[ledToLight],0,0,intensity);  // blue
       break;
     case(3):
       CircuitPlayground.setPixelColor(LEDPINS[ledToLight],intensity,intensity,intensity);  // white
       break;
   }
}

