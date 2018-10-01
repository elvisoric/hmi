#ifndef NRG_STATIC_SHADER_H
#define NRG_STATIC_SHADER_H

#include "ShaderProgram.h"

namespace nrg {
class StaticShader : public ShaderProgram {
 public:
  StaticShader() : ShaderProgram("shaders/shader.vs", "shaders/shader.fs") {
    bindAttributes();
  }

 protected:
  void bindAttributes() override { bindAttribute(0, "position"); }
};
}  // namespace nrg
#endif  // NRG_STATIC_SHADER_H