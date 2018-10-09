#ifndef NRG_ENTITY_H
#define NRG_ENTITY_H

#include <TexturedModel.h>
#include <glm/glm.hpp>

namespace nrg {
class Entity {
 public:
  Entity(const TexturedModel& model, const glm::vec3& position, float rotX,
         float rotY, float rotZ, float scale)
      : model_{model},
        position_{position},
        rotX_{rotX},
        rotY_{rotY},
        rotZ_{rotZ},
        scale_{scale} {}

  inline TexturedModel& model() { return model_; }
  inline const glm::vec3 position() const { return position_; }
  inline float rotX() const { return rotX_; }
  inline float rotY() const { return rotY_; }
  inline float rotZ() const { return rotZ_; }
  inline float scale() const { return scale_; }

  void increasePosition(float dx, float dy, float dz) {
    position_.x += dx;
    position_.y += dy;
    position_.z += dz;
  }

  void increaseRotation(float dx, float dy, float dz) {
    rotX_ += dx;
    rotY_ += dy;
    rotZ_ += dz;
  }

 private:
  TexturedModel model_;
  glm::vec3 position_;
  float rotX_, rotY_, rotZ_;
  float scale_;
};
}  // namespace nrg
#endif  // NRG_ENTITY_H