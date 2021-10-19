#ifndef STATE_H
#define STATE_H
#include <memory>

#include <GL/glew.h>
#include <SDL.h>

#include "shader.h"
#include "camera.h"

class state {
  bool exit = false;
  SDL_Window *window;
  SDL_GLContext context;
  int width;
  int height;

  std::unique_ptr<shader_program> program;
  GLuint vao;
  projection proj;
  camera cam;

public:
  state(int width, int height);

  auto should_quit() const -> bool;
  auto input() -> void;
  auto update(double dt) -> void;
  auto render() -> void;
};

#endif
