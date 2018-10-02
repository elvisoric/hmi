#include <Renderer.h>
#include <TexturedModel.h>
#include <glad/glad.h>

namespace nrg {
void Renderer::prepare() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const TexturedModel& model) const {
  auto& rawModel = model.rawModel();
  glBindVertexArray(rawModel.vaoID());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, model.texture().id());
  glDrawElements(GL_TRIANGLES, rawModel.vertexCount(), GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
}  // namespace nrg