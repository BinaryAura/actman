#ifndef CONMPONENTS_RUNKY_MEMORY
#define CONMPONENTS_RUNKY_MEMORY

#include <vector>

struct RunkyMemory {
  RunkyMemory() = default;
  RunkyMemory(const RunkyMemory&) = default;
  RunkyMemory(const std::vector<float> data) : data(data) {}

  std::vector<float> data;
};

#endif // CONMPONENTS_RUNKY_MEMORY
