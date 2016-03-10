#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    board = new Board();
    side = s;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

std::vector<Move> Player::getValidMoveList () {

    std::vector<Move> v;
    //iterate through all positions of board TODO improve efficiency? we don't have to iterate though all positions, really... but then the board is small
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move m(i,j);
            if (board->checkMove(&m, side)) {
                // add to list
                v.push_back(m);
            }
        }
    }

    return v;
}

int getMultiplier (int x, int y) {
    if (x == 1 or x == 6)
        if (y == 1 or y == 6)
            return -2;

    if (x == 0 or x == 7)
        if (y == 0 or y == 7)
            return 3;
        if (y == 1 or y == 6)
            return -3;
        return 2;

    if (y == 0 or y == 7)
        if (x == 1 or x == 6)
            return -3;
        return 2;

    return 1;

}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move * Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    // timeLeft -= msLeft;
    Side other = (side == BLACK) ? WHITE : BLACK;

    if (opponentsMove) { // not null
        board->doMove(opponentsMove, other);
    }

    // TODO include condition for unlimited time or otherwise
    // but for now whatever

    std::vector<Move> movelist = getValidMoveList();
    if (movelist.size() == 0)
        return NULL; // no available moves

    int newBoardScore, highest = std::numeric_limits<int>::min();
    int best, multiplier;
    for (int i = 0; i < movelist.size(); i++) {
        Board *tryout = board->copy(); // :S
        tryout->doMove(&movelist[i], side);
        newBoardScore = tryout->count(side) - tryout->count(other);

        multiplier = getMultiplier(movelist[i].getX(), movelist[i].getY());

        if (multiplier < 0 and newBoardScore < 0)
            newBoardScore *= -multiplier;
        else newBoardScore *= multiplier;

        if (newBoardScore > highest) {
            highest = newBoardScore;
            best = i;
        }

        delete tryout;
    }

    //board->doMove(&movelist[best], side);

    int x = movelist[best].getX();
    int y = movelist[best].getY();

    Move *m = new Move(x,y);
    board->doMove(m, side);

    return m;

}
