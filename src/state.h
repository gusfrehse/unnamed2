#ifndef STATE_H
#define STATE_H

#include <SDL.h>

class state {
  bool exit = false;
  SDL_Window *window;
  SDL_GLContext context;
  int width;
  int height;

public:
  state(int width, int height);

  auto should_quit() const -> bool;
  auto input() -> void;
  auto update(double dt) -> void;
  auto render() -> void;
};

#endif
