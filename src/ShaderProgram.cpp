#include <ShaderProgram.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

namespace {
std::string typeAsString(int type) {
  switch (type) {
    case GL_VERTEX_SHADER:
      return "VERTEX";
    case GL_FRAGMENT_SHADER:
      return "FRAGMENT";
    default:
      return "UNKNOWN";
  }
}
unsigned int loadShader(const std::string& file, int type) {
  std::string code;
  std::ifstream shaderFile;
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shaderFile.open(file);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    code = shaderStream.str();
  } catch (std::ifstream::failure& f) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  const char* shaderCode = code.c_str();
  unsigned int shaderID;
  int success;
  char logInfo[512];

  shaderID = glCreateShader(type);
  glShaderSource(shaderID, 1, &shaderCode, NULL);
  glCompileShader(shaderID);
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, logInfo);
    std::cout << file << std::endl;
    std::cout << "ERROR::SHADER::" << typeAsString(type)
              << "::COMPILE_ERROR: " << logInfo << std::endl;
  }
  return shaderID;
}
}  // namespace

namespace nrg {

ShaderProgram::ShaderProgram(const std::string& vertexFile,
                             const std::string& fragmentFile) {
  vertexShaderID_ = loadShader(vertexFile, GL_VERTEX_SHADER);
  fragmentShaderID_ = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
  programID_ = glCreateProgram();
  glAttachShader(programID_, vertexShaderID_);
  glAttachShader(programID_, fragmentShaderID_);
}

ShaderProgram::~ShaderProgram() { cleanUp(); }

void ShaderProgram::cleanUp() const {
  stop();
  glDetachShader(programID_, vertexShaderID_);
  glDetachShader(programID_, fragmentShaderID_);
  glDeleteShader(vertexShaderID_);
  glDeleteShader(fragmentShaderID_);
  glDeleteProgram(programID_);
}
void ShaderProgram::linkProgram() const {
  glLinkProgram(programID_);
  glValidateProgram(programID_);
  int success;
  char logInfo[512];
  glGetProgramiv(programID_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programID_, 512, NULL, logInfo);
    std::cout << "ERROR::PROGRAM::LINK_ERROR " << logInfo << std::endl;
  }
}

void ShaderProgram::loadUniform(unsigned int location, bool value) const {
  float toLoad = 0.0f;
  if (value) toLoad = 1.0f;
  glUniform1f(location, value);
}

void ShaderProgram::loadUniform(unsigned int location, int value) const {
  glUniform1i(location, value);
}

void ShaderProgram::loadUniform(unsigned int location, float value) const {
  glUniform1f(location, value);
}

void ShaderProgram::loadUniform(unsigned int location,
                                const glm::vec3& value) const {
  glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::loadUniform(unsigned int location,
                                const glm::mat4& value) const {
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int ShaderProgram::getUniformLocation(
    const std::string& uniformName) const {
  return glGetUniformLocation(programID_, uniformName.c_str());
}

}  // namespace nrg