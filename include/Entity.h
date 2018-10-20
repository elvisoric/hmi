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
        rotation_{rotX, rotY, rotZ},
        scale_{scale} {}

  inline TexturedModel& model() { return model_; }
  inline glm::vec3& position() { return position_; }
  inline glm::vec3& rotation() { return rotation_; }
  inline float& scaling() { return scale_; }
  inline float rotX() const { return rotation_.x; }
  inline float rotY() const { return rotation_.y; }
  inline float rotZ() const { return rotation_.z; }
  inline float scale() const { return scale_; }

  void increasePosition(float dx, float dy, float dz) {
    position_.x += dx;
    position_.y += dy;
    position_.z += dz;
  }

  void increaseRotation(float dx, float dy, float dz) {
    rotation_.x += dx;
    rotation_.y += dy;
    rotation_.z += dz;
  }

 private:
  TexturedModel model_;
  glm::vec3 position_;
  glm::vec3 rotation_;
  float scale_;
};

class LineEntity {
 public:
  LineEntity(const RawModel& model, const glm::vec3& position, float rotX,
             float rotY, float rotZ, float scale)
      : model_{model},
        position_{position},
        rotation_{rotX, rotY, rotZ},
        scale_{scale} {}

  inline RawModel& model() { return model_; }
  inline glm::vec3& position() { return position_; }
  inline float rotX() const { return rotation_.x; }
  inline float rotY() const { return rotation_.y; }
  inline float rotZ() const { return rotation_.z; }
  inline float scale() const { return scale_; }

 private:
  RawModel model_;
  glm::vec3 position_;
  glm::vec3 rotation_;
  float scale_;
};
}  // namespace nrg
#endif  // NRG_ENTITY_H