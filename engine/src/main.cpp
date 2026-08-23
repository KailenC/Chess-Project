#include "../include/board.h"
#include "../include/evaluation.h"
#include "../include/search.h"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

struct EngineState {
    std::atomic_bool searching{false};
    std::atomic_bool stopRequested{false};
    std::atomic_bool quit{false};
};

void uci_loop(EngineState &state, std::thread &search_thread) {
    Board board;
    Search search;

    board.InitJumperAttacks();
    board.InitMagics();

    search.rootDepth = 5;

    std::string input;
    while (std::getline(std::cin, input)) {
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
                            // if there is a promo, promote the correct piece
                            if (get_move_promo(list.moves[i]) != 0) {
                                std::string move = MoveToString(list.moves[i]);
                                if (move[4] != token[4]) continue;
                            }
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
            state.stopRequested = true;

            if (search_thread.joinable())
                search_thread.join();

            state.stopRequested = false;
            state.searching = false;

            state.searching = true;
            search.aborted = false;
            search.stopFlag = &state.stopRequested;
            Board searchBoard = board;
            search_thread = std::thread([&state, &search, searchBoard]() {
                int move = 0ULL;


                search.bestMove = 0;
                search.NegaMax(searchBoard, 5, -10000, 10000);
                move = search.bestMove;
                state.searching = false;


                const int src = get_move_source(move);
                const int tgt = get_move_target(move);
                const int promo = get_move_promo(move);

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
                std::cout.flush();
            });
        } else if (input == "quit") {
            state.stopRequested = true;
            state.quit = true;
            break;
        } else if (input == "ucinewgame") {
            board = Board();
            board.InitJumperAttacks();
            board.InitMagics();
        } else if (input == "stop") {
            state.stopRequested = true;
        }
    }
}

/*
 * need to implement go methods
* go wtime 9800 btime 10000 winc 100 binc 100

Here's what each field means:

wtime <ms> — White's remaining time on the clock, in milliseconds
btime <ms> — Black's remaining time on the clock, in milliseconds
winc <ms> — White's increment added per move (your +0.1 → 100ms)
binc <ms> — Black's increment added per move

 *
 */

int main() {
    EngineState state;
    std::thread search_thread;

    uci_loop(state, search_thread);

    state.searching = false;
    if (search_thread.joinable()) {
        search_thread.join();
    }

    return 0;
}
