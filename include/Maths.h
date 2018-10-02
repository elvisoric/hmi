#ifndef NRG_MATHS_H
#define NRG_MATHS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nrg {
glm::mat4 createTransformation(const glm::vec3& translation, float rx, float ry,
                               float rz, float scale);
}  // namespace nrg
#endif  // NRG_MATHS_H