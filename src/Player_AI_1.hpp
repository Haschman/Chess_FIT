#ifndef PLAYER_AI_1
#define PLAYER_AI_1

#include "Player.hpp"

class PlayerAI1 : public Player {
public:
    PlayerAI1(char color);

    int askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos) override;
    int askWhere(Board &board, Position &currentPos, Position &newPos) override;
};

#endif