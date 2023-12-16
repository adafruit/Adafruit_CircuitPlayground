/*A Theme - Tetris 
  with Bass
  Uses Arduino tone library pitches.h [http://arduino.cc/en/Tutorial/tone ]
       by electricmango
  https://electricmango.github.io
  
  Thanks to Gori Fater for uploading the sheet music on Google.
  http://www.gamemusicthemes.com/sheetmusic/gameboy/tetris/
  
  Version 1.0.4
  -------------------------------------------------------------------------
  A Theme - Tetris with Bass is licensed under the
  
  Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
  
  You are free to:

  Share — copy and redistribute the material in any medium or format
  Adapt — remix, transform, and build upon the material
  for any purpose, even commercially.
  The licensor cannot revoke these freedoms as long as you follow the license terms.

  Under the following terms:

  Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
  ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

  No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

  The full license is available at https://creativecommons.org/licenses/by-sa/4.0/legalcode
  
  Copyright (c) 2012 ~ 2015 electricmango
  -------------------------------------------------------------------------
  A_Theme___Tetris_with_Bass.ino
*/

#include "pitches.h"

//notes in the melody:
int melody[] = {
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0, 
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0, 
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
};

//note durations: 4 = quarter note, 8 = eighth note, etc
int noteDurations[] = {
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
};

void setup(){
  //iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 1000; thisNote++){

    /*
      to calculate the note duration, take one second divided by the note type.
      e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    */
    int noteDuration = 1000/noteDurations[thisNote];
    tone(5, melody[thisNote],noteDuration);
    
    /*
      to distinguish the notes, set a minimum time between them.
      the note's duration + 30% seems to work well:
     */
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
   
    noTone(5); //stop the tone playing:
  }
  digitalWrite(5,LOW);
}

void loop(){ /*press reset to replay. */ }


