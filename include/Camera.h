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
      : position_{0.0f, 0.0f, 5.0f},
        front_{0.0f, 0.0f, -1.0f},
        up_{0.0f, 1.0f, 0.0f} {}

  inline const glm::vec3& position() const { return position_; }

  inline float pitch() const { return pitch_; }
  inline float yaw() const { return yaw_; }
  inline float roll() const { return roll_; }
  glm::mat4 view() const {
    auto center = position_ + front_;
    if (animate_) {
      center = glm::vec3(0.0f);
      return glm::lookAt(animatePosition_, center, up_);
    }
    return glm::lookAt(position_, center, up_);
  }
  void active(bool a) { active_ = a; }
  bool active() const { return active_; }
  void enableAnimation() { animate_ = true; }
  void disableAnimation() { animate_ = false; }

  virtual void move(GLFWwindow* window) = 0;
  void animate() {
    float radius = 20.0f;
    float x = sin(glfwGetTime()) * radius;
    float z = cos(glfwGetTime()) * radius;
    animatePosition_.x = x;
    animatePosition_.z = z;
  }
  virtual ~Camera() = default;

 protected:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 animatePosition_;
  float pitch_;
  float yaw_;
  float roll_;
  bool active_{false};
  float cameraSpeed_ = 0.05f;
  bool animate_{true};
};

class BasicCamera : public Camera {
 public:
  void move(GLFWwindow* window) override {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position_.z -= cameraSpeed_;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position_.z += cameraSpeed_;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position_.x += cameraSpeed_;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position_.x -= cameraSpeed_;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      position_.y -= cameraSpeed_;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      position_.y += cameraSpeed_;
  }

 private:
};
class FpsCamera : public Camera {
 public:
  FpsCamera() {
    auto f = [this](double x, double y) {
      if (active()) {
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
      }
    };
    MouseInputSubject::instance().subscribe(f);
  }
  void move(GLFWwindow* window) override {
    // TODO: delta time between frames and than multiply with speed, to have
    // constant movement on different platforms
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position_ += cameraSpeed_ * front_;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position_ -= cameraSpeed_ * front_;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position_ -= glm::normalize(glm::cross(front_, up_)) * cameraSpeed_;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position_ += glm::normalize(glm::cross(front_, up_)) * cameraSpeed_;
  }

 private:
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
class CameraHolder {
 public:
  CameraHolder(Camera& fps, Camera& basic)
      : fps_{fps}, basic_{basic}, current_{&fps_} {
    fps_.active(true);
    basic_.active(false);
  }
  Camera& camera() { return *current_; }
  void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && released_) {
      released_ = false;
      pressed_ = true;
      change();
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE && pressed_) {
      released_ = true;
      pressed_ = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
      camera().enableAnimation();
      animate_ = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
      camera().disableAnimation();
      animate_ = false;
    }

    if (animate_) {
      camera().animate();
    } else {
      camera().move(window);
    }
  }
  void change() {
    if (fps_.active()) {
      std::cout << "Activating Basic camera" << std::endl;
      current_ = &basic_;
      fps_.active(false);
      basic_.active(true);
    } else {
      std::cout << "Activating FPS camera" << std::endl;
      current_ = &fps_;
      fps_.active(true);
      basic_.active(false);
    }
  }

 private:
  Camera& fps_;
  Camera& basic_;
  Camera* current_;
  bool released_{true};
  bool pressed_{false};
  bool animate_{false};
};
}  // namespace nrg

#endif  // NRG_CAMERA_H