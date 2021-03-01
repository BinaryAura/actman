#ifndef CONMPONENTS_DUNKY_MEMORY
#define CONMPONENTS_DUNKY_MEMORY

#include <vector>
// #include "components/transform.h"
#include "cell.h"

struct DunkyMemory {
  DunkyMemory() = default;
  DunkyMemory(const DunkyMemory&) = default;
  DunkyMemory(const std::vector<Cell> data) : data(data) {}

  std::vector<Cell> data;
};

#endif // CONMPONENTS_DUNKY_MEMORY
