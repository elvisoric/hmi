#ifndef NRG_STATIC_SHADER_H
#define NRG_STATIC_SHADER_H

#include <Camera.h>
#include "ShaderProgram.h"

namespace nrg {
class StaticShader : public ShaderProgram {
 public:
  StaticShader() : ShaderProgram("shaders/shader.vs", "shaders/shader.fs") {
    bindAttributes();
  }

  void loadTransformation(const glm::mat4& transformation) const {
    loadUniform(locationTransformation_, transformation);
  }

  void loadProjection(const glm::mat4& projection) const {
    loadUniform(locationProjection_, projection);
  }

  void loadView(const Camera& camera) const {
    loadUniform(locationView_, camera.view());
  }

  void getAllUniformLocations() override {
    locationTransformation_ = getUniformLocation("transformationMatrix");
    locationProjection_ = getUniformLocation("projection");
    locationView_ = getUniformLocation("view");
  }

 protected:
  void bindAttributes() override {
    bindAttribute(0, "position");
    bindAttribute(1, "aTexCoords");
    linkProgram();
    getAllUniformLocations();
  }

 private:
  unsigned int locationTransformation_;
  unsigned int locationProjection_;
  unsigned int locationView_;
};
}  // namespace nrg
#endif  // NRG_STATIC_SHADER_H