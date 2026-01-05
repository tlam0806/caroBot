#include <cassert>
#include <vector>
#include "board.h"
#include <iostream>

std::vector<int> dx = {-1, 0, 1, 1};
std::vector<int> dy = {1, 1, 1, 0};

void Board::setCell (const int &row, const int &col, const Cell &value) {
    assert(row >= 0 && row < size && col >= 0 && col < size);
    if (value != Cell::Empty) moveHistory.emplace(row, col);
    // assert(cells[row][col] == Cell::Empty);
    cells[row][col] = value;
}

Cell Board::checkWinner() const {
    for (int r = 0; r < size; r++) for (int c = 0; c < size; c++) {
        if (cells[r][c] == Cell::Empty) continue;
        for (int dir = 0; dir < 4; dir++) {
            int _r = r, _c = c;
            bool win = 1;
            for (int step = 0; step < winningLen; step++) {
                if (!inBoard(_r, _c) || cells[r][c] != cells[_r][_c]) {
                    win = 0;
                    break;
                }
                _r += dx[dir];
                _c += dy[dir];
            }
            if (win) {
                return cells[r][c];
            }
        }
    }
    return Cell::Empty;
}

int Board::expand (int x, int y, int dX, int dY, Cell type) const {
    int ret = 0;
    while (inBoard(x, y) && getCell(x, y) == type) {
        ret++;
        x += dX;
        y += dY;
    }
    return ret;
}

Pattern Board::getPattern (const Cell& type) const {
    Pattern pattern;
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) for (int dir = 0; dir < 4; dir++) {
        if (cells[i][j] != type) continue;
        int x = i, y = j;
        int headX = x - dx[dir], headY = y - dy[dir];
        if (inBoard(headX, headY) && cells[headX][headY] == cells[i][j]) continue;
        // std::cout << i << " " << j << "\n";
        int headLen = expand(headX, headY, -dx[dir], -dy[dir], Cell::Empty);
        int curLen = expand(x, y, dx[dir], dy[dir], cells[i][j]);
        int tailLen = expand(x + curLen * dx[dir], y + curLen * dy[dir], dx[dir], dy[dir], Cell::Empty);
        if (headLen + curLen + tailLen < 5) continue; 
        if (curLen <= 1) continue;
        int open = std::min(1, headLen) + std::min(1, tailLen);
        if (!open) continue;
        if (curLen == 4) {
            if (open == 2) pattern.openFour++;
            else pattern.closedFour++;
        } else if (curLen == 3) {
            if (open == 2 && headLen + tailLen > 2) pattern.openThree++;
            else pattern.closedThree++;
        } else if (curLen == 2) {
            if (open == 2) {
                pattern.openTwo++;
                // i << " " << j << " " << dx[dir] << " " << dy[dir] << "\n";
            }
        }
    }
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) for (int dir = 0; dir < 4; dir++) {
        int x = i, y = j;
        if (cells[x][y] != Cell::Empty) continue;
        int len1 = expand(x + dx[dir], y + dy[dir], dx[dir], dy[dir], type);
        int len2 = expand(x - dx[dir], y - dy[dir], -dx[dir], -dy[dir], type);
        if (len1 == 0 || len2 == 0) continue;
        if (len1 + len2 >= 4) {
            pattern.brokenFour++;
        } else if (len1 + len2 == 3) {
            int open = std::min(expand(x + dx[dir] * len1, y + dy[dir] * len2, dx[dir], dy[dir], Cell::Empty)
        , 1) + std::min(expand(x - dx[dir] * len2, y - dy[dir] * len2, -dx[dir], -dy[dir], Cell::Empty), 1);
            if (open == 2) {
                pattern.openBrokenThree++;
            } else if (open == 1) {
                pattern.closedBrokenThree++;
            }  
        }
    }
    return pattern;
}

bool Board::rollBack() {
    if (moveHistory.empty()) return false;
    auto [x, y] = moveHistory.top(); moveHistory.pop();
    setCell(x, y, Cell::Empty); 
    return true;
}

bool Board::isDraw () const {
    return moveHistory.size() == size * size;
}