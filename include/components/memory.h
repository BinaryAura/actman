#ifndef COMPONENTS_MEMORY_H
#define COMPONENTS_MEMORY_H

#include<vector>

template <typename T>
struct Memory {
  Memory() = default;
  Memory(const Memory&) = default;
  Memory(const std::vector<T> data) : data(data) {}

  std::vector<T> data;
};

#endif // COMPONENTS_MEMORY_H
