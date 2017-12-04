#include "globals.h"
#include "handlers.h"
#include "src/logger.h"
#include "src/molecule.h"
#include "src/scene.h"
#include "src/shape.h"
#include "src/utils/cxxopts.h"
#include <csignal>
#include <iostream>
#include <memory>

___globals___ Globals;

struct molecule_info {
  Point center;
  int radius;
  int depth;
};

const char *DEFAULT_SCENE_NAME = "default_scene";
const char *DEFAULT_RADIUS = "18";
const char *DEFAULT_RECURSION_DEPTH = "4";
const char *DEFAULT_CHUNK_SIZE_PER_THREAD = "10";

int main(int argc, char **argv) {
  cxxopts::Options opts("Molecule");

  opts.add_options()("n,name", "Filename for render",
                     cxxopts::value<std::string>()
                         ->default_value(DEFAULT_SCENE_NAME)
                         ->implicit_value(DEFAULT_SCENE_NAME))

      ("r,radius", "Set molecule root node radius",
       cxxopts::value<int>()
           ->default_value(DEFAULT_RADIUS)
           ->implicit_value(DEFAULT_RADIUS))

          ("d,depth", "Set molecule recursion depth",
           cxxopts::value<int>()
               ->default_value(DEFAULT_RECURSION_DEPTH)
               ->implicit_value(DEFAULT_RECURSION_DEPTH))

              ("c,chunk-per-thread", "Set chunk size per one thread to render",
               cxxopts::value<unsigned>()
                   ->default_value(DEFAULT_CHUNK_SIZE_PER_THREAD)
                   ->implicit_value(DEFAULT_CHUNK_SIZE_PER_THREAD));

  auto result = opts.parse(argc, argv);

  const auto r_name = result["name"].as<std::string>();
  const auto r_radius = result["radius"].as<int>();
  const auto r_depth = result["depth"].as<int>();
  const auto r_threads = result["chunk-per-thread"].as<unsigned>();

  // ============ Print program info ===================
  std::cout << "=============================================" << '\n';
  std::cout << "Name: " << r_name << '\n';
  std::cout << "Root node radius: " << r_radius << '\n';
  std::cout << "Depth: " << r_depth << '\n';
  std::cout << "Chunk size per thread: " << r_threads << '\n';
  std::cout << "=============================================" << '\n';
  // ===================================================

  molecule_info mi;
  mi.depth = r_depth;
  mi.radius = r_radius;
  mi.center = [](const int rad, const int dep) -> Point {
    int radius = rad;
    int offset = radius;

    for (int i = 1; i <= dep; i++) {
      radius /= 2;
      offset += radius < 1 ? 1 : radius;
    }

    offset += 1; // Add padding
    offset *= 2;

    return Point(offset, offset, offset);
  }(mi.radius, mi.depth);

  Globals = {r_threads};

  signal(SIGINT, exit_handler);

  auto scene = std::make_unique<Scene>(mi.center.x() * 2, mi.center.y() * 2,
                                       mi.center.y() * 2, ' ');
  // auto sphere = std::make_shared<Sphere>('1', Point(20, 20, 10), 10);

  // sphere->push_child(std::make_shared<Sphere>(
  //     '0', Point(sphere->get_radius() + 4, 0, 0), 4, *sphere));
  // sphere->push_child(std::make_shared<Sphere>(
  //     '0', Point(-sphere->get_radius() - 4, 0, 0), 4, *sphere));

  // scene->push_child(sphere);

  Log logger(r_name);
  logger.clear_file();

  auto molecule = create_molecule(r_name, mi.center, mi.radius, mi.depth);
  scene->push_child(molecule);
  scene->render(r_name, false);

  exit_handler(0);
  return 0;
}
