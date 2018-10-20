#ifndef NRG_BASIC_SHADER_H
#define NRG_BASIC_SHADER_H

#include "ShaderProgram.h"

namespace nrg {
class BasicShader : public ShaderProgram {
 public:
  BasicShader() : ShaderProgram("shaders/basic.vs", "shaders/basic.fs") {
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
    linkProgram();
    getAllUniformLocations();
  }

 private:
  unsigned int locationTransformation_;
  unsigned int locationProjection_;
  unsigned int locationView_;
};
}  // namespace nrg

#endif  //  NRG_BASIC_SHADER_H