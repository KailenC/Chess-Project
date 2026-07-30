#include "../include/evaluation.h"

int Evaluation::Evaluate(Board board) {
    // white
    int pawnScore = pawn * CountBits(board.whitePawns);
    int rookScore = rook * CountBits(board.whiteRooks);
    int knightScore = knight * CountBits(board.whiteKnights);
    int bishopScore = bishop * CountBits(board.whiteBishops);
    int queenScore = queen * CountBits(board.whiteQueens);

    // black
    int pawnScore2 = pawn * CountBits(board.blackPawns);
    int rookScore2 = rook * CountBits(board.blackRooks);
    int knightScore2 = knight * CountBits(board.blackKnights);
    int bishopScore2 = bishop * CountBits(board.blackBishops);
    int queenScore2 = queen * CountBits(board.blackQueens);

    int score = pawnScore + rookScore + knightScore + bishopScore + queenScore - (
                    pawnScore2 + rookScore2 + knightScore2 + bishopScore2 + queenScore2);
    return (board.sideToMove == board.white) ? score : -score;
}
