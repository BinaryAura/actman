#include <cstring>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>

#include "actman.h"
#include "error.h"

int32_t main(int32_t argc, const char** argv) {
  std::vector<std::string> av;
  for(int32_t i = 1; i < argc; i++) {
    av.push_back(argv[i]);
  }

  ActMan game;
  game.setup(av);
  return 0;
}
