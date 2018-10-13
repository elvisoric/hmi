#ifndef NRG_ACTION_H
#define NRG_ACTION_H
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
#include "Entity.h"

namespace nrg {
class ActionSubject {
 public:
  static ActionSubject& instance() {
    static ActionSubject instance = ActionSubject{};
    return instance;
  }
  using SubscribeSignature = std::function<void()>;
  void subscribe(SubscribeSignature f) { observers_.push_back(f); }
  void notify() const {
    for (auto& f : observers_) f();
  }

  void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && released_) {
      released_ = false;
      pressed_ = true;
      notify();
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && pressed_) {
      released_ = true;
      pressed_ = false;
    }
  }

 private:
  ActionSubject() {}
  std::vector<SubscribeSignature> observers_;
  bool pressed_{false}, released_{true};
};

class Action {
 public:
  Action() {
    auto f = [this] {
      if (enabled_)
        disable();
      else
        enable();
    };
    ActionSubject::instance().subscribe(f);
  }
  void enable() { enabled_ = true; }
  void disable() { enabled_ = false; }
  virtual void update(Entity& entity) = 0;
  void process(Entity& entity) {
    if (enabled_) update(entity);
  }
  virtual ~Action() = default;

 private:
  bool enabled_{true};
};
class ForwardBackAction : public Action {
 public:
  ForwardBackAction(float negative, float positive)
      : negativeAxis_{negative}, positiveAxis_{positive} {}
  void update(Entity& entity) override {
    if (entity.position().z > positiveAxis_) dv_ = -dv_;
    if (entity.position().z < negativeAxis_) dv_ = -dv_;
    entity.increasePosition(0.0f, 0.0f, dv_);
  }

 private:
  float negativeAxis_;
  float positiveAxis_;
  float dv_{0.2f};
};

class RotateAction : public Action {
 public:
  RotateAction(float dx, float dy, float dz) : dx_{dx}, dy_{dy}, dz_{dz} {}
  void update(Entity& entity) override {
    entity.increaseRotation(dx_, dy_, dz_);
  }

 private:
  float dx_, dy_, dz_;
};
}  // namespace nrg
#endif  // NRG_ACTION_H