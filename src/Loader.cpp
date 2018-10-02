#include <Loader.h>
#include <stb_image.h>
#include <iostream>

namespace nrg {

Loader::~Loader() { cleanUp(); }

RawModel Loader::loadVAO(std::vector<float> &vertices,
                         std::vector<float> &textureCoords,
                         std::vector<unsigned int> &indices) {
  auto vao = createVAO();
  storeDataInAttributesList(0, 3, vertices);
  storeDataInAttributesList(1, 2, textureCoords);
  bindIndicesBuffer(indices);
  unbindVAO();
  return RawModel{vao, indices.size()};
}

void Loader::cleanUp() {
  for (auto &vao : vaos_) glDeleteVertexArrays(1, &vao);
  for (auto &vbo : vbos_) glDeleteBuffers(1, &vbo);
  for (auto &tex : textures_) glDeleteBuffers(1, &tex);
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
                                       unsigned int coordinateSize,
                                       std::vector<float> &data) {
  unsigned int vbo;
  vbos_.push_back(vbo);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE,
                        coordinateSize * sizeof(float), (void *)0);
  glEnableVertexAttribArray(attributeNumber);
}

void Loader::bindIndicesBuffer(std::vector<unsigned int> &indices) {
  unsigned int ebo;
  vbos_.push_back(ebo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               indices.data(), GL_STATIC_DRAW);
}

unsigned int Loader::loadTexture(const std::string &path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
  }
  stbi_image_free(data);
  textures_.push_back(textureID);

  return textureID;
}
}  // namespace nrg