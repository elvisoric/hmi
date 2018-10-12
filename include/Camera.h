#ifndef NRG_CAMERA_H
#define NRG_CAMERA_H

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nrg {
class Camera {
 public:
  Camera()
      : position_{0.0f, 0.0f, 3.0f},
        front_{0.0f, 0.0f, -1.0f},
        up_{0.0f, 1.0f, 0.0f} {
    auto f = [this](double x, double y) {
      if (firstMove) {
        lastX_ = x;
        lastY_ = y;
        firstMove = false;
      }
      float xoffset = x - lastX_;
      float yoffset = lastY_ - y;  // y range from bottom to top
      lastX_ = x;
      lastY_ = y;
      float sensitivity = 0.05f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;
      yaw_ += xoffset;
      pitch_ += yoffset;
      if (pitch() > 89.0f) pitch_ = 89.0f;
      if (pitch() < -89.0f) pitch_ = -89.0f;
      updateFront();
    };
    MouseInputSubject::instance().subscribe(f);
  }
  void move(GLFWwindow* window) {
    // TODO: delta time between frames and than multiply with speed, to have
    // constant movement on different platforms
    float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position_ += cameraSpeed * front_;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position_ -= cameraSpeed * front_;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position_ -= glm::normalize(glm::cross(front_, up_)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position_ += glm::normalize(glm::cross(front_, up_)) * cameraSpeed;
  }
  inline const glm::vec3& position() const { return position_; }

  inline float pitch() const { return pitch_; }
  inline float yaw() const { return yaw_; }
  inline float roll() const { return roll_; }
  glm::mat4 view() const {
    auto view = glm::lookAt(position_, position_ + front_, up_);
    return view;
  }

 private:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  float pitch_;
  float yaw_;
  float roll_;
  float lastX_{1024.0f / 2.0f}, lastY_{768.0f / 2.0f};
  bool firstMove{true};
  void updateFront() {
    glm::vec3 f;
    f.x = cos(glm::radians(pitch())) * cos(glm::radians(yaw()));
    f.y = sin(glm::radians(pitch()));
    f.z = cos(glm::radians(pitch())) * sin(glm::radians(yaw()));
    front_ = glm::normalize(f);
  }
};
}  // namespace nrg

#endif  // NRG_CAMERA_H