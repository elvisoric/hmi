#ifndef NRG_CAMERA_H
#define NRG_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nrg {
class Camera {
 public:
  void move(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position_.z -= 0.02f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position_.z += 0.02f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position_.x += 0.02f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position_.x -= 0.02f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) position_.y += 0.02f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) position_.y -= 0.02f;
  }
  inline const glm::vec3& position() const { return position_; }

  inline float pitch() const { return pitch_; }
  inline float yaw() const { return yaw_; }
  inline float roll() const { return roll_; }
  glm::mat4 view() const {
    glm::mat4 view{1.0f};
    view =
        glm::rotate(view, glm::radians(pitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(yaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 negativePosition{-position().x, -position().y, -position().z};
    view = glm::translate(view, negativePosition);
    return view;
  }

 private:
  glm::vec3 position_;
  float pitch_;
  float yaw_;
  float roll_;
};
}  // namespace nrg

#endif  // NRG_CAMERA_H