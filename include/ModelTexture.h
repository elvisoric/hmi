#ifndef NRG_MODEL_TEXTURE_H
#define NRG_MODEL_TEXTURE_H

namespace nrg {
class ModelTexture {
 public:
  ModelTexture(unsigned int id) : textureID_{id} {}
  inline unsigned int id() const { return textureID_; }
  inline float shineDamper() const { return shineDamper_; }
  inline void shineDamper(float damper) { shineDamper_ = damper; }
  inline float reflectivity() const { return reflectivity_; }
  inline void reflectivity(float reflectivity) { reflectivity_ = reflectivity; }
  inline void specularMap(unsigned int specularMap) {
    specularMap_ = specularMap;
    hasSpecularMap_ = true;
  }
  inline unsigned int specularMap() const { return specularMap_; }
  inline bool hasSpecularMap() const { return hasSpecularMap_; }

 private:
  unsigned int textureID_;
  unsigned int specularMap_;
  bool hasSpecularMap_{false};
  float shineDamper_;
  float reflectivity_;
};
}  // namespace nrg

#endif  // NRG_MODEL_TEXTURE_H