#ifndef NRG_DISPLAY_H
#define NRG_DISPLAY_H
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace nrg {
class FramebufferChangeSubject {
 public:
  static FramebufferChangeSubject& instance() {
    static FramebufferChangeSubject instance = FramebufferChangeSubject{};
    return instance;
  }
  using SubscribeSignature = std::function<void(int, int)>;
  void subscribe(SubscribeSignature f) { observers_.push_back(f); }
  void notify(int width, int height) const {
    for (auto& f : observers_) f(width, height);
  }

 private:
  FramebufferChangeSubject() {}
  std::vector<SubscribeSignature> observers_;
};

class ScrollSubject {
 public:
  static ScrollSubject& instance() {
    static ScrollSubject instance = ScrollSubject{};
    return instance;
  }
  using SubscribeSignature = std::function<void(double, double)>;
  void subscribe(SubscribeSignature f) { observers_.push_back(f); }
  void notify(double xoffset, double yoffset) const {
    for (auto& f : observers_) f(xoffset, yoffset);
  }

 private:
  ScrollSubject() {}
  std::vector<SubscribeSignature> observers_;
};

class MouseInputSubject {
 public:
  static MouseInputSubject& instance() {
    static MouseInputSubject instance = MouseInputSubject{};
    return instance;
  }
  using SubscribeSignature = std::function<void(double, double)>;
  void subscribe(SubscribeSignature f) { observers_.push_back(f); }
  void notify(double x, double y) const {
    for (auto& f : observers_) f(x, y);
  }

 private:
  MouseInputSubject() {}
  std::vector<SubscribeSignature> observers_;
};

class Display {
 public:
  Display(GLFWwindow* window, float width, float height);
  virtual ~Display();

  inline void pollEvents() const { glfwPollEvents(); }

  inline void update() const { glfwSwapBuffers(window_); }
  inline bool shouldClose() const { return glfwWindowShouldClose(window_); }
  inline GLFWwindow* window() { return window_; }

  inline float width() const { return width_; }
  inline float height() const { return height_; }
  void processInput();

 private:
  GLFWwindow* window_;
  float width_;
  float height_;
};

Display createDisplay(float width, float height);
}  // namespace nrg

#endif  // NRG_DISPLAY_H