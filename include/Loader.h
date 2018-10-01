#ifndef NRG_LOADER_H
#define NRG_LOADER_H
#include <glad/glad.h>
#include <vector>
#include "RawModel.h"

namespace nrg {
class Loader {
 public:
  RawModel loadVAO(std::vector<float> &vertices,
                   std::vector<unsigned int> &indices);
  virtual ~Loader();
  void cleanUp();

 private:
  std::vector<unsigned int> vaos_;
  std::vector<unsigned int> vbos_;
  inline void unbindVAO();
  unsigned int createVAO();
  void storeDataInAttributesList(int attributeNumber, std::vector<float> &data);
  void bindIndicesBuffer(std::vector<unsigned int> &indices);
};
}  // namespace nrg

#endif  // NRG_LOADER_H