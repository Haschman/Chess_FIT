#ifndef PLAYER_REAL
#define PLAYER_REAL

#include "Player.hpp"

class PlayerReal : public Player {
public:
    PlayerReal(char color);

    int askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos) override;
    int askWhere(Board &board, Position &currentPos, Position &newPos) override;
};

#endif
