#include "src/scene.h"
#include "src/shape.h"
#include <iostream>
#include <memory>

int main() {
  auto scene = std::make_unique<Scene>(41, 41, 41, ' ');
  auto sphere = std::make_shared<Sphere>('1', Point(20, 20, 20), 20);

  scene->push_child(sphere);

  scene->render("first_scene", false);

  return 0;
}
