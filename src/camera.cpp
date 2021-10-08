#include <iostream>
#include <gl/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>

#include "camera.h"

#define ANGLE_LITTLE_AMOUNT 0.01F

camera::camera(glm::vec3 eye, float hangle, float vangle, float sensitivity, float speed)
    : eye(eye), hangle(hangle), vangle(vangle), sensitivity(sensitivity), speed(speed) {}

auto camera::view_mat() const -> glm::mat4 {
  return glm::rotate(
      glm::rotate(glm::mat4(1.0F), hangle, glm::vec3(0.0F, 1.0F, 0.0F)), vangle,
      glm::vec3(1.0F, 0.0F, 0.0F));
}


auto camera::update_hangle(float angle, double dt) -> void {
  hangle += angle * sensitivity * (float) dt;
  std::cerr << "[i] Updated hangle: " << hangle << std::endl;
  
  if (hangle > glm::pi<float>()) {
    hangle = -glm::pi<float>() + ANGLE_LITTLE_AMOUNT;
  } else if (hangle < -glm::pi<float>()) {
    hangle = glm::pi<float>() - ANGLE_LITTLE_AMOUNT;
  }
}

auto camera::update_vangle(float angle, double dt) -> void {
  vangle += angle * sensitivity * (float) dt;
  std::cerr << "[i] Updated vangle: " << vangle << std::endl;

  if (vangle > glm::half_pi<float>()) {
    vangle = glm::half_pi<float>() - ANGLE_LITTLE_AMOUNT;
  } else if (vangle < -glm::half_pi<float>()) {
    vangle = -glm::half_pi<float>() + ANGLE_LITTLE_AMOUNT; 
  }
}

projection::projection(float aspect_ratio, float fovy, float near_plane)
    : aspect_ratio(aspect_ratio), fovy(fovy), near_plane(near_plane) {}

auto projection::proj_mat() const -> glm::mat4 {
  return glm::infinitePerspective(fovy, aspect_ratio, near_plane);
}

auto projection::resize(int new_width, int new_height) -> void {
  glViewport(0, 0, new_width, new_height);
  aspect_ratio = (float)new_width / (float)new_height;
}
