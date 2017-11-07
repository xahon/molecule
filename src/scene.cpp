#include "scene.h"

void Scene::render(std::string filename, bool with_frame) {
  std::ofstream file;
  file.open(filename + ".vox", std::ios::trunc);

  for (int z = 0; z < depth; z++) {

    auto separator = std::make_unique<std::string>(width + 2, '-');

    if (with_frame) {
      std::cout << *separator << '\n';
      file << *separator << '\n';
    }

    for (int y = 0; y < height; y++) {
      if (with_frame) {
        std::cout << "|";
        file << "|";
      }

      for (int x = 0; x < width; x++) {

        char cur = default_filler;
        buffer->set(x, y, z, true); // set default scene char

        for (auto child = children.begin(); child != children.end(); child++) {
          bool is_here_some_child = (*child)->generation_algorithm(x, y, z);

          if (is_here_some_child) {
            cur = (*child)->get_sign();
          }
        }

        std::cout << cur;
        file << cur;
      }

      if (with_frame) {
        std::cout << "|";
        file << "|";

        if (y == 0) {
          std::cout << " z-index: " << z;
          file << " z-index: " << z;
        }
      }

      std::cout << '\n';
      file << '\n';
    }

    if (with_frame) {
      std::cout << *separator;
      file << *separator;
    }

    std::cout << '\n';
    file << '\n';
  }
}
