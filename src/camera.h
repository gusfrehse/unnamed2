#ifndef CAMERA_H
#define CAMERA_H
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class controller { 
  public:
  float up_amount = 0.0F;       // NOLINT 
  float down_amount = 0.0F;     // NOLINT 
  float left_amount = 0.0F;     // NOLINT 
  float right_amount = 0.0F;    // NOLINT 
  float forward_amount = 0.0F;  // NOLINT 
  float backward_amount = 0.0F; // NOLINT 
  float vangle_amount = 0.0F;   // NOLINT 
  float hangle_amount = 0.0F;   // NOLINT 

  auto up() -> void;
  auto down() -> void;
  auto left() -> void;
  auto right() -> void;
  auto forward() -> void;
  auto backward() -> void;
  auto vangle(float amount) -> void;
  auto hangle(float amount) -> void;
  auto reset() -> void;
};

class camera {
  glm::vec3 eye;
  glm::vec3 dir;
  glm::vec3 right;
  glm::vec3 up;
  float hangle;
  float vangle;

  float sensitivity;
  float speed;

public:
  controller control; // NOLINT

  camera(glm::vec3 eye, float hangle, float vangle, float sensitivity,
         float speed);
  auto view_mat() const -> glm::mat4;
  auto update(double dt) -> void;
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
