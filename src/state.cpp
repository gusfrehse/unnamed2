#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <gl/glew.h>

#include "shader.h"
#include "state.h"

#define COLOR_COMPONENT_SIZE 8
#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 5
#define FOVY 90.0F
#define NEAR_PLANE 0.1F
#define CAM_SENSITIVITY 0.001F
#define CAM_SPEED 1.0F

// clang-format off
const std::array<float, 9> VERTICES = {
     0.0F,  0.5F, -3.0F,
    -0.5F, -0.5F, -3.0F,
     0.5F, -0.5F, -3.0F,
};
// clang-format on

state::state(int width, int height)
    : width(width), height(height),
      proj((float)width / (float)height, FOVY, NEAR_PLANE),
      cam(glm::vec3(0.0F, 0.0F, 0.0F), 0.0F, 0.0F, CAM_SENSITIVITY, CAM_SPEED) {
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

  SDL_SetRelativeMouseMode(SDL_TRUE);

  window = SDL_CreateWindow("unnamed2", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_OPENGL);

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

  shader vert_shader{"src/vert.glsl", GL_VERTEX_SHADER};
  shader frag_shader{"src/frag.glsl", GL_FRAGMENT_SHADER};

  vert_shader.compile();
  frag_shader.compile();

  program = std::make_unique<shader_program>();
  program->attach(vert_shader);
  program->attach(frag_shader);

  program->link();

  GLuint buffer{};
  glCreateBuffers(1, &buffer);
  glNamedBufferData(buffer, VERTICES.size() * sizeof(float), VERTICES.data(),
                    GL_STATIC_DRAW);

  vao = 0;
  glCreateVertexArrays(1, &vao);

  glEnableVertexArrayAttrib(vao, 0);

  const GLuint buffer_binding_index = 0;
  glVertexArrayVertexBuffer(vao, buffer_binding_index, buffer, 0,
                            3 * sizeof(GLfloat));

  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(vao, 0, buffer_binding_index);
}

auto state::input(double dt) -> void {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT:
      exit = true;
      break;
    case SDL_MOUSEMOTION:
      // TODO Remove following commented line:
      //std::cout << "[i] " << e.motion.xrel << " " << e.motion.yrel << std::endl;
      cam.update_hangle((float) e.motion.xrel, dt);
      //cam.update_vangle((float) e.motion.yrel, dt);
      break;
    case SDL_KEYDOWN:
      if (e.key.keysym.sym == SDLK_ESCAPE) {
        exit = true;
      }
    }
  }
}

auto state::update(double dt) -> void {}

auto state::render() -> void {
  glUseProgram(program->get_id());
  glBindVertexArray(vao);

  auto view_matrix = cam.view_mat();
  auto proj_matrix = proj.proj_mat();
  auto mvp = proj_matrix * view_matrix;

  program->uniform("mvp", mvp);

  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  SDL_GL_SwapWindow(window);
}

auto state::should_quit() const -> bool { return exit; }
