#pragma once

#include "math.h"
#include "shape.h"

enum SphereConstraints { TOP, BOTTOM, LEFT, RIGHT, NONE };
const SphereConstraints BEGIN_CONSTRAINT = TOP;
const SphereConstraints END_CONSTRAINT = RIGHT;

std::ostream &operator<<(std::ostream &lhs, SphereConstraints e) {
  switch (e) {
  case TOP:
    lhs << "TOP";
    break;
  case BOTTOM:
    lhs << "BOTTOM";
    break;
  case LEFT:
    lhs << "LEFT";
    break;
  case RIGHT:
    lhs << "RIGHT";
    break;
  case NONE:
    lhs << "NONE";
    break;
  }
  return lhs;
}

std::string character_blacklist = "";

std::shared_ptr<Sphere> create_molecule(Point center, int radius, int depth,
                                        bool randomize_chars = true,
                                        SphereConstraints constraints = NONE,
                                        int sign = 1) {

  char char_to_out = '0' + sign;

  if (randomize_chars) {
    int loop_threshold = 1000;
    while (true) {
      --loop_threshold;
      const char random_char = '0' + (int)(rand() % 20);
      int found_the_same = character_blacklist.find(random_char);
      std::cout << character_blacklist << '\n';

      if (found_the_same == std::string::npos) {
        char_to_out = random_char;
        character_blacklist += random_char;
        break;
      }

      if (loop_threshold <= 0) {
        exit(1);
      }
    }
  }

  auto master_sphere =
      std::make_shared<Sphere>('0' + char_to_out, center, radius);

  std::vector<std::shared_ptr<Shape>> children_to_link;

  std::vector<SphereConstraints> children_positions;
  std::cout << "\nStarting " << char_to_out << constraints << " having depth "
            << depth << ", radius: " << radius << ", center: " << center
            << '\n';

  if (depth != 0) {
    int child_radius = radius / 2;
    child_radius = child_radius == 0 ? 1 : child_radius;
    // Child calculated points
    Point t(0, -child_radius - radius + 1);
    Point b(0, child_radius + radius - 1);
    Point l(-child_radius - radius + 1, 0);
    Point r(child_radius + radius - 1, 0);
    // If no child, after returning, object will have correct coords

    for (auto cns = BEGIN_CONSTRAINT; cns <= END_CONSTRAINT;
         cns = SphereConstraints(cns + 1)) {
      if (cns != constraints) {

        Point p =
            (cns == TOP) ? t : (cns == BOTTOM) ? b : (cns == LEFT) ? l : r;

        std::cout << "Point diff for next child: " << p << " (ORIGINAL) "
                  << center << '\n';

        children_positions.push_back(cns);

        children_to_link.push_back(
            create_molecule(master_sphere->get_center() + p, child_radius,
                            depth - 1, randomize_chars, cns, int(sign + 1)));
      }
    }
  }

  std::cout << "\nID: " << char_to_out << constraints << '\n';
  std::cout << "Shouldn't have " << constraints << '\n';
  std::cout << "Depth: " << depth << ". Radius: " << radius
            << ". Sign: " << char_to_out << '\n';
  std::cout << "Has " << children_to_link.size() << " children" << '\n';

  for (int i = 0; i < children_positions.size(); i++) {
    std::cout << "Children at " << children_positions[i] << '\n';
  }

  for (auto child = children_to_link.begin(); child != children_to_link.end();
       child++) {
    std::cout << "Linking child " << child - children_to_link.begin() << '\n';
    (*child)->relative_to(*master_sphere);
    master_sphere->push_child(*child);
  }

  std::cout << "Returning sphere" << '\n';

  if (constraints == NONE) {
    std::cout << "Master sphere: Center: " << center << '\n';
  }

  return master_sphere;
}
