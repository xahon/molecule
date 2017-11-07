#pragma once

#include "arrays.h"
#include "point.h"
#include <math.h>
#include <memory>

typedef bool (*gen_func)(int x, int y, int z);

class Shape {
public:
  Shape(char sign, Point center) : sign(sign), center(center){};
  Shape(Shape &&) = default;
  Shape(const Shape &) = default;
  Shape &operator=(Shape &&) = default;
  Shape &operator=(const Shape &) = default;
  ~Shape(){};

  char get_sign() { return sign; }
  virtual bool generation_algorithm(const int x, const int y, const int z) {
    return false;
  }

protected:
  char sign;
  Point center;
};

class Sphere : public Shape {
public:
  Sphere(char sign, Point center, float radius)
      : Shape(sign, center), radius(radius){};

  ~Sphere(){};

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
