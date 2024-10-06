#include "slider.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

void update_slider_position(Custom_slider *s) {
  if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    s->rect.x = GetMouseX();
    s->rect.y = GetMouseY() + 20;
  }
}

void update_slider_value(Custom_slider *s, SliderValueUpdateCallback callback) {
  static int mouseX_before_click = -1;
  static int mouseX_after_click = -1;

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    mouseX_after_click = GetMouseX();

    // update the slider value based on the mouse movement
    int delta = mouseX_after_click - mouseX_before_click;
    if (mouseX_before_click != -1) {
      float new_value = s->value + (delta / s->rect.width) * s->max;

      // clamp value between 0 and max
      if (new_value < 0.0f) {
        new_value = 0.0f;
      } else if (new_value > s->max) {
        new_value = s->max;
      }

      s->value = new_value;

      // call the callback function if it's not NULL
      if (callback) {
        callback(new_value);
      }
    }

    mouseX_before_click = mouseX_after_click;
  } else {
    mouseX_before_click = -1;
  }
}

void update_opacity_value(Custom_slider *s) {
  // constants for animation duration and target opacity
  const float duration = s->duration; // animation duration in seconds
  static float elapsed = 0.0f;        // static to retain value across frames
  static float start_opacity = 0.0f;  // starting opacity value
  static float target_opacity = 0.0f; // target opacity value

  // if the left mouse button is pressed
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    // set target opacity to 1.0 when pressed
    if (target_opacity != 1.0f) {
      start_opacity = s->opacity;
      target_opacity = 1.0f;
      elapsed = 0.0f; // reset elapsed time
    }
  } else {
    // set target opacity to 0.0 when released
    if (target_opacity != 0.0f) {
      start_opacity = s->opacity;
      target_opacity = 0.0f;
      elapsed = 0.0f; // reset elapsed time
    }
  }

  // increment elapsed time by frame time
  elapsed += GetFrameTime();

  // calculate the animation progress based on elapsed time and duration
  float progress = elapsed / duration;

  // make sure the progress does not exceed 1.0
  if (progress > 1.0f) {
    progress = 1.0f;
  }

  // update the current opacity based on the progress
  // and the opacity will be used with the Fade() function to make the component
  // transparent
  s->opacity = Lerp(start_opacity, target_opacity, progress);
}

void draw_slider(Custom_slider *s) {
  // draw the outer rectangle
  DrawRectangleRec(s->rect, Fade(s->color, s->opacity));

  // calculate the width of the inner rectangle based on the slider value
  s->value_in_pixels = (s->value / s->max) * s->rect.width;

  Rectangle inner_rect = {
      s->rect.x,          // start at the same x as the outer rectangle
      s->rect.y,          // start at the same y as the outer rectangle
      s->value_in_pixels, // width based on the value
      s->rect.height      // same height as the outer rectangle
  };

  // draw the inner rectangle
  DrawRectangleRec(inner_rect, Fade(s->inner_color, s->opacity));

  // draw outer lines
  DrawRectangleLinesEx(s->rect, 5, Fade(BLACK, s->opacity));

  // draw the value on the right or left
  // I choose left
  int fontSize = 15;

  // make the string
  char buffer[10];
  sprintf(buffer, "%d%%", (int)s->value);

  // measure text width
  int text_width = MeasureText(buffer, fontSize);

  // the center along the y-axis
  int center_height = s->rect.y + (s->rect.height - fontSize) / 2;
  int padding = (int)(s->rect.width * 5 / 100);

  // draw the text on the side of the slider;
  DrawText(buffer, s->rect.x + s->rect.width - (text_width + padding),
           center_height, fontSize, Fade(BLACK, s->opacity));
}

// no need for these?
float ORIGINAL_WIDTH = -1.0f;
float ORIGINAL_X = -1.0f;
/*
 *
 * should probably save the value of the mouse position as soon as the value
 * exceeds the max whilst clicking. and get the distance from THERE.
 *
 */
void update_rect_on_out_of_bounds(Custom_slider *s) {

  static float original_width = -1.0f;
  static float original_x = -1.0f;
  static float resist_power =
      0.05; /* the higher this value, the less it will strech */
  // initialize original_width only if it has not been set before
  if (original_width < 0.0f) {
    original_width = s->rect.width;
  }
  if (original_x < 0.0f) {
    original_x = GetMouseX();
    printf("Initializing original_x: %.2f\n", original_x);
  }
  static float distance_from_rect = -1.0f;

  if (s->opacity > 0.1f && GetMouseX() > s->rect.x + s->rect.width) {
    // handle dragging past the max value
    distance_from_rect = GetMouseX() - (s->rect.x + s->rect.width);

    // reduce the stretching amount the further the mouse is from the max value
    float stretching_factor = 1.0f / (1.0f + distance_from_rect * resist_power);
    float stretching_amount = distance_from_rect * stretching_factor;

    s->rect.width = original_width + stretching_amount;

  } else if (s->opacity > 0.1f && GetMouseX() < s->rect.x) {

    distance_from_rect = original_x - GetMouseX();
    //  for the other direction: move the rect by x amount the left, while also
    //  streching it to the right
    float stretching_factor = 1.0f / (1.0f + distance_from_rect * resist_power);
    float stretching_amount = distance_from_rect * stretching_factor;

    s->rect.width = original_width + stretching_amount;
    s->rect.x = original_x - stretching_amount;
  } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    distance_from_rect = -1.0f;
    s->rect.width = original_width;
    original_width = -1.0f;
    original_x = -1.0f;
  }
}
