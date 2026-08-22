#include "../include/board.h"
#include "../include/evaluation.h"
#include "../include/search.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    Board board;
    Search search;
    Evaluation eval;

    board.InitJumperAttacks();
    board.InitMagics();

    search.rootDepth = 5;

    std::string input;
    constexpr bool run = true;
    while (run && std::getline(std::cin, input)) {
        if (input == "uci") {
            std::cout << "id name KailenEngine\nid author kailen\nuciok\n";
            std::cout.flush();
        } else if (input == "isready") {
            std::cout << "readyok\n";
            std::cout.flush();
        } else if (input.substr(0, 8) == "position") {
            std::istringstream ss(input);
            std::string token;

            // "position"
            ss >> token;

            // "startpos" or "fen"
            ss >> token;

            if (token == "startpos") {
                board = Board();
            } else if (token == "fen") {
                // FEN consists of 6 fields
                std::string fen;
                std::string fenPart;

                for (int i = 0; i < 6; i++) {
                    ss >> fenPart;

                    if (i > 0)
                        fen += " ";

                    fen += fenPart;
                }

                board.LoadFEN(fen);
            }

            // Check whether there are moves
            ss >> token;

            if (token == "moves") {
                while (ss >> token) {
                    // UCI move is normally:
                    // e2e4
                    // e7e8q
                    // etc.

                    if (token.length() < 4)
                        continue;

                    int src =
                            (token[0] - 'a') +
                            (8 - (token[1] - '0')) * 8;

                    int tgt =
                            (token[2] - 'a') +
                            (8 - (token[3] - '0')) * 8;

                    MoveList list;
                    board.GenerateMoves(list);

                    bool found = false;

                    for (int i = 0; i < list.count; i++) {
                        if (get_move_source(list.moves[i]) == src &&
                            get_move_target(list.moves[i]) == tgt) {
                            board.MakeMove(list.moves[i]);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        std::cerr << "ERROR: Could not find UCI move: "
                                << token << std::endl;
                    }
                }
            }
        } else if (input.substr(0, 2) == "go") {
            search.bestMove = 0;
            search.NegaMax(board, eval, 5, -10000, 10000);
            int move = search.bestMove;

            int src = get_move_source(move);
            int tgt = get_move_target(move);
            int promo = get_move_promo(move);

            std::cout << "bestmove ";

            std::cout << static_cast<char>('a' + src % 8)
                      << (8 - src / 8);

            std::cout << static_cast<char>('a' + tgt % 8)
                      << (8 - tgt / 8);

            if (promo != 0) {
                switch (promo) {
                    case N:
                    case n:
                        std::cout << 'n';
                        break;

                    case B:
                    case b:
                        std::cout << 'b';
                        break;

                    case R:
                    case r:
                        std::cout << 'r';
                        break;

                    case Q:
                    case q:
                        std::cout << 'q';
                        break;
                }
            }

            std::cout << "\n";
        } else if (input == "quit") {
            break;
        } else if (input == "ucinewgame") {
            board = Board();
        }
    }
    return 0;
}
