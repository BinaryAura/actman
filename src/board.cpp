#include "board.h"

#include <algorithm>

std::vector<Cell> Board::get_neighbors(Cell cell) {
  std::vector<Cell> neighbors;
  if (cell.x > 0)
    neighbors.emplace_back(cell.x - 1, cell.y);
  if (cell.y > 0)
    neighbors.emplace_back(cell.x, cell.y - 1);
  if (cell.x < this->cols - 1)
    neighbors.emplace_back(cell.x + 1, cell.y);
  if (cell.y < this->rows - 1)
    neighbors.emplace_back(cell.x, cell.y + 1);
  return neighbors;
}

std::vector<GameObject*> Board::get_objects_at(const Cell cell) {
  std::vector<GameObject*> objects;

  for(GameObject* it : this->objects) {
    if (it->get_cell() == cell) {
      objects.emplace_back(it);
    }
  }
  return objects;
}

void Board::add_object(GameObject& object) {
  this->objects.emplace_back(&object);
}

void Board::remove_object(std::vector<GameObject*>::iterator object) {
  this->objects.erase(object);
}
