#pragma once

#include "arrays.h"
#include "shape.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class Scene {
public:
  Scene(int width, int height, int depth, char default_filler = ' ')
      : width(width), height(height), depth(depth),
        default_filler(default_filler),
        buffer(std::make_shared<Vector3>(width, height, depth)){};

  Scene(Scene &&) = default;
  Scene(const Scene &) = default;
  Scene &operator=(Scene &&) = default;
  Scene &operator=(const Scene &) = default;
  ~Scene(){};

  void render(std::string filename);
  void push_child(std::shared_ptr<Shape> child) {
    children.emplace_back(child);
  }

private:
  const int width;
  const int height;
  const int depth;
  const char default_filler;

  std::shared_ptr<Vector3> buffer;
  std::vector<std::shared_ptr<Shape>> children;
};
