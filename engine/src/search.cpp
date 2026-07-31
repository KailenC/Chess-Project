#include "../include/search.h"

int Search::NegaMax(Board &board, Evaluation e, const int depth, int alpha, const int beta) {
    if (depth == 0)
        return e.Evaluate(board);

    MoveList list;
    board.GenerateMoves(list);

    if (depth == rootDepth) {
        for (int i = 0; i < list.count; i++) {
            Board copy = board;
            copy.MakeMove(list.moves[i]);
            const int us = copy.sideToMove ^ 1;
            const U64 king = (us == copy.white) ? copy.whiteKing : copy.blackKing;
            if (!copy.IsSquareAttacked(GetLSBIndex(king))) {
                bestMove = list.moves[i];
                break;
            }
        }
    }

    int legalMoves = 0;

    for (int i = 0; i < list.count; i++) {
        Board copy = board;
        copy.MakeMove(list.moves[i]);

        const int us = copy.sideToMove ^ 1;

        // check if king is in check
        U64 king = (us == copy.white) ? copy.whiteKing : copy.blackKing;
        if (copy.IsSquareAttacked(GetLSBIndex(king)))
            continue;

        legalMoves++;

        int score = -NegaMax(copy, e, depth - 1, -beta, -alpha);

        if (score > alpha) {
            alpha = score;
            if (depth == rootDepth)
                bestMove = list.moves[i];
        }
        if (alpha >= beta)
            break;
    }

    if (legalMoves == 0) {
        const U64 king = (board.sideToMove == board.white) ? board.whiteKing : board.blackKing;

        board.sideToMove ^= 1;
        bool inCheck = board.IsSquareAttacked(GetLSBIndex(king));
        board.sideToMove ^= 1;

        return inCheck ? -100000 + (rootDepth - depth) : 0;
    }

    return alpha;
}
