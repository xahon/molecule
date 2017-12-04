#include "logger.h"

Log::Log(std::string filename) : filename(filename) { clear_file(); };

const std::map<LoggerLevel, std::string> Log::level = {{NORMAL, ".vox"},
                                                       {DEBUG, ".debug"}};

void Log::clear_file() {
  if (filename.size() <= 0) {
    return;
  }

  // std::ofstream(filename + ".vox", std::ios::trunc);
  // std::ofstream(filename + ".debug", std::ios::trunc);

  for (int lev = LoggerLevelBegin; lev != LoggerLevelEnd; lev++) {
    const auto ext = level.find(LoggerLevel(lev))->second;
    std::ofstream file(filename + ext, std::ios::trunc);
    file.close();
  }
}

void Log::write(std::string printable) { std::cout << printable; }
void Log::write(char printable) { std::cout << std::string(1, printable); }

void Log::writeFile(char printable, LoggerLevel lev) {
  writeFile(std::string(1, printable), lev);
}

void Log::writeFile(std::string printable, LoggerLevel lev) {
  if (filename.size() <= 0) {
    std::cout
        << "BUG: Attempt to write file without passing its name in constructor!"
        << '\n';
    return;
  }

  std::string extension;
  auto pos = level.find(lev);

  if (pos == level.end()) {
    std::cout << "BUG: Unknown logger level!" << lev << '\n';
    extension = ".unknown";
  } else {
    extension = pos->second;
  }

  std::ofstream stream(filename + extension, std::ios::app);
  stream << printable;
  stream.close();
}

void Log::step(int size) {
  current_step += size;

  int next_percentage = (int)((float)current_step / count * 100);
  int current_percentage = (int)((float)old_step / count * 100);

  if (current_percentage != next_percentage) {
    std::cout << "\rProgress: " << next_percentage << "%                      "
              << std::flush;
    old_step = current_step;
  }
}

void Log::register_count(int count) {
  if (count < 0) {
    throw "Count must be greater than 0";
  }

  Log::count = count;
}
