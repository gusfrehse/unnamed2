#ifndef CAMERA_H
#define CAMERA_H
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class camera {
  glm::vec3 eye;
  glm::vec3 dir;
  float hangle;
  float vangle;
  auto view_mat() const -> glm::mat4;
};

class projection {
  float aspect_ratio;
  float fovy;
  float near_plane;

  projection(float aspect_ratio, float fovy, float near_plane);

  auto proj_mat() const -> glm::mat4;
  auto resize(int width, int height) -> void;
};

#endif
