#include <gl/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "camera.h"

camera::camera(glm::vec3 eye, float hangle, float vangle)
    : eye(eye), hangle(hangle), vangle(vangle) {}

auto camera::view_mat() const -> glm::mat4 {
  return glm::rotate(
      glm::rotate(glm::mat4(1.0F), hangle, glm::vec3(0.0F, 1.0F, 0.0F)), vangle,
      glm::vec3(1.0F, 0.0F, 0.0F));
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
