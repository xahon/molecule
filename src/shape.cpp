#include "shape.h"

PointInfo Shape::get_at(const int x, const int y, const int z) {
  auto ch = generation_algorithm(x, y, z) ? PointInfo(true, sign)
                                          : PointInfo(false, ' ');

  for (auto child = children.begin(); child != children.end(); child++) {
    auto child_point = (*child)->get_at(x, y, z);
    if (child_point.has) {
      ch = child_point;
    }
  }

  return ch;
}
