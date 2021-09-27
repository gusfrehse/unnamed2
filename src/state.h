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

  bool should_quit();
  void input();
  void update(double dt);
  void render();
};

#endif
