#include <Maths.h>

namespace nrg {
glm::mat4 createTransformation(const glm::vec3& translation, float rx, float ry,
                               float rz, float scale) {
  glm::mat4 transformation{1.0f};
  transformation = glm::translate(transformation, translation);
  transformation = glm::rotate(transformation, glm::radians(rx),
                               glm::vec3(1.0f, 0.0f, 0.0f));
  transformation = glm::rotate(transformation, glm::radians(ry),
                               glm::vec3(0.0f, 1.0f, 0.0f));
  transformation = glm::rotate(transformation, glm::radians(rz),
                               glm::vec3(0.0f, 0.0f, 1.0f));
  transformation = glm::scale(transformation, glm::vec3{scale});
  return transformation;
}

}  // namespace nrg