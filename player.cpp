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

    this->board = new Board();
    this->side = s;
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
            if (board->checkMove(&m, this->side)) {
                // add to list
                v.push_back(m);
            }
        }
    }

    return v;
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

    if (opponentsMove) { // not null
        Side other = (side == BLACK) ? WHITE : BLACK;
        board->doMove(opponentsMove, other);
    }

    // TODO include condition for unlimited time or otherwise
    // but for now whatever

    std::vector<Move> movelist = getValidMoveList();

    if (movelist.size() == 0)
        return NULL; // no available moves

    //for now, play the first move
    board->doMove(&movelist[0], this->side);

    int x = movelist[0].getX();
    int y = movelist[0].getY();

    Move *m = new Move(x,y);

    return m;

}
