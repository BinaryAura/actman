// #include <stdio.h>
#include <fstream>
// #include <iostream>
// #include <math.h>

#include <hadron/error.h>
#include "boardconfig.h"

BoardConfig::BoardConfig(std::filesystem::path config_file) {
  this->load_config(config_file);
}

void BoardConfig::load_config(std::filesystem::path config_file) {
  uint32_t l = 1, c = 1;
  std::ifstream config(config_file);

  if(!config) {
    throw NoSuchFile(config_file.string());
  }


  uint32_t rows, cols;
  config >> rows >> cols;
  this->rows = rows;
  this->cols = cols;
  if(rows < 0) {
    throw ConfigError(config_file.string(), l, -1, std::to_string(rows).c_str());
  }
  if(cols < 0) {
    throw ConfigError(config_file.string(), l, -1, std::to_string(cols).c_str());
  }
  config.ignore();
  l++;

  char *buffer = new char[cols + 1];
  for(uint32_t r = 0; r < rows; r++) {
    config.getline(buffer, cols + 1);
    this->init_state.push_back(std::string(buffer));
    for(auto ch : this->init_state.back()) {
      switch(ch) {
        case ' ':
        case '#':
        case '.':
        case '$':
        case '*':
        case 'A':
        case 'P':
        case 'B':
        case 'D':
        case 'R':
          break;
        default:
          throw ConfigError(config_file.string(), l, c, std::string(&ch, 1).c_str());
      }
      c++;
    }
    c = 1;
    l++;
  }
  delete[] buffer;
  char dirs[4];
  config.getline(dirs, 5);
  for(uint32_t i = uint32_t(Ghost::PUNKY); i < uint32_t(Ghost::NUM_GHOSTS); i++) {
    try {
      this->set_direction(Ghost(i), this->parse_direction(dirs[i]));
    } catch(const ConfigError& err) {
      throw ConfigError(config_file, l, c, err.text);
    }
    c++;
  }
  l++;
  uint32_t n_chkpts, x, y;
  config >> n_chkpts;
  if(n_chkpts < 0)
    throw ConfigError(config_file, l, -1, std::to_string(n_chkpts).c_str());
  for (uint32_t i = 0; i < n_chkpts; i++) {
    config >> y >> x;
    if(x < 0)
      throw ConfigError(config_file, l, -1, std::to_string(x).c_str());
    if(y < 0)
      throw ConfigError(config_file, l, -1, std::to_string(y).c_str());
    add_to_patrol({x, y});
  }
  config.ignore();
  l++;
  do {
    try {
      this->add_to_pool(this->parse_direction(config.get()));
    } catch(const ConfigError& err) {
      throw ConfigError(config_file, l, c, err.text);
    }

  } while(config.peek() == 'U' || config.peek() == 'R' || config.peek() == 'D' || config.peek() == 'L');
  if(this->dir_pool.size() <= 0)
    throw ConfigError(config_file, l, -1, "");
}

void BoardConfig::set_state(std::vector<std::string>& board) {
  this->init_state = board;
  this->rows = board.size();
  this->cols = board[0].size();
}

void BoardConfig::set_direction(Ghost ghost, float dir) {
  this->dirs[uint32_t(ghost)] = dir;
}

void BoardConfig::set_patrol(std::vector<Cell>& patrol) {
  this->patrol = patrol;
}

void BoardConfig::add_to_patrol(Cell cell) {
  this->patrol.push_back(cell);
}

void BoardConfig::clear_patrol() {
  this->patrol.clear();
}

void BoardConfig::set_pool(std::vector<float>& dirs) {
  this->dir_pool = dirs;
}

void BoardConfig::add_to_pool(float dir) {
  this->dir_pool.push_back(dir);
}

void BoardConfig::clear_pool() {
  this->dir_pool.clear();
}

BoardConfig::BoardConfig(BoardConfig& config) {
  this->rows = config.rows;
  this->cols = config.cols;
  this->init_state = config.init_state;
  for(uint32_t i = 0; i < 4; i++) {
    this->dirs[i] = config.dirs[i];
  }
  this->patrol = config.patrol;
  this->dir_pool = config.dir_pool;
}

BoardConfig& BoardConfig::operator= (const BoardConfig& config) {
  if (this == &config) {
    return *this;
  }
  this->rows = config.rows;
  this->cols = config.cols;
  this->init_state = config.init_state;
  for(uint32_t i = 0; i < 4; i++) {
    this->dirs[i] = config.dirs[i];
  }
  this->patrol = config.patrol;
  this->dir_pool = config.dir_pool;
  return *this;
}

float BoardConfig::parse_direction(char dir) const {
  switch(dir) {
    case 'U':
      return 0;
      break;
    case 'R':
      return 90;
      break;
    case 'D':
      return 180;
      break;
    case 'L':
      return 270;
      break;
  }
  throw ConfigError("", -1, -1, std::string(&dir, 1).c_str());
}
