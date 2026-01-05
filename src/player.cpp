#include "player.h"
#include <iostream>

bool Player::makeMove (Board &board, const int &row, const int &col) const {
    assert(board.inBoard(row, col));
    if (board.getCell(row, col) != Cell::Empty) return false;
    board.setCell(row, col, type);
    Pattern pattern = board.getPattern(Cell::X);
    return true;
}