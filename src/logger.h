#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>

enum LoggerLevel { NORMAL, DEBUG };

class Log {
public:
  Log(){};
  explicit Log(std::string filename);
  void write(std::string printable);
  void write(char printable);
  void writeFile(char printable, LoggerLevel lev = NORMAL);
  void writeFile(std::string printable, LoggerLevel lev = NORMAL);
  void step(int size = 1);
  void register_count(int count);

private:
  static const std::map<LoggerLevel, std::string> level;
  std::string filename;
  int count = 0;
  int current_step = 0;
  int old_step = 0;

  void clear_file();
};
