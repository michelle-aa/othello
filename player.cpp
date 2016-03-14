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
     * Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    board = new Board();
    side = s;
    pieceCount = 0;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

std::vector<Move> getValidMoveList (Board *b, Side s) {

    std::vector<Move> v;

    int x, y;

    list<int>::iterator it;

    for (it = b->available.begin(); it != b->available.end(); ++it) {
        x = *it % 8;
        y = *it / 8;
        Move m(x, y);

        if (b->checkMove(&m, s)) {
            // add to list
            v.push_back(m);
        }
    }

    return v;
}

int Player::getMultiplier (int x, int y) {
    if (x < 0)
        return 1;

    // 0-19: opening, 20-40: midgame, >=41: endgame
    if (pieceCount < 20) {
        // opening

        if (x == 1 or x == 6)
            if (y == 1 or y == 6)
                return -2; // non-edge pieces diagonally adjacent to corners

        if (x == 0 or x == 7)
            if (y == 0 or y == 7)
                return 3; // corner pieces
            if (y == 1 or y == 6)
                return -3; // edge pieces adjacent to corners
            return 2; // other edge pieces

        if (y == 0 or y == 7)
            if (x == 1 or x == 6)
                return -3; // edge pieces adjacent to corners
            return 2; // other edge pieces

        return 1; // all other pieces

    } else if (pieceCount > 40) {
        // endgame

        if (x == 1 or x == 6)
            if (y == 1 or y == 6)
                return -2; // non-edge pieces diagonally adjacent to corners

        if (x == 0 or x == 7)
            if (y == 0 or y == 7)
                return 2; // corner pieces
            if (y == 1 or y == 6)
                return -2; // edge pieces adjacent to corners
            return 2; // other edge pieces

        if (y == 0 or y == 7)
            if (x == 1 or x == 6)
                return -2; // edge pieces adjacent to corners
            return 2; // other edge pieces

        return 1; // all other pieces
    } else {
        // midgame

        if (x == 1 or x == 6)
            if (y == 1 or y == 6)
                return -2; // non-edge pieces diagonally adjacent to corners

        if (x == 0 or x == 7)
            if (y == 0 or y == 7)
                return 3; // corner pieces
            if (y == 1 or y == 6)
                return -3; // edge pieces adjacent to corners
            return 2; // other edge pieces

        if (y == 0 or y == 7)
            if (x == 1 or x == 6)
                return -3; // edge pieces adjacent to corners
            return 2; // other edge pieces

        return 1; // all other pieces
    }

}

pair<Move, int> Player::getBestMove (Board *b, bool seekingMax, int depth, int lastX, int lastY, int alpha, int beta) {
    Side enemy = (side == BLACK) ? WHITE : BLACK;
    int score;
    Side current;

    if (seekingMax)
        current = side;
    else // opponent; seeking min
        current = (side == BLACK) ? WHITE : BLACK;

    Side other = (current == BLACK) ? WHITE : BLACK;

    std::vector<Move> movelist = getValidMoveList(b, current);

    // if at end of tree/no possible moves, return own score
    if (depth == 0 or movelist.size() == 0) {
        score = b->count(side) - b->count(enemy);

        int multiplier = getMultiplier(lastX, lastY);
        if (testingMinimax)
            multiplier = 1;

        if (multiplier < 0 and score < 0)
            score *= -multiplier;
        else score *= multiplier;

        Move move(-1,-1);
        pair<Move, int> p (move, score);
        return p;
    }

    // otherwise, do a DFS on possible moves
    int best = 0, bestScore;

    if (seekingMax) {
        // as maximising player
        bestScore = std::numeric_limits<int>::min();

        for (int i = 0; i < movelist.size(); i++) {
            Board *tryout = b->copy();
            tryout->doMove(&movelist[i], current);
            int x = movelist[i].getX();
            int y = movelist[i].getY();
            score = getBestMove(tryout, other, depth-1, x, y, alpha, beta).second;

            delete tryout;

            if (score > bestScore) {
                bestScore = score;
                best = i;
                alpha = max(bestScore, alpha);
            }

            if (alpha > beta)
                break;
        }
    } else {
        // as minimising player
        bestScore = std::numeric_limits<int>::max();

        for (int i = 0; i < movelist.size(); i++) {
            Board *tryout = b->copy();
            tryout->doMove(&movelist[i], current);
            int x = movelist[i].getX();
            int y = movelist[i].getY();
            score = getBestMove(tryout, other, depth-1, x, y, alpha, beta).second;

            delete tryout;

            if (score < bestScore) {
                bestScore = score;
                best = i;
                beta = min(bestScore, alpha);
            }

            if (alpha > beta)
                break;
        }
    }

    pair<Move, int> p (movelist[best], bestScore);
    return p;
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
     * Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    // timeLeft -= msLeft;
    Side other = (side == BLACK) ? WHITE : BLACK;

    if (opponentsMove) { // not null
        board->doMove(opponentsMove, other);
        pieceCount++;
    }

    // TODO include condition for unlimited time or otherwise
    // but for now whatever

    Move best = getBestMove(board, true, 6, -1, -1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()).first;

    int x = best.getX();
    int y = best.getY();

    Move *m = new Move(x,y);

    if (x < 0)
        return NULL;

    board->doMove(m, side);
    pieceCount++;

    return m;

}
