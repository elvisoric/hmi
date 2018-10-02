#ifndef NRG_STATIC_SHADER_H
#define NRG_STATIC_SHADER_H

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

  void getAllUniformLocations() override {
    locationTransformation_ = getUniformLocation("transformationMatrix");
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
};
}  // namespace nrg
#endif  // NRG_STATIC_SHADER_H