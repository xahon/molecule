#include "src/scene.h"
#include "src/shape.h"
#include <iostream>
#include <memory>

int main() {
  auto scene = std::make_unique<Scene>(40, 20, 20, ' ');
  auto small_sphere = std::make_shared<Sphere>('1', Point(34, 14, 13), 5);
  auto big_sphere = std::make_shared<Sphere>('2', Point(4, 4, 12), 7);
  auto centered_sphere = std::make_shared<Sphere>('3', Point(20, 10, 10), 4);
  auto colliding_sphere_example =
      std::make_shared<Sphere>('C', Point(6, 6, 4), 4);

  scene->push_child(big_sphere);
  scene->push_child(small_sphere);
  scene->push_child(centered_sphere);
  scene->push_child(colliding_sphere_example);

  scene->render("first_scene");
  scene->render("first_scene_without_frame", false);

  return 0;
}
