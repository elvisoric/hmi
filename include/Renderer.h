#ifndef NRG_RENDERER_H
#define NRG_RENDERER_H

namespace nrg {
class RawModel;
class Renderer {
 public:
  void prepare();
  void render(const RawModel& model) const;
};
}  // namespace nrg
#endif  // NRG_RENDERER_H