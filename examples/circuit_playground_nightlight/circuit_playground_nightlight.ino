//----------------------------------------------------------------------------------------------------
//  circuit_playground_nightlight turns the circuit playground into a nightlight
//  cycling through a list of colors. The nightlight is active if it is in the dark
//  or the slide switch is in the "right" position.
//
//  author frank morton
//  copyright 2017 base2 incorporated
//  mit license (https://opensource.org/licenses/MIT)
//----------------------------------------------------------------------------------------------------
#include <Adafruit_CircuitPlayground.h>

const int min_brightness = 3;
const int max_brightness = 35;
const int brightness_delay = 80;
const int transition_delay = 5;
const int active_nightlight_light_level = 10;
const int inactive_nightlight_delay_led_on = 500;
const int inactive_nightlight_delay_led_off = 1500;

int current_color = 0;
int colors[][3] = { { 255, 0, 0 }, { 0, 0, 223 }, { 0, 223, 0 }, { 169, 223, 20 }, { 20, 217, 223 }, { 20, 33, 223 } };
int colors_count = sizeof(colors[0]);

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
}

//----------------------------------------------------------------------------------------------------
//  set all 10 neopixels to the specified rgb color and brightness followed by a delay
//----------------------------------------------------------------------------------------------------
void set_color(int red, int green, int blue, int brightness, int brightness_interval) {
  CircuitPlayground.setBrightness(brightness);

  for(int pixel=0;pixel<10;++pixel) {
    CircuitPlayground.setPixelColor(pixel, red, green, blue);
  }

  delay(brightness_interval);
}

//----------------------------------------------------------------------------------------------------
//  determine the next color level to use when transitioning between colors
//----------------------------------------------------------------------------------------------------
int next_transition_color(int from, int to) {
  if (from < to) return(++from);
  if (from > to) return(--from);
  return(from);
}

//----------------------------------------------------------------------------------------------------
//  slowly transition from one color to the next between cycles
//----------------------------------------------------------------------------------------------------
void transition_colors(int *from_rgb_colors, int *to_rgb_colors) {
  int red = from_rgb_colors[0];
  int green = from_rgb_colors[1];
  int blue = from_rgb_colors[2];

  while ((red != to_rgb_colors[0]) || (green != to_rgb_colors[1]) || (blue != to_rgb_colors[2])) {
    red = next_transition_color(red, to_rgb_colors[0]);
    green = next_transition_color(green, to_rgb_colors[1]);
    blue = next_transition_color(blue, to_rgb_colors[2]);

    set_color(red, green, blue, min_brightness, transition_delay);
  }
}

//----------------------------------------------------------------------------------------------------
//  change to the next color cycling through different brightness levels for the specified color
//----------------------------------------------------------------------------------------------------
int change_colors(int color) {
  int *rgb_colors = colors[color];

  int red = rgb_colors[0];
  int green = rgb_colors[1];
  int blue = rgb_colors[2];

  //--------------------------------------------------------------------------------------------------
  //  first cycle with increasing brightness followed by decreasing brightness
  //--------------------------------------------------------------------------------------------------
  for(int brightness=min_brightness;brightness<=max_brightness;++brightness) set_color(red, green, blue, brightness, brightness_delay);
  for(int brightness=max_brightness;brightness>=min_brightness;--brightness) set_color(red, green, blue, brightness, brightness_delay);

  if(++color >= colors_count) color = 0;

  //--------------------------------------------------------------------------------------------------
  //  if the minimum brightness is non-zero and the nightlight is still active transition
  //  from the current color to the next by adjusting each rgb color one level at a time
  //--------------------------------------------------------------------------------------------------
  if((min_brightness > 0) && active_nightlight()) {
    int *to_rgb_colors = colors[color];

    transition_colors(rgb_colors, to_rgb_colors);
  }

  return(color);
}

//----------------------------------------------------------------------------------------------------
//  clear the neopixels and start the color cycle over if deactived (dark or move slide switch)
//----------------------------------------------------------------------------------------------------
void deactive_nightlight() {
  CircuitPlayground.clearPixels();

  current_color = 0;
}

//----------------------------------------------------------------------------------------------------
//  check if the nightlight is active (dark or slide switch in the "right" position")
//----------------------------------------------------------------------------------------------------
boolean active_nightlight() {
  //--------------------------------------------------------------------------------------------------
  //  nightlight is active if slide switch is in the "right" position
  //--------------------------------------------------------------------------------------------------
  if(CircuitPlayground.slideSwitch()) return(true);

  //--------------------------------------------------------------------------------------------------
  //  nightlight is active if it is dark enough
  //--------------------------------------------------------------------------------------------------
  if(analogRead(A5) <= active_nightlight_light_level) return(true);

  //--------------------------------------------------------------------------------------------------
  //  if nightlight is not active, deactivate and blink the red led to show the 
  //  circuit playground is still running
  //--------------------------------------------------------------------------------------------------
  deactive_nightlight();

  CircuitPlayground.redLED(HIGH);

  delay(inactive_nightlight_delay_led_on);

  CircuitPlayground.redLED(LOW);

  delay(inactive_nightlight_delay_led_off);

  return(false);
}

//----------------------------------------------------------------------------------------------------
//  cycle through the specified colors if the nightlight is active
//----------------------------------------------------------------------------------------------------
void loop() {
  if(active_nightlight()) {
    current_color = change_colors(current_color);
  }
}
