#ifndef NRG_MODEL_TEXTURE_H
#define NRG_MODEL_TEXTURE_H

namespace nrg {
class ModelTexture {
 public:
  ModelTexture(unsigned int id) : textureID_{id} {}
  unsigned int id() const { return textureID_; }

 private:
  unsigned int textureID_;
};
}  // namespace nrg

#endif  // NRG_MODEL_TEXTURE_H