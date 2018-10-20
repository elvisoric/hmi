#ifndef NRG_LOADER_H
#define NRG_LOADER_H
#include <glad/glad.h>
#include <vector>
#include "RawModel.h"

namespace nrg {
class Loader {
 public:
  RawModel loadVAO(std::vector<float> &vertices,
                   std::vector<float> &textureCoords,
                   std::vector<float> &normals,
                   std::vector<unsigned int> &indices);

  RawModel loadVAO(std::vector<float> &vertices);
  virtual ~Loader();
  void cleanUp();
  unsigned int loadTexture(const std::string &path);

 private:
  std::vector<unsigned int> vaos_;
  std::vector<unsigned int> vbos_;
  std::vector<unsigned int> textures_;
  inline void unbindVAO();
  unsigned int createVAO();
  void storeDataInAttributesList(int attributeNumber,
                                 unsigned int coordinateSize,
                                 std::vector<float> &data);
  void bindIndicesBuffer(std::vector<unsigned int> &indices);
};
}  // namespace nrg

#endif  // NRG_LOADER_H