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

  void loadShine(float damper, float reflectivity) const {
    loadUniform(locationReflectivity_, reflectivity);
    loadUniform(locationShineDamper_, damper);
  }

  void useAmbient(bool use) {
    start();
    useAmbient_ = use;
    loadUniform(locationUseAmbientLight_, use);
    stop();
  }

  void useDiffuse(bool use) {
    start();
    useDiffuse_ = use;
    loadUniform(locationUseDiffuseLight_, use);
    stop();
  }

  void useSpecular(bool use) {
    start();
    useSpecular_ = use;
    loadUniform(locationUseSpecularLight_, use);
    stop();
  }

  void useLight(bool use) {
    start();
    useLight_ = use;
    loadUniform(locationUseLight_, use);
    stop();
  }

  void getAllUniformLocations() override {
    locationTransformation_ = getUniformLocation("transformationMatrix");
    locationProjection_ = getUniformLocation("projection");
    locationView_ = getUniformLocation("view");
    locationLightPosition_ = getUniformLocation("lightPosition");
    locationLightColor_ = getUniformLocation("lightColor");
    locationReflectivity_ = getUniformLocation("reflectivity");
    locationShineDamper_ = getUniformLocation("shineDamper");
    locationUseAmbientLight_ = getUniformLocation("useAmbientLight");
    locationUseDiffuseLight_ = getUniformLocation("useDiffuseLight");
    locationUseSpecularLight_ = getUniformLocation("useSpecularLight");
    locationUseLight_ = getUniformLocation("useLight");
  }
  void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && ambientReleased_) {
      ambientReleased_ = false;
      ambientPressed_ = true;
      useAmbient(!useAmbient_);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && ambientPressed_) {
      ambientReleased_ = true;
      ambientPressed_ = false;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && diffuseReleased_) {
      diffuseReleased_ = false;
      diffusePressed_ = true;
      useDiffuse(!useDiffuse_);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && diffusePressed_) {
      diffuseReleased_ = true;
      diffusePressed_ = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && specularReleased_) {
      specularReleased_ = false;
      specularPressed_ = true;
      useSpecular(!useSpecular_);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && specularPressed_) {
      specularReleased_ = true;
      specularPressed_ = false;
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && lightReleased_) {
      lightReleased_ = false;
      lightPressed_ = true;
      useLight(!useLight_);
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE && lightPressed_) {
      lightReleased_ = true;
      lightPressed_ = false;
    }
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
  unsigned int locationShineDamper_;
  unsigned int locationReflectivity_;

  unsigned int locationUseAmbientLight_;
  unsigned int locationUseDiffuseLight_;
  unsigned int locationUseSpecularLight_;
  unsigned int locationUseLight_;
  bool useAmbient_;
  bool useDiffuse_;
  bool useSpecular_;
  bool useLight_;

  bool ambientReleased_{true};
  bool diffuseReleased_{true};
  bool specularReleased_{true};
  bool lightReleased_{true};

  bool ambientPressed_{false};
  bool diffusePressed_{false};
  bool specularPressed_{false};
  bool lightPressed_{true};
};
}  // namespace nrg
#endif  // NRG_STATIC_SHADER_H