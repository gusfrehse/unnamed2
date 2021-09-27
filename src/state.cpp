#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

#include <gl/glew.h>


#include "state.h"

#define COLOR_COMPONENT_SIZE 8
#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 5

const std::array<float, 9> VERTICES = {
    0.0F, 0.5F, 0.0F, 0.0F, 0.5F, 0.0F, 0.0F, 0.5F, 0.0F,
};

state::state(int width, int height) : width(width), height(height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "[-] Could not initialize SDL: " << SDL_GetError()
              << std::endl;
    return;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, COLOR_COMPONENT_SIZE);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, COLOR_COMPONENT_SIZE);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, COLOR_COMPONENT_SIZE);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, COLOR_COMPONENT_SIZE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

  // Enable output callback
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(
      [](GLenum /*source*/, GLenum type, GLuint /*id*/, GLenum /*severety*/,
         GLsizei /*length*/, const char *message, const void * /*userParam*/) {
        std::cerr << (const char *)((type == GL_DEBUG_TYPE_ERROR) ? "[!]"
                                                                  : "[-]")
                  << " OpenGL Error: " << message << std::endl;
      },
      0);

  glClearColor(1.0F, 0.0F, 0.0F, 1.0F);
  glViewport(0, 0, width, height);

  std::ifstream vert_shader_is("src/vert.glsl");
  std::stringstream vert_shader_ss;
  vert_shader_ss << vert_shader_is.rdbuf();

  std::ifstream frag_shader_is("src/vert.glsl");
  std::stringstream frag_shader_ss;
  frag_shader_ss << frag_shader_is.rdbuf();

  std::cout << "[i] Vertex shader:\n" << vert_shader_ss.view() << std::endl;
  std::cout << "[i] Fragment shader:\n" << frag_shader_ss.view() << std::endl;
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

auto state::update(double dt) -> void {}

auto state::render() -> void {
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
}

auto state::should_quit() -> bool { return exit; }
