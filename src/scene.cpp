#include "scene.h"

namespace {
class Log {
public:
  static void init_file(std::string filename) {
    file.open(filename + ".vox", std::ios::trunc);
  }
  static void write(char sign_to_print, bool write_to_stdout = true) {
    if (write_to_stdout) {
      std::cout << sign_to_print;
    }

    file << sign_to_print;
  }

  static void step(int size = 1) {
    current_step += size;

    int next_percentage = (int)((float)current_step / count * 100);
    int current_percentage = (int)((float)old_step / count * 100);

    if (current_percentage != next_percentage) {
      std::cout << "\rProgress: " << next_percentage << "%        "
                << std::flush;
      old_step = current_step;
    }
  }

  static void register_count(int count) {
    if (count < 0) {
      throw "Count must be greater than 0";
    }

    Log::count = count;
  }

private:
  static std::ofstream file;
  static int count;
  static int current_step;
  static int old_step;
};

std::ofstream Log::file;
int Log::count = 0;
int Log::current_step = 0;
int Log::old_step = 0;
} // namespace

void Scene::render(std::string filename, bool write_to_stdout) {
  Log::init_file(filename);

  std::cout << "\e[?25l\n";
  if (!write_to_stdout) {
    Log::register_count(depth * height * width);
    Log::step(0);
  }

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

        if (!write_to_stdout) {
          Log::step();
        }
        Log::write(cur.sign, write_to_stdout);
      }
      Log::write('\n', write_to_stdout);
    }
    Log::write('\n', write_to_stdout);
  }
}
