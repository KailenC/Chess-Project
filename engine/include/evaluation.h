#pragma once
#include "board.h"

class Evaluation {
public:
    int Evaluate(Board board);

private:
    enum {
        pawn = 100,
        knight = 300,
        bishop = 300,
        rook = 500,
        queen = 900
    };
};
