#pragma once

#include "arrays.h"
#include "point.h"
#include <math.h>
#include <memory>

struct PointInfo {
  PointInfo(bool has, char sign) : has(has), sign(sign){};

  bool has;
  char sign;
};

class Shape {
public:
  Shape(char sign, Point center) : sign(sign), center(center){};
  Shape(char sign, Point center, Shape &parent) : sign(sign), center(center) {
    relative_to(parent);
  };
  Shape(Shape &&) = default;
  Shape(const Shape &) = default;
  Shape &operator=(Shape &&) = default;
  Shape &operator=(const Shape &) = default;
  ~Shape(){};

  char get_sign() { return sign; }
  virtual bool generation_algorithm(const int x, const int y, const int z) {
    return false;
  }

  virtual PointInfo get_at(const int x, const int y, const int z) final;
  virtual PointInfo get_at(const Point &point) final {
    return get_at(point.x(), point.y(), point.z());
  }

  virtual void relative_to(Shape &parent) {
    this->parent = std::make_shared<Shape>(parent);
    set_center(get_center() +
               parent.get_center()); // Convert relative coords to global
    aligned = true;
  }

  virtual void push_child(std::shared_ptr<Shape> child) {
    if (child->aligned) {
      children.emplace_back(child);
    } else {
      std::cerr << "Align child relative to other first\n";
    }
  }

  Point get_center() const { return center; }
  void set_center(const Point &point) { center = point; }

protected:
  char sign;
  Point center;
  std::shared_ptr<Shape> parent = nullptr;
  std::vector<std::shared_ptr<Shape>> children;
  bool aligned = false;
};

class Sphere : public Shape {
public:
  Sphere(char sign, Point center, float radius)
      : Shape(sign, center), radius(radius){};
  Sphere(char sign, Point center, float radius, Shape &parent)
      : Shape(sign, center, parent), radius(radius){};

  ~Sphere(){};

  float get_radius() const { return radius; }

protected:
  float radius;

  bool generation_algorithm(const int x, const int y, const int z) {
    const int xpow = (int)pow((double)x - center.x(), 2);
    const int ypow = (int)pow((double)y - center.y(), 2);
    const int zpow = (int)pow((double)z - center.z(), 2);
    const int rpow = (int)pow((double)radius, 2);

    if (xpow + ypow + zpow <= rpow) {
      return true;
    }

    return false;
  }
};
