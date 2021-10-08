#ifndef CAMERA_H
#define CAMERA_H
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class camera {
  glm::vec3 eye;
  float hangle;
  float vangle;

  float sensitivity;
  float speed;
  
public:
  camera(glm::vec3 eye, float hangle, float vangle, float sensitivity, float speed);
  auto view_mat() const -> glm::mat4;
  auto update_hangle(float angle, double dt) -> void;
  auto update_vangle(float angle, double dt) -> void;
};

class projection {
  float aspect_ratio;
  float fovy;
  float near_plane;

public:
  projection(float aspect_ratio, float fovy, float near_plane);

  auto proj_mat() const -> glm::mat4;
  auto resize(int width, int height) -> void;
};

#endif
