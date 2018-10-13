#ifndef NRG_ACTION_H
#define NRG_ACTION_H
#include "Entity.h"
namespace nrg {
class Action {
 public:
  Action() {}
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