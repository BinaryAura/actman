#ifndef COMPONENTS_MOVE_LOG_H
#define COMPONENTS_MOVE_LOG_H

#include <vector>

struct MoveLog {
  MoveLog() = default;
  MoveLog(const MoveLog&) = default;
  MoveLog(const std::vector<float>& moves) : moves(moves) {}

  std::vector<float> moves;
};

#endif // COMPONENTS_MOVE_LOG_H
