#include <GL/glew.h>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#define INFO_LOG_LENGTH 512

shader::shader(const std::string &path, GLenum type) : type(type), id(glCreateShader(type)) {

  std::ifstream shader_is{path};

  std::stringstream shader_ss;

  shader_ss << shader_is.rdbuf();

  source = shader_ss.str();
}

auto shader::compile() const -> void {
  const char* c_source = source.c_str();
  glShaderSource(id, 1, &c_source, NULL);
  glCompileShader(id);
}

auto shader::print_source() const -> void {
  std::cout << source << std::endl;
}

auto shader::get_id() const -> GLuint {
  return id;
}

shader_program::shader_program() : id(glCreateProgram()) {}

auto shader_program::attach(const shader &s) const -> void {
  glAttachShader(id, s.get_id());
}

auto shader_program::get_id() const -> GLuint {
  return id;
}

auto shader_program::get_uniform_location(const std::string &name) -> GLint {
  GLint loc = 0;

  if (uniforms_locations.find(name) != uniforms_locations.end()) {
    // there is already an entry in the map
    loc = uniforms_locations[name];
  } else {
    // we need to add the entry
    const auto *name_c_str = name.c_str();
    loc = glGetUniformLocation(id, name_c_str);

    uniforms_locations.emplace(name, loc);
  }

  return loc;
}

auto shader_program::link() const -> void {
  glLinkProgram(id);

  GLint status = GL_TRUE;
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint max_length = 0;
    std::array<GLchar, INFO_LOG_LENGTH> arr{};
    glGetProgramInfoLog(id, INFO_LOG_LENGTH, &max_length,
                        (GLchar *)arr.data());
    std::cerr << "[-] Shader Info Log: " << (char *)arr.data() << std::endl;
  }
}

auto shader_program::uniform(const std::string &name, const glm::vec3 &value) -> void {
  glProgramUniform1fv(id, get_uniform_location(name), 3, glm::value_ptr(value));
}

auto shader_program::uniform(const std::string &name, const glm::mat4 &value) -> void{
  glProgramUniformMatrix4fv(id, get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
}
