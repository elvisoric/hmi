#ifndef NRG_RENDERER_H
#define NRG_RENDERER_H

namespace nrg {
class TexturedModel;
class Renderer {
 public:
  void prepare();
  void render(const TexturedModel& model) const;
};
}  // namespace nrg
#endif  // NRG_RENDERER_H