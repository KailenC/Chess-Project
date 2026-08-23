#pragma once
#include "board.h"
#include "evaluation.h"
#include <atomic>

class Search {
public:
    int NegaMax(Board board, int depth, int alpha, int beta);

    int bestMove = 0;
    int rootDepth = 0;
    long nodeCount = 0;
    std::atomic<bool>* stopFlag = nullptr;
    bool aborted = false;
};
