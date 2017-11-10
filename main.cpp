#include "src/molecule.h"
#include "src/scene.h"
#include "src/shape.h"
#include <csignal>
#include <iostream>
#include <memory>

void signal_handler(int signum) {
  std::cout << "\e[?25h\n";

  exit(signum);
}

int main() {
  signal(SIGINT, signal_handler);

  auto scene = std::make_unique<Scene>(70, 70, 70, ' ');
  // auto sphere = std::make_shared<Sphere>('1', Point(20, 20, 10), 10);

  // sphere->push_child(std::make_shared<Sphere>(
  //     '0', Point(sphere->get_radius() + 4, 0, 0), 4, *sphere));
  // sphere->push_child(std::make_shared<Sphere>(
  //     '0', Point(-sphere->get_radius() - 4, 0, 0), 4, *sphere));

  // scene->push_child(sphere);

  auto molecule = create_molecule(Point(35, 35, 35), 10, 4);
  scene->push_child(molecule);

  scene->render("first_scene", false);

  return 0;
}
