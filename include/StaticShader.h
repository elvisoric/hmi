#ifndef NRG_STATIC_SHADER_H
#define NRG_STATIC_SHADER_H

#include <Camera.h>
#include <Light.h>
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

  void loadLight(const Light& light) const {
    loadUniform(locationLightPosition_, light.position());
    loadUniform(locationLightColor_, light.color());
  }

  void getAllUniformLocations() override {
    locationTransformation_ = getUniformLocation("transformationMatrix");
    locationProjection_ = getUniformLocation("projection");
    locationView_ = getUniformLocation("view");
    locationLightPosition_ = getUniformLocation("lightPosition");
    locationLightColor_ = getUniformLocation("lightColor");
  }

 protected:
  void bindAttributes() override {
    bindAttribute(0, "position");
    bindAttribute(1, "aTexCoords");
    bindAttribute(2, "normal");
    linkProgram();
    getAllUniformLocations();
  }

 private:
  unsigned int locationTransformation_;
  unsigned int locationProjection_;
  unsigned int locationView_;
  unsigned int locationLightPosition_;
  unsigned int locationLightColor_;
};
}  // namespace nrg
#endif  // NRG_STATIC_SHADER_H