#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <gl/glew.h>

#include "state.h"

#define COLOR_COMPONENT_SIZE 8
#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 5
#define INFO_LOG_LENGTH 512

const std::array<float, 9> VERTICES = {
    0.0F, 0.5F, 0.0F, -0.5F, -0.5F, 0.0F, 0.5F, -0.5F, 0.0F,
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
                                                                  : "[i]")
                  << " OpenGL says \"" << message << "\"" << std::endl;
      },
      0);

  glClearColor(1.0F, 0.0F, 0.0F, 1.0F);
  glViewport(0, 0, width, height);

  std::stringstream vert_shader_ss;
  {
    std::ifstream vert_shader_is("src/vert.glsl");
    vert_shader_ss << vert_shader_is.rdbuf();
  }

  std::stringstream frag_shader_ss;
  {
    std::ifstream frag_shader_is("src/frag.glsl");
    frag_shader_ss << frag_shader_is.rdbuf();
  }

  std::cout << "[i] Vertex shader:\n" << vert_shader_ss.str() << std::endl;
  std::cout << "[i] Fragment shader:\n" << frag_shader_ss.str() << std::endl;

  auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
  auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

  {
    auto source = vert_shader_ss.str();
    const auto *source_c_str = source.c_str();
    glShaderSource(vert_shader, 1, &source_c_str, 0);
  }

  {
    auto source = frag_shader_ss.str();
    const auto *source_c_str = source.c_str();
    glShaderSource(frag_shader, 1, &source_c_str, 0);
  }

  glCompileShader(vert_shader);
  glCompileShader(frag_shader);

  auto shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);
  glLinkProgram(shader_program);

  GLint status = GL_TRUE;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint max_length = 0;
    std::array<GLchar, INFO_LOG_LENGTH> arr{};
    glGetProgramInfoLog(shader_program, INFO_LOG_LENGTH, &max_length,
                        (GLchar *)arr.data());
    std::cerr << "[-] Shader Info Log: " << (char *)arr.data() << std::endl;
  }

  GLuint buffer{};
  glCreateBuffers(1, &buffer);
  glNamedBufferData(buffer, VERTICES.size() * sizeof(float), VERTICES.data(),
                    GL_STATIC_DRAW);

  GLuint vertex_array{};
  glCreateVertexArrays(1, &vertex_array);

  glEnableVertexArrayAttrib(vertex_array, 0);

  const GLuint buffer_binding_index = 0;
  glVertexArrayVertexBuffer(vertex_array, buffer_binding_index, buffer, 0,
                            3 * sizeof(GLfloat));

  glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(vertex_array, 0, buffer_binding_index);

  glUseProgram(shader_program);
  glBindVertexArray(vertex_array);
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
  glDrawArrays(GL_TRIANGLES, 0, 3);
  SDL_GL_SwapWindow(window);
}

auto state::should_quit() const -> bool { return exit; }
