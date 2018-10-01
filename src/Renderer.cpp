#include <RawModel.h>
#include <Renderer.h>
#include <glad/glad.h>

namespace nrg {
void Renderer::prepare() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const RawModel& model) const {
  glBindVertexArray(model.vaoID());
  glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, 0);
}
}  // namespace nrg