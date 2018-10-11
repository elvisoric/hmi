#include <Display.h>
#include <Entity.h>
#include <Maths.h>
#include <Renderer.h>
#include <StaticShader.h>
#include <TexturedModel.h>
#include <glad/glad.h>

namespace nrg {
Renderer::Renderer(const Display& display) {
  projection_ = glm::perspective(
      fieldOfView_, display.width() / display.height(), nearPlane_, farPlane_);
  auto f = [this](int width, int height) {
    projection_ = glm::perspective(fieldOfView_, (float)width / (float)height,
                                   nearPlane_, farPlane_);
  };
  FramebufferChangeSubject::instance().subscribe(f);
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
  glDrawElements(GL_TRIANGLES, rawModel.vertexCount(), GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
}  // namespace nrg