#pragma once

#include "shape.h"

enum SphereConstraints { TOP, BOTTOM, LEFT, RIGHT, NONE };
const SphereConstraints BEGIN_CONSTRAINT = TOP;
const SphereConstraints END_CONSTRAINT = RIGHT;

std::shared_ptr<Sphere> create_molecule(Point center, int radius, int depth,
                                        SphereConstraints constraints = NONE,
                                        int sign = 1) {

  auto master_sphere = std::make_shared<Sphere>('0' + sign, center, radius);

  std::vector<std::shared_ptr<Shape>> children_to_link;

  if (depth != 0) {
    int child_radius = radius / 2;
    // Child calculated points
    Point t(0, -child_radius - radius + 1);
    Point b(0, child_radius + radius - 1);
    Point l(-child_radius - radius + 1, 0);
    Point r(child_radius + radius - 1, 0);
    // If no child, after returning, object will have correct coords

    for (auto cns = BEGIN_CONSTRAINT; cns <= END_CONSTRAINT;
         cns = SphereConstraints(cns + 1)) {
      if (cns != constraints) {
        std::cout << "Current cns " << cns << "" << '\n';
        std::cout << "Depth: " << depth << ". Radius: " << radius
                  << ". Sign: " << sign << '\n';
        Point p =
            (cns == TOP) ? t : (cns == BOTTOM) ? b : (cns == LEFT) ? l : r;
        children_to_link.push_back(
            create_molecule(p, child_radius, depth - 1, cns, sign + 1));
      }
    }
  }

  for (auto child = children_to_link.begin(); child != children_to_link.end();
       child++) {
    std::cout << "Linking " << children_to_link.size() << " children" << '\n';
    (*child)->relative_to(*master_sphere);
    master_sphere->push_child(*child);
  }

  std::cout << "Returning sphere" << '\n';

  if (constraints == NONE) {
    std::cout << "Master sphere: Center: " << center << '\n';
  }

  return master_sphere;
}
