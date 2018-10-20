#ifndef NRG_GRID_H
#define NRG_GRID_H

#include <vector>

namespace nrg {
inline std::vector<float> makeGrid(unsigned int row, unsigned int column,
                                   float step) {
  std::vector<float> result;
  float start = -float(column) / 2 * step;
  float lineWidth = column * step + step;
  float leftX = start;
  float rightX = leftX + lineWidth;

  for (auto i = 0u; i < row; ++i) {
    auto z = i * -step - start;
    result.push_back(leftX);
    result.push_back(0.0f);
    result.push_back(z);

    result.push_back(rightX);
    result.push_back(0.0f);
    result.push_back(z);
  }

  start = -float(row) / 2 * step;
  lineWidth = row * step + step;
  float nearZ = start;
  float farZ = nearZ + lineWidth;

  for (auto i = 0u; i < column; ++i) {
    auto x = i * -step - start;
    result.push_back(x);
    result.push_back(0.0f);
    result.push_back(nearZ);

    result.push_back(x);
    result.push_back(0.0f);
    result.push_back(farZ);
  }

  return result;
}
}  // namespace nrg

#endif  // NRG_GRID_H