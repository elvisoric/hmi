#ifndef NRG_TEXTURED_MODEL_H
#define NRG_TEXTURED_MODEL_H
#include "ModelTexture.h"
#include "RawModel.h"

namespace nrg {
class TexturedModel {
 public:
  TexturedModel(const RawModel& model, const ModelTexture& texture)
      : rawModel_{model}, texture_{texture} {}
  const RawModel& rawModel() const { return rawModel_; }
  const ModelTexture& texture() const { return texture_; }

 private:
  RawModel rawModel_;
  ModelTexture texture_;
};
}  // namespace nrg
#endif  // NRG_TEXTURED_MODEL_H