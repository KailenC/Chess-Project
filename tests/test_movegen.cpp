#include <catch2/catch_test_macros.hpp>
#include "board.h"

namespace {
    struct EngineTablesFixture {
        EngineTablesFixture() {
            static bool done = false;
            if (!done) {
                Board temp;
                temp.InitJumperAttacks();
                temp.InitMagics();
                done = true;
            }
        }
    };
}

TEST_CASE_METHOD(EngineTablesFixture, "Board initialization and side to move", "[board]") {
    Board board;
    board.InitJumperAttacks();
    board.InitMagics();

    REQUIRE(board.sideToMove == Board::white);

    REQUIRE(board.Occupancy(Board::white) != 0);
    REQUIRE(board.Occupancy(Board::black) != 0);
}


TEST_CASE_METHOD(EngineTablesFixture, "Perft startpos from depth 1 to depth 6") {
    Board board;
    board.InitJumperAttacks();
    board.InitMagics();

    //board.Divide(1);

    REQUIRE(board.Perft(1) == 20);
    REQUIRE(board.Perft(2) == 400);
    REQUIRE(board.Perft(3) == 8902);
    REQUIRE(board.Perft(4) == 197281);
    REQUIRE(board.Perft(5) == 4865609);
    //REQUIRE(board.Perft(6) == 119060324);
}

TEST_CASE_METHOD(EngineTablesFixture, "Kiwipete test") {
    Board board;
    board.LoadFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    REQUIRE(board.Perft(1) == 48);
    REQUIRE(board.Perft(2) == 2039);
    REQUIRE(board.Perft(3) == 97862);
    REQUIRE(board.Perft(4) == 4085603);
    //REQUIRE(board.Perft(5) == 193690690);
}

TEST_CASE_METHOD(EngineTablesFixture, "test 2") {
    Board board;
    board.LoadFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ");
    REQUIRE(board.Perft(1) == 44);
    REQUIRE(board.Perft(2) == 1486);
    REQUIRE(board.Perft(3) == 62379);
    REQUIRE(board.Perft(4) == 2103487);
    //REQUIRE(board.Perft(5) == 89941194);
}

TEST_CASE_METHOD(EngineTablesFixture, "test 3") {
    Board board;
    board.LoadFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");
    REQUIRE(board.Perft(1) == 46);
    REQUIRE(board.Perft(2) == 2079);
    REQUIRE(board.Perft(3) == 89890);
    REQUIRE(board.Perft(4) == 3894594);
}