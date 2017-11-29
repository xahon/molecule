#pragma once

#include "point.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class Vector2 { // Simple rectangle grid
public:
  Vector2(int width, int height) : width(width), height(height) {
    for (int i = 0; i < width * height; i++) {
      auto cell = std::make_shared<bool>(false);
      buffer.emplace_back(cell);
    }
  };
  Vector2(Vector2 &&) = default;
  Vector2(const Vector2 &) = default;
  ~Vector2(){};

  void set(const Point &point, bool val) { set(point.x(), point.y(), val); }

  void set(const int x, const int y, bool val) {
    buffer.assign(get_index_of(x, y), std::make_shared<bool>(val));
  }

  bool get(int x, int y) const { return *(buffer.at(get_index_of(x, y))); }

  bool get(const Point &point) const {
    return *(buffer.at(get_index_of(point)));
  }

private:
  int width;
  int height;
  std::vector<std::shared_ptr<bool>> buffer;

  int get_index_of(const Point &point) const {
    return get_index_of(point.x(), point.y());
  }

  int get_index_of(const int x, const int y) const { return x + y * height; }
};

class Vector3 // 3D vector space
{
public:
  Vector3(int width, int height, int depth)
      : width(width), height(height), depth(depth) {
    for (int z = 0; z < depth; z++) {
      auto layer = std::make_shared<Vector2>(width, height);
      buffer.emplace_back(layer);
    }
  };

  Vector3(Vector3 &&) = default;
  Vector3(const Vector3 &) = default;
  ~Vector3(){};

  void set(const Point &point, bool val) {
    set(point.x(), point.y(), point.z(), val);
  }

  void set(const int x, const int y, const int z, bool val) {
    auto layer = buffer.at(z);
    layer->set(x, y, val);
  }

  bool get(const int x, const int y, const int z) const {
    return buffer.at(z)->get(x, y);
  }

  bool get(const Point &point) const {
    return get(point.x(), point.y(), point.z());
  }

private:
  int width;
  int height;
  int depth;
  std::vector<std::shared_ptr<Vector2>> buffer;
};
