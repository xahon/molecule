#pragma once

#include "arrays.h"
#include "logger.h"
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

  void render(std::string filename, bool write_to_stdout = true);
  void push_child(std::shared_ptr<Shape> child) {
    children.emplace_back(child);
  }

  void generate_chunk(const int from_x, const int from_y, const int from_z,
                      const int to_x, const int to_y, const int to_z,
                      std::shared_ptr<std::string> buffer,
                      std::shared_ptr<Log> logger);

private:
  const int width;
  const int height;
  const int depth;
  const char default_filler;

  std::shared_ptr<Vector3> buffer;
  std::vector<std::shared_ptr<Shape>> children;
};
