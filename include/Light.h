#ifndef NRG_LIGHT_H
#define NRG_LIGHT_H

#include <glm/glm.hpp>

namespace nrg {
class Light {
 public:
  Light(const glm::vec3& position, const glm::vec3& color)
      : position_{position}, color_{color} {}

  inline void position(const glm::vec3 position) { position_ = position; }
  inline const glm::vec3& position() const { return position_; }
  inline void color(const glm::vec3 color) { position_ = color; }
  inline const glm::vec3& color() const { return color_; }

 private:
  glm::vec3 position_;
  glm::vec3 color_;
};
}  // namespace nrg
#endif  // NRG_LIGHT_H