#ifndef BORAD_LOGIC_H
#define BOARD_LOGIC_H

struct BoardLogic {
  BoardLogic() = default;
  BoardLogic(BoardLogic&) = default;
  BoardLogic(bool fills_cell) : fills_cell(fill_cell) {}
  
  bool fills_cell;
}

#endif // BOARD_LOGIC_H
