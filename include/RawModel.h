#ifndef NRG_RAW_MODEL_H
#define NRG_RAW_MODEL_H

namespace nrg {
class RawModel {
 public:
  RawModel() = default;
  RawModel(unsigned int vaoID, unsigned long vertexCount)
      : vaoID_{vaoID}, vertexCount_{vertexCount} {}
  inline unsigned int vaoID() const { return vaoID_; }
  inline unsigned long vertexCount() const { return vertexCount_; }

 private:
  unsigned int vaoID_;
  unsigned long vertexCount_;
};
}  // namespace nrg

#endif  // NRG_RAW_MODEL_H