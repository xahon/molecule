#pragma once

#include <iostream>

void exit_handler(int signum) {
  std::cout << "\e[?25h\n"; // Make terminal caret visible again

  exit(signum);
}
