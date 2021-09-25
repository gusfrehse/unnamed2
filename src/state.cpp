#include <gl/glew.h>
#include <iostream>

#include "state.h"

state::state(int width, int height) : width(width), height(height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "[-] Could not initialize SDL: " << SDL_GetError()
              << std::endl;
    return;
  }

  window =
      SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, SDL_WINDOW_OPENGL);

  context = SDL_GL_CreateContext(window);

  auto glew_err = glewInit();
  if (glew_err != GLEW_OK) {
    std::cerr << "[-] Could not initialize GLEW: "
              << glewGetErrorString(glew_err) << std::endl;
  }

  if (!GLEW_VERSION_4_5) {
    std::cerr << "[-] GLEW does not support OpenGL 4.5" << std::endl;
  }
}

auto state::input() -> void {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT:
      exit = true;
    }
  }
}

auto state::update(float dt) -> void {
  return;
}

auto state::render() -> void {
  return;
}

auto state::should_quit() -> bool {
  return exit;
}
