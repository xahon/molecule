#include "scene.h"

namespace {
class Log {
public:
  static void init_file(std::string filename) {
    file.open(filename + ".vox", std::ios::trunc);
  }
  static void write(char sign_to_print) {
    std::cout << sign_to_print;
    file << sign_to_print;
  }

private:
  static std::ofstream file;
};

std::ofstream Log::file;
} // namespace

void Scene::render(std::string filename) {
  Log::init_file(filename);

  for (int z = 0; z < depth; z++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        Point cur_point(x, y, z);
        PointInfo cur(true, default_filler);
        buffer->set(cur_point, true); // set default scene char

        for (auto child = children.begin(); child != children.end(); child++) {
          auto child_point = (*child)->get_at(cur_point);
          cur = child_point.has ? child_point : cur;
        }

        Log::write(cur.sign);
      }
      Log::write('\n');
    }
    Log::write('\n');
  }
}
