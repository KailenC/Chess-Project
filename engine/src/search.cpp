#include "../include/search.h"

int Search::NegaMax(Board board, Evaluation e, int depth, int alpha, int beta)
{
    if (depth == 0)
        return e.Evaluate(board);

    MoveList list;
    board.GenerateMoves(list);

    for (int i = 0; i < list.count; i++)
    {
        board.MakeMove(list.moves[i]);

        int us = board.sideToMove ^ 1;

        // check if king is in check
        U64 king = (us == board.white) ? board.whiteKing : board.blackKing;
        if (board.IsSquareAttacked(GetLSBIndex(king)))
            continue;

        int score = -NegaMax(board, e, depth - 1, -beta, -alpha);

        if (score > alpha)
            alpha = score;
        if (alpha >= beta)
            break;
    }
    return alpha;
}