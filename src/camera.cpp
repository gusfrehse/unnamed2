#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "camera.h"

#define ANGLE_LITTLE_AMOUNT 0.01F

auto controller::up() -> void { up_amount = 1.0F; }

auto controller::down() -> void { down_amount = 1.0F; }

auto controller::left() -> void { left_amount = 1.0F; }

auto controller::right() -> void { right_amount = 1.0F; }

auto controller::forward() -> void { forward_amount = 1.0F; }

auto controller::backward() -> void { backward_amount = 1.0F; }

auto controller::vangle(float amount) -> void { vangle_amount = amount; }

auto controller::hangle(float amount) -> void { hangle_amount = amount; }

auto controller::reset() -> void {
  up_amount = 0.0F;
  down_amount = 0.0F;
  left_amount = 0.0F;
  right_amount = 0.0F;
  forward_amount = 0.0F;
  backward_amount = 0.0F;
  vangle_amount = 0.0F;
  hangle_amount = 0.0F;
}

camera::camera(glm::vec3 eye, float hangle, float vangle, float sensitivity,
               float speed)
    : eye(eye), dir(glm::vec3(0.0F, 0.0F, -1.0F)),
      right(glm::vec3(1.0F, 0.0F, 0.0F)), up(glm::vec3(1.0F, 0.0F, 0.0F)),
      hangle(hangle), vangle(vangle), sensitivity(sensitivity), speed(speed) {}

auto camera::view_mat() const -> glm::mat4 {
  glm::vec3 world_up(0.0F, 1.0F, 0.0F);
  glm::mat4 camera(1.0F);
  camera = glm::rotate(camera, vangle, right);
  camera = glm::rotate(camera, hangle, world_up);
  camera = glm::translate(camera, eye);

  return inverse(camera);
}

auto camera::update(double dt) -> void {
  std::cout << "[i] eye: " << eye.x << " " << eye.y << " " << eye.z
    << " dir: "   << dir.x   << " " << dir.y << " " << dir.z << " -> " << glm::length(dir)
    << " right: " << right.x << " " << right.y << " " << right.z<< " -> " << glm::length(right)
    << " up: "   << up.x    << " " << up.y << " " << up.z<< " -> " << glm::length(up)
    << std::endl;
  // rotation --
  dir += right * control.hangle_amount * sensitivity * (float)dt;
  dir += up * control.vangle_amount * sensitivity * (float)dt;
  dir = glm::normalize(dir);

  right = glm::cross(dir, glm::vec3(0.0F, 1.0F, 0.0F));
  right = glm::normalize(right);

  up = glm::cross(right, dir);

  hangle += control.hangle_amount * sensitivity * (float)dt;

  if (hangle > glm::pi<float>()) {
    hangle = -glm::pi<float>() + ANGLE_LITTLE_AMOUNT;
  } else if (hangle < -glm::pi<float>()) {
    hangle = glm::pi<float>() - ANGLE_LITTLE_AMOUNT;
  }

  vangle += control.vangle_amount * sensitivity * (float)dt;

  if (vangle > glm::half_pi<float>()) {
    vangle = glm::half_pi<float>() - ANGLE_LITTLE_AMOUNT;
  } else if (vangle < -glm::half_pi<float>()) {
    vangle = -glm::half_pi<float>() + ANGLE_LITTLE_AMOUNT;
  }

  // translation --
  auto front = glm::vec3(control.right_amount, control.up_amount,
                         control.forward_amount);
  auto back = glm::vec3(control.left_amount, control.down_amount,
                        control.backward_amount);

  // a camera forwards means it's z coord decreases
  glm::vec3 displacement(control.right_amount - control.left_amount,
                         control.up_amount - control.down_amount,
                         control.backward_amount - control.forward_amount);

  eye += float(dt) * displacement * speed;

  control.reset();
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
