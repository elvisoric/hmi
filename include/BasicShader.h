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

  void color(const glm::vec3& color) {
    loadUniform(locationColor_, color);
    color_ = color;
  }

  inline glm::vec3 color() { return color_; }

  void getAllUniformLocations() override {
    locationTransformation_ = getUniformLocation("transformationMatrix");
    locationProjection_ = getUniformLocation("projection");
    locationView_ = getUniformLocation("view");
    locationColor_ = getUniformLocation("color");
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
  unsigned int locationColor_;
  glm::vec3 color_;
};
}  // namespace nrg

#endif  //  NRG_BASIC_SHADER_H