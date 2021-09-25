#include "state.h"
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

auto main(int /*unused*/, char** /*unused*/) -> int {
  state s{ WIDTH, HEIGHT };

  auto dt = 13.3f;

  while (!s.should_quit()) {
    s.input();
    s.update(dt);
    s.render();
  }

  return 0;
}
