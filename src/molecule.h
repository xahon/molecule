#include "logger.h"
#include "math.h"
#include "shape.h"

enum SphereConstraint { TOP, BOTTOM, LEFT, RIGHT, NONE };
const SphereConstraint BEGIN_CONSTRAINT = TOP;
const SphereConstraint END_CONSTRAINT = RIGHT;

std::string to_string(const SphereConstraint &cst) {
  switch (cst) {
  case TOP:
    return "TOP";
  case BOTTOM:
    return "BOTTOM";
  case LEFT:
    return "LEFT";
  case RIGHT:
    return "RIGHT";
  case NONE:
    return "NONE";
  }
  return "UNKNOWN";
}

SphereConstraint invert(const SphereConstraint &cst) {
  switch (cst) {
  case TOP:
    return BOTTOM;
  case BOTTOM:
    return TOP;
  case LEFT:
    return RIGHT;
  case RIGHT:
    return LEFT;
  default:
    return NONE;
  }
}

namespace {
std::shared_ptr<Log> logger = nullptr;
}

std::shared_ptr<Sphere> create_molecule(const std::string &logfile,
                                        const Point &center, int radius,
                                        int depth,
                                        SphereConstraint constraint = NONE) {
  if (logger == nullptr) {
    logger = std::make_shared<Log>(logfile);
  }

  char char_to_out = (char)(depth + 100);
  auto master_sphere = std::make_shared<Sphere>(char_to_out, center, radius);
  std::vector<std::shared_ptr<Shape>> children_to_link;
  std::vector<SphereConstraint> children_positions;

  if (depth != 0) {
    int child_radius = radius / 2;
    child_radius = child_radius == 0 ? 1 : child_radius;
    int offset = child_radius + radius;

    // Child precalculated points
    Point t(0, -offset /* + 1*/);
    Point b(0, offset /*- 1*/);
    Point l(-offset /*+ 1*/, 0);
    Point r(offset /*- 1*/, 0);
    // If no child, after returning, object will have correct coords

    for (auto cns = BEGIN_CONSTRAINT; cns <= END_CONSTRAINT;
         cns = SphereConstraint(cns + 1)) {
      if (cns != invert(constraint) && cns != NONE) {
        // Skip inverted align position and none (The deepest node)

        Point p =
            (cns == TOP) ? t : (cns == BOTTOM) ? b : (cns == LEFT) ? l : r;

        children_positions.push_back(cns);
        children_to_link.push_back(
            create_molecule(logfile, master_sphere->get_center() + p,
                            child_radius, depth - 1, cns));
      }
    }
  }

  std::string log_info;
  log_info += "Generating shape:\n";
  log_info += "ID: ";
  log_info += char_to_out;
  log_info += to_string(constraint);
  log_info += "\n";
  log_info += "Depth: ";
  log_info += std::to_string(depth);
  log_info += "\n";
  log_info += "Center:";
  log_info += center.to_string();
  log_info += "\n";
  log_info += "Radius: ";
  log_info += std::to_string(radius);
  log_info += "\n";
  log_info += "Character: ";
  log_info += char_to_out;
  log_info += " as int (";
  log_info += std::to_string(depth + 100);
  log_info += ")";
  log_info += "\n";
  log_info += "Will have ";
  log_info += [](const std::vector<SphereConstraint> &v) -> std::string {
    std::string acc = "";
    if (v.size() == 0) {
      return "NOTHING";
    }
    for (auto i = v.begin(); i != v.end(); ++i) {
      acc += to_string(*i) + " ";
    }
    return acc;
  }(children_positions);
  log_info += "\n\n\n";

  // logger.write(log_info);
  // logger.write log_info << std::endl;
  logger->writeFile(log_info, LoggerLevel::DEBUG);

  for (auto child = children_to_link.begin(); child != children_to_link.end();
       child++) {
    (*child)->relative_to(*master_sphere);
    master_sphere->push_child(*child);
  }

  if (constraint == NONE) {
    std::cout << "All the children have been added to their parents.\nMaster "
                 "sphere. Center: "
              << center << '\n';
  }

  return master_sphere;
}
