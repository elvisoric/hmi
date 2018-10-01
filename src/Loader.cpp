#include <Loader.h>

namespace nrg {

Loader::~Loader() { cleanUp(); }

RawModel Loader::loadVAO(std::vector<float> &vertices,
                         std::vector<unsigned int> &indices) {
  auto vao = createVAO();
  storeDataInAttributesList(0, vertices);
  bindIndicesBuffer(indices);
  unbindVAO();
  return RawModel{vao, indices.size()};
}

void Loader::cleanUp() {
  for (auto &vao : vaos_) glDeleteVertexArrays(1, &vao);
  for (auto &vbo : vbos_) glDeleteBuffers(1, &vbo);
}
void Loader::unbindVAO() { glBindVertexArray(0); }
unsigned int Loader::createVAO() {
  unsigned int vao;
  vaos_.push_back(vao);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  return vao;
}

void Loader::storeDataInAttributesList(int attributeNumber,
                                       std::vector<float> &data) {
  unsigned int vbo;
  vbos_.push_back(vbo);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE,
                        3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

void Loader::bindIndicesBuffer(std::vector<unsigned int> &indices) {
  unsigned int ebo;
  vbos_.push_back(ebo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               indices.data(), GL_STATIC_DRAW);
}
}  // namespace nrg