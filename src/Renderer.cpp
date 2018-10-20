#include <Display.h>
#include <Entity.h>
#include <Maths.h>
#include <Renderer.h>
#include <StaticShader.h>
#include <TexturedModel.h>
#include <glad/glad.h>

#include <BasicShader.h>

namespace nrg {
Renderer::Renderer(const Display& display) : display_{display} {
  projection_ = glm::perspective(glm::radians(fieldOfView_),
                                 display.width() / display.height(), nearPlane_,
                                 farPlane_);
  auto f = [this](int width, int height) {
    projection_ =
        glm::perspective(glm::radians(fieldOfView_),
                         (float)width / (float)height, nearPlane_, farPlane_);
  };
  FramebufferChangeSubject::instance().subscribe(f);

  auto fovUpdate = [this](double xoffset, double yoffset) {
    std::cout << "Field of View: " << fieldOfView_ << std::endl;
    if (fieldOfView_ >= 1.0f && fieldOfView_ <= 75.0f) {
      fieldOfView_ -= yoffset;
    }
    if (fieldOfView_ <= 1.0f) fieldOfView_ = 1.0f;
    if (fieldOfView_ >= 75.0f) fieldOfView_ = 75.0f;
    projection_ = glm::perspective(glm::radians(fieldOfView_),
                                   display_.width() / display_.height(),
                                   nearPlane_, farPlane_);
  };
  ScrollSubject::instance().subscribe(fovUpdate);
}
void Renderer::prepare() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Entity& entity, StaticShader& shader) const {
  auto& model = entity.model();
  auto& rawModel = model.rawModel();
  glBindVertexArray(rawModel.vaoID());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  auto transform =
      nrg::createTransformation(entity.position(), entity.rotX(), entity.rotY(),
                                entity.rotZ(), entity.scale());
  shader.loadTransformation(transform);
  shader.loadProjection(projection_);
  shader.loadShine(model.texture().shineDamper(),
                   model.texture().reflectivity());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, model.texture().id());
  shader.loadUseSpecularMap(model.texture().hasSpecularMap());
  if (model.texture().hasSpecularMap()) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, model.texture().specularMap());
  }
  glDrawElements(GL_TRIANGLES, rawModel.vertexCount(), GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

void Renderer::render(LineEntity& entity, BasicShader& shader) const {
  auto& rawModel = entity.model();
  glBindVertexArray(rawModel.vaoID());
  glEnableVertexAttribArray(0);
  auto transform =
      nrg::createTransformation(entity.position(), entity.rotX(), entity.rotY(),
                                entity.rotZ(), entity.scale());
  shader.loadTransformation(transform);
  shader.loadProjection(projection_);

  glDrawArrays(GL_LINES, 0, rawModel.vertexCount());
  glDisableVertexAttribArray(0);
}
}  // namespace nrg