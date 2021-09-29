#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <gl/glew.h>
#include <glm/vec3.hpp>

class shader {
  std::string source;
  GLenum type;
  GLuint id;

public:
  shader(const std::string &path, GLenum type);
  auto compile() const -> void;
  auto get_id() const -> GLuint;
  auto print_source() const -> void;
};

class shader_program {
  GLuint id;
  std::unordered_map<std::string, GLint> uniforms_locations;

  auto get_uniform_location(const std::string &name) -> GLint;
public:
  shader_program();
  auto attach(const shader &s) const -> void;
  auto link() const -> void;
  auto get_id() const -> GLuint;

  auto uniform(const std::string &name, glm::vec3 value);
};
#endif
