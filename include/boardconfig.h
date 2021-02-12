#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <vector>
#include <filesystem>

// #include "board.h"
#include "cell.h"

enum class Direction {NORTH, EAST, SOUTH, WEST, NUM_DIRS};

enum class Ghost {PUNKY, BUNKY, DUNKY, RUNKY, NUM_GHOSTS};

class BoardConfig {
public:
  BoardConfig(uint32_t rows=0, uint32_t cols=0) : rows(rows), cols(cols) {}
  BoardConfig(std::filesystem::path config_file);

  void set_state(std::vector<std::string> board);
  void set_direction(Ghost ghost, Direction dir);
  void set_patrol(std::vector<Cell> patrol);
  void add_to_patrol(Cell cell);
  void clear_patrol();
  void set_pool(std::vector<Direction> dirs);
  void add_to_pool(Direction dir);
  void clear_pool();

  uint32_t rows, cols;
  std::vector<std::string> init_state;
  Direction dirs[4];
  std::vector<Cell> patrol;
  std::vector<Direction> dir_pool;

private:
  void load_config(std::filesystem::path config_file);
};

#endif // BOARD_CONFIG_H
