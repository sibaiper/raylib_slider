#include "slider.h"
#include <raylib.h>
#include <stdio.h>

// callback function to call when sliding:
void my_callback(float new_value) {
  char buffer[10];
  int fontSize = 90;

  sprintf(buffer, "%d%%", (int)new_value);

  // calc the center of the text
  int text_width = MeasureText(buffer, fontSize);
  int center_height = (GetScreenHeight() - fontSize) / 2;

  DrawText(buffer, (GetScreenWidth() / 2) - (text_width / 2), center_height,
           fontSize, BLACK);
}

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "slider");

  Rectangle slider_rect = {20, 20, 200, 40};
  Color slider_color = {200, 200, 200, 255};
  Color inner_color = {170, 170, 170, 255};
  Custom_slider my_slider = {slider_rect, slider_color, inner_color,
                             200.0f,      100.0f,       0.0f,
                             0,           false,        0.35f};
  SliderValueUpdateCallback callback =
      my_callback; // assigning the function to the callback

  SetTargetFPS(60);

  int fontSize = 40;

  char *slider_component_header = "Slider Component - Raylib";

  // calc the center of the text
  int text_width = MeasureText(slider_component_header, fontSize);
  int center_height = (GetScreenHeight() - fontSize) / 2;
  // Color textColor = SKYBLUE;
  Color textColor = DARKBLUE;

  while (!WindowShouldClose()) {
    // update values for the slider in the loop
    update_opacity_value(&my_slider);
    update_slider_value(&my_slider, callback);
    update_slider_position(&my_slider);
    update_rect_on_out_of_bounds(&my_slider);

    // draw the slider and other stuff
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(slider_component_header, (GetScreenWidth() / 2) - (text_width / 2),
             center_height - 180, fontSize, textColor);

    draw_slider(&my_slider);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
