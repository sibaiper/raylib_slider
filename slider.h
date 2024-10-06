#ifndef CUSTOM_SLIDER_H
#define CUSTOM_SLIDER_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
  Rectangle rect; // position and size of the button
  Color color;    // should use the opacity
  Color inner_color;
  float max;   // maximum value of the slider
  float value; // value of the slider
  float value_in_pixels;
  float opacity;  // from 0 to 1
  bool visiable;  // indicates if the button is Initialized
  float duration; // duration of the animation in seconds
} Custom_slider;

typedef void (*SliderValueUpdateCallback)(float new_value);

// function prototypes
void update_slider_position(Custom_slider *s);
void update_slider_value(Custom_slider *s, SliderValueUpdateCallback callback);
void update_opacity_value(Custom_slider *s);
void draw_slider(Custom_slider *s);

#endif // CUSTOM_SLIDER_H
