#include "src/scene.h"
#include "src/shape.h"
#include <iostream>
#include <memory>

int main() {
  auto scene = std::make_unique<Scene>(41, 41, 21, ' ');
  auto sphere = std::make_shared<Sphere>('1', Point(20, 20, 10), 10);

  sphere->push_child(std::make_shared<Sphere>(
      '0', Point(sphere->get_radius() + 4, 0, 0), 4, *sphere));
  sphere->push_child(std::make_shared<Sphere>(
      '0', Point(-sphere->get_radius() - 4, 0, 0), 4, *sphere));

  scene->push_child(sphere);

  scene->render("first_scene");

  return 0;
}
