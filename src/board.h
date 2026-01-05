#pragma once
#include <cassert>
#include <iostream>
#include "cell.h"
#include "pattern.h"
#include <stack>
#include "random.h"

const int size = 15;
const int winningLen = 5;

class Pattern;

class Board {
private :
    std::stack<std::pair<int, int> > moveHistory;
    Cell cells[size][size] = {Cell::Empty};
    int score = {0};
public :
    std::pair<int, int> getLastMove () const {
        if (moveHistory.empty()) return {-1, -1};
        return moveHistory.top();
    }
    void print () const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (cells[i][j] == Cell::X) {
                    std::cout << "X";
                } else if (cells[i][j] == Cell::O) {
                    std::cout << "O";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }
    bool inBoard (const int &x, const int &y) const {
        return 0 <= x && x < size && 0 <= y && y < size;
    }
    Cell getCell (const int &row, const int &col) const {
        assert(inBoard(row, col));   
        return cells[row][col];
    }
    void setCell (const int &row, const int &col, const Cell &value);
    Cell checkWinner() const;
    Pattern getPattern(const Cell &type) const;
    bool rollBack ();
    bool isDraw () const;
    int expand (int x, int y, int dX, int dY, Cell type) const;
};