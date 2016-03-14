#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
#include <limits>
#include <algorithm>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Board *board;

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
private:
    Side side;
    pair<Move, int> getBestMove (Board *b, bool seekingMax, int depth, int lastX, int lastY, int alpha, int beta);
};

#endif
