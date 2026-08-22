#include "../include/evaluation.h"

int Evaluation::Evaluate(const Board &board) {
    // white
    const int pawnScore = pawn * CountBits(board.whitePawns);
    const int rookScore = rook * CountBits(board.whiteRooks);
    const int knightScore = knight * CountBits(board.whiteKnights);
    const int bishopScore = bishop * CountBits(board.whiteBishops);
    const int queenScore = queen * CountBits(board.whiteQueens);
    const int kingScore = king * CountBits(board.whiteKing);

    // black
    const int pawnScore2 = pawn * CountBits(board.blackPawns);
    const int rookScore2 = rook * CountBits(board.blackRooks);
    const int knightScore2 = knight * CountBits(board.blackKnights);
    const int bishopScore2 = bishop * CountBits(board.blackBishops);
    const int queenScore2 = queen * CountBits(board.blackQueens);
    const int kingScore2 = king * CountBits(board.blackKing);

    const int score = pawnScore + rookScore + knightScore + bishopScore + queenScore + kingScore - (
                    pawnScore2 + rookScore2 + knightScore2 + bishopScore2 + queenScore2 + kingScore2);
    return (board.sideToMove == Board::white) ? score : -score;
}
