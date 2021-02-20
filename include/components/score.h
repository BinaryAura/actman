#ifndef COMPONENTS_SCORE_H
#define COMPONENTS_SCORE_H

struct Score {
  Score() = default;
  Score(const Score&) = default;
  Score(const uint32_t value) : value(value) {}

  uint32_t value;
};

#endif // COMPONENTS_SCORE_H
