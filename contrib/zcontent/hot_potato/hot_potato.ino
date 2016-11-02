#include <Adafruit_CircuitPlayground.h>
/*
 * Hot potato game using Adafruit's Circuit Playground
 * Initial version by Dan Cogliano (dan@zcontent.net) May 2016
 * Utilizes the device accelerometer, buzzer, neopixels,
 * buttons and slide switch
 * 
 * Features:
 * Neopixel counter is displayed for each toss
 * Random # of tosses each game
 * When switch enabled, players can also be eliminated if "potato" held too long
 * Flashing red neopixels and sound occurs when a player is eliminated
 * 
 * Game Rules: 
 * With 2 or more players, press the left or right button to start a new game.
 * Throw the "potato" from one person to another until you are eliminated
 * by the buzzer.  Use the slide switch to add a countdown timer while holding 
 * the "potato", where you could also be eliminated by the buzzer if you hold 
 * the "potato" too long.
 * 
 * Last person standing wins!
 * 
 * Setup: 
 * Use one or more rubber bands to keep board and battery together
 * for tossing (or create/find a case).
 * 
 */
 
// # of tosses is random but no more than MAXTOSSES
#define MAXTOSSES 20
// HOTNESS eliminates player who holds potato for this amount
// 20 per second (40 = 2 seconds)
#define HOTNESS 40

uint8_t inflight = false;
uint8_t activegame = false;
int tosscounter = 0;
int hotcounter = 0;
int remainingtosses = 0;

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
}

void newGame()
{
  remainingtosses = random(1,MAXTOSSES);
  tosscounter = 0;
  hotcounter = 0;
  inflight = false;
  activegame = true;
  // tones for beginning of game
  for(int i = 0; i < 3; i++)
  {
    // visual countdown
    setPixelCounter(3 - i);
    // audio countdown
    CircuitPlayground.playTone(440, 250);
    delay(500);
  }
  CircuitPlayground.clearPixels();
  CircuitPlayground.playTone(880, 500);
  delay(500);
  // show starting toss tosscounter
  setPixelCounter(tosscounter);
}

void endGame()
{
  activegame = false;
  CircuitPlayground.clearPixels();
}

void showAlert()
{
  for(int j = 0; j < 4; j++)
  {
    for(int i= 0; i < 10; i++)
    {
      CircuitPlayground.setPixelColor(i,0xff,0x00,0x00);
    }
    CircuitPlayground.playTone(220, 250);
    delay(250);
    CircuitPlayground.clearPixels();
    delay(250);
  }
}

void setPixelCounter(int counter)
{
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(counter%10, CircuitPlayground.colorWheel(25 * (counter%10)));
}

void loop() {
  delay(50);
  if(activegame)
  {
    // get accelerometer vector
    float x = CircuitPlayground.motionX();
    float y = CircuitPlayground.motionY();
    float z = CircuitPlayground.motionZ();
    float accel = x*x + y*y + z*z;
    if(accel < 2)
    {
      // in flight
      inflight = true;
      hotcounter = 0;
    }
    else if(inflight) // landing now
    {
      inflight = false;
      tosscounter++;
      remainingtosses--;
      // update neopixel toss tosscounter
      setPixelCounter(tosscounter);
      if(remainingtosses < 0)
      {
        // this player is out!
        endGame();
        showAlert();
      }
    }
    hotcounter++;
    if(CircuitPlayground.slideSwitch() && hotcounter > HOTNESS)
    {
      // held too long, this player is out!
      endGame();
      showAlert();
    } 
  }
  if(CircuitPlayground.leftButton() || CircuitPlayground.rightButton())
  {
    newGame();
  }
}
