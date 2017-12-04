#include "scene.h"
#include "../globals.h"
#include <chrono>
#include <ctime>
#include <math.h>
#include <sys/time.h>
#include <thread>

// #define Globals.CHUNK_PER_THREAD 10

std::mutex mtx;

void Scene::generate_chunk(const int from_x, const int from_y, const int from_z,
                           const int to_x, const int to_y, const int to_z,
                           std::shared_ptr<std::string> buf,
                           std::shared_ptr<Log> logger) {
  // std::cout << "Iterating thru " << from_z << " to " << to_z << " - " <<
  // depth << '\n';
  for (int z = from_z; z < to_z; z++) {
    for (int y = from_y; y < to_y; y++) {
      for (int x = from_x; x < to_x; x++) {
        PointInfo current_point_info(true, default_filler);
        // buffer->set(cur_point, true); // set default scene char

        for (unsigned i = children.size(); i-- > 0;) {
          auto child_point = children[i]->get_at(x, y, z);

          if (child_point.has) {
            // std::cout << "Has another " << child_point.sign << '\n';
            current_point_info = child_point;
            break;
          }
        }

        mtx.lock();
        (*logger).step();
        mtx.unlock();

        (*buf)[x + y * height + z * height * width] = current_point_info.sign;
        // render_printer->writeFile(std::string(1, current_point_info.sign));
      }
      // (*buf)[y * height + z * height * width] = '\n';
      // render_printer->writeFile("\n");
    }
    // (*buf)[z * height * width] = '\n';
    // render_printer->writeFile("\n");
  }
}

void generate_layer_thread(Scene *scene, const int from_x, const int from_y,
                           const int from_z, const int to_x, const int to_y,
                           const int to_z, std::shared_ptr<std::string> buf,
                           std::shared_ptr<Log> logger) {
  scene->generate_chunk(from_x, from_y, from_z, to_x, to_y, to_z, buf, logger);
}

static inline auto now() { return std::chrono::high_resolution_clock::now(); }

void Scene::render(std::string filename, bool write_to_stdout) {
  auto time1 = now();

  Log logger(filename);

  logger.write("\e[?25l\n");
  logger.register_count(depth * height * width);
  logger.step(0);

  auto raw_render =
      std::make_shared<std::string>(depth * height * width, default_filler);
  std::vector<std::thread> threads;

  auto logger_shared_ptr = std::make_shared<Log>(logger);

  const int max_thread_count = ceil((float)depth / Globals.CHUNK_PER_THREAD);

  for (int thr = 0; thr < max_thread_count; thr++) {
    // from x=0 y=0 z=depth/THC*thr to x = width y = height z=depth
    /*
      const int from_x, const int from_y, const int from_z,
      const int to_x, const int to_y, const int to_z
    */
    // TODO: Check it
    const int fromz = Globals.CHUNK_PER_THREAD * thr;
    int toz_tmp = Globals.CHUNK_PER_THREAD * (thr + 1);
    const int toz = toz_tmp >= depth ? depth : toz_tmp;

    std::cout << "Thread pushed from " << fromz << " to " << toz << "\n";

    threads.push_back(std::thread(generate_layer_thread, this, 0, 0, fromz,
                                  width, height, toz, raw_render,
                                  logger_shared_ptr));
  }

  logger.write(std::to_string(threads.size()) + " threads will be spawned\n\n");

  for (auto &&t : threads)
    t.join();

  logger.register_count(depth * height * width);
  logger.step(0);
  logger.write("\n\nPrinting...\n\n");
  for (int z = 0; z < depth; z++) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        auto cc = raw_render->at(x + y * height + z * height * width);
        logger.writeFile(cc);
        logger.step();
      }
      logger.writeFile('\n');
    }
    logger.writeFile('\n');
  }

  auto time2 = now();

  std::string postfix = "ms";

  int diff =
      std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1)
          .count();

  if (diff > 1000) {
    postfix = "s";
    diff =
        std::chrono::duration_cast<std::chrono::seconds>(time2 - time1).count();
  }

  logger.write("\nTime elapsed: ");
  logger.write(std::to_string(diff));
  logger.write(postfix);
  logger.write("\n");
}
