#include <iostream>
#include <stdlib.h>
#include <inttypes.h>
#include <random>

#include <vector>

#include "board.h"
#include "objects/wall.h"

int32_t main(int32_t argc, char** argv) {
  Board board(4,4);
  std::vector<Wall> walls;

  for (uint32_t i = 0; i < 4; i++) {
    walls.emplace_back(0,i);
  }

  for (uint32_t i = 0; i < 4; i++) {
    board.add_object(walls[i]);
  }
}
