#ifndef NRG_RENDERER_H
#define NRG_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nrg {
class Entity;
class StaticShader;
class BasicShader;
class Display;
class Renderer {
 public:
  Renderer(const Display& diplay);
  void prepare();
  void render(Entity& entity, StaticShader& shader) const;
  void render(LineEntity& entity, BasicShader& shader) const;

 private:
  const Display& display_;
  glm::mat4 projection_;
  float fieldOfView_{45.0f};
  float nearPlane_{0.1f};
  float farPlane_{100.0f};
};
}  // namespace nrg
#endif  // NRG_RENDERER_H