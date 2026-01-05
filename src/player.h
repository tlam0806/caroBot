#pragma once
#include "cell.h"
#include "board.h"

class Player {
private : 
    Cell type {Cell::Empty};
public : 
    void setType (const Cell &newType) {
        type = newType;
    }
    Cell getMyType() const {
        return type;
    }
    Cell getOpType() const {
        return type == Cell::X ? Cell::O : Cell::X;
    }
    virtual bool makeMove (Board &board, const int &row, const int &col) const;
};