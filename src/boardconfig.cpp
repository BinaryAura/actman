#include <stdio.h>
#include <fstream>
#include <iostream>

#include "error.h"
#include "boardconfig.h"

BoardConfig::BoardConfig(std::filesystem::path config_file) {
  this->load_config(config_file);
}

void BoardConfig::load_config(std::filesystem::path config_file) {
  std::ifstream config(config_file);

  if(!config) {
    throw NoSuchFile(config_file.string());
  }

  uint32_t rows, cols;
  config >> rows >> cols;
  config.ignore();

  std::vector<std::string> board;
  char *buffer = new char[cols + 1];
  for(uint32_t r = 0; r < rows; r++) {
    config.getline(buffer, cols + 1);
    board.push_back(std::string(buffer));
  }
  delete buffer;
  this->set_state(board);
  char dirs[4];
  config.getline(dirs, 5);
  for(uint32_t i = uint32_t(Ghost::PUNKY); i < uint32_t(Ghost::NUM_GHOSTS); i++) {
    Direction d;
    switch(dirs[i]) {
      case 'U':
        d = Direction::NORTH;
        break;
      case 'R':
        d = Direction::EAST;
        break;
      case 'D':
        d = Direction::SOUTH;
        break;
      case 'L':
        d = Direction::WEST;
        break;
    }
    this->set_direction(Ghost(i), d);
  }
  uint32_t n_chkpts, x, y;
  config >> n_chkpts;
  for (uint32_t i = 0; i < n_chkpts; i++) {
    config >> y >> x;
    add_to_patrol({x, y});
  }
  config.ignore();
  char buf;
  do {
    buf = config.get();
    Direction d;
    switch(buf) {
      case 'U':
        d = Direction::NORTH;
        break;
      case 'R':
        d = Direction::EAST;
        break;
      case 'D':
        d = Direction::SOUTH;
        break;
      case 'L':
        d = Direction::WEST;
        break;
      default:
        buffer = 0;
        break;
    }
    this->add_to_pool(d);
  }while(buffer);
}

void BoardConfig::set_state(std::vector<std::string> board) {
  this->init_state = board;
  this->rows = board.size();
  this->cols = board[0].size();
}

void BoardConfig::set_direction(Ghost ghost, Direction dir) {
  this->dirs[uint32_t(ghost)] = dir;
}

void BoardConfig::set_patrol(std::vector<Cell> patrol) {
  this->patrol = patrol;
}

void BoardConfig::add_to_patrol(Cell cell) {
  this->patrol.push_back(cell);
}

void BoardConfig::clear_patrol() {
  this->patrol.clear();
}

void BoardConfig::set_pool(std::vector<Direction> dirs) {
  this->dir_pool = dirs;
}

void BoardConfig::add_to_pool(Direction dir) {
  this->dir_pool.push_back(dir);
}

void BoardConfig::clear_pool() {
  this->dir_pool.clear();
}
