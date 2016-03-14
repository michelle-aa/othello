(solo)

I attempted to vary the heuristic depending on how far the game had progressed. Not knowing too much about othello, I categorised the games as endgame being over 40 pieces on the board, and treated early- to midgame the same.

I adjusted the heuristic to attempt to account for pieces that were flipped that could never be turned back e.g. an edge piece that connects with an order piece. In the previous version of my code, I did not account for this and thus would unfairly weight scores against edge pieces.

The score calculation could vastly be improved - if I had more time to work on this I would try to count number of 'permanent' pieces and incorporate that to weight the scores. The way the multiplier is handled is also quite primitive - it cannot really compare moves that gain a good position but have a large loss in piece count to moves that have good piece counts but bad piece placement. Trying to adjust this resulted in a higher loss rate.
