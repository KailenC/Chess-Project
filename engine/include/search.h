#pragma once
#include "board.h"
#include "evaluation.h"

class Search
{
public:
    int NegaMax(Board &board, Evaluation e, int depth, int alpha, int beta);

    int bestMove = 0;
    int rootDepth = 0;
};