#pragma once
#include "board.h"

class Evaluation
{
public:
    int Evaluate(Board board);

private:
    enum
    {
        pawn = 1,
        knight = 3,
        bishop = 3,
        rook = 5,
        queen = 9
    };
};