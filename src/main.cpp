
#include <gl/glew.h>
#include <SDL.h>
#include <iostream>

#include "state.h"

#define WIDTH 1280
#define HEIGHT 720
#define SECONDS_TO_MILLIS 1000.0

auto main(int /*unused*/, char** /*unused*/) -> int {
  state s{ WIDTH, HEIGHT };

  auto time_last = SDL_GetPerformanceCounter();
  auto time_now = SDL_GetPerformanceCounter();

  while (!s.should_quit()) {
    time_last = time_now;
    time_now = SDL_GetPerformanceCounter();

    auto dt = (double) (time_now - time_last) * SECONDS_TO_MILLIS / (double) SDL_GetPerformanceFrequency(); // NOLINT 1000.0 are how many milliseconds in 

    s.input(dt);
    s.update(dt);
    s.render();
  }

  return 0;
}
