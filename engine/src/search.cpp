#include "../include/search.h"

int Search::NegaMax(Board board, const int depth, int alpha, const int beta) {
    if (stopFlag && stopFlag->load(std::memory_order_relaxed)) {
        aborted = true;
        return 0;
    }

    if (depth == 0)
        return Evaluation::Evaluate(board);

    if (aborted) return 0;

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
        U64 const king = (us == copy.white) ? copy.whiteKing : copy.blackKing;
        if (copy.IsSquareAttacked(GetLSBIndex(king)))
            continue;

        legalMoves++;

        const int score = -NegaMax(copy, depth - 1, -beta, -alpha);

        if (aborted) return 0;

        if (score > alpha) {
            alpha = score;
            if (depth == rootDepth)
                bestMove = list.moves[i];
        }
        if (alpha >= beta)
            break;
    }

    if (legalMoves == 0) {
        const U64 king = (board.sideToMove == Board::white) ? board.whiteKing : board.blackKing;

        board.sideToMove ^= 1;
        const bool inCheck = board.IsSquareAttacked(GetLSBIndex(king));
        board.sideToMove ^= 1;

        return inCheck ? -100000 + (rootDepth - depth) : 0;
    }

    return alpha;
}
