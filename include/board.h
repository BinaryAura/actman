#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "objects/gameobject.h"
#include "cell.h"


class Board {
public:

  Board(uint32_t rows=0, uint32_t cols=0) : rows(rows), cols(cols) {}

  uint32_t get_rows() {
    return this->rows;
  }

  uint32_t get_cols() {
    return this->cols;
  }

  std::vector<Cell> get_neighbors(Cell cell);
  std::vector<GameObject*> get_objects_at(Cell cell);
  void add_object(GameObject& object);
  void remove_object(std::vector<GameObject*>::iterator object);
private:
  uint32_t rows, cols;
  std::vector<GameObject*> objects;
};

#endif // BOARD_H
