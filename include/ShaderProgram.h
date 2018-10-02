#ifndef NRG_SHADER_PROGRAM_H
#define NRG_SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>

namespace nrg {
class ShaderProgram {
 public:
  ShaderProgram(const std::string& vertexFile, const std::string& fragmentFile);
  virtual ~ShaderProgram();

  inline void start() const { glUseProgram(programID_); }
  inline void stop() const { glUseProgram(0); }
  void cleanUp() const;

 protected:
  virtual void bindAttributes() = 0;
  inline void bindAttribute(int attribute, const std::string& name) {
    glBindAttribLocation(programID_, attribute, name.c_str());
  }
  void linkProgram() const;

 private:
  unsigned int programID_;
  unsigned int vertexShaderID_;
  unsigned int fragmentShaderID_;
};
}  // namespace nrg

#endif  // NRG_SHADER_PROGRAM_H