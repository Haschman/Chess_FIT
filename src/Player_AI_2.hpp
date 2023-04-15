#ifndef PLAYER_AI_2
#define PLAYER_AI_2

#include "Player.hpp"

class PlayerAI2 : public Player {
public:
    PlayerAI2(char color);

    int askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos) override;
    int askWhere(Board &board, Position &currentPos, Position &newPos) override;

private:
    vectorPosFigure m_canKillWith;
    std::shared_ptr<ChessFigure> m_toAttack;
    bool m_wasAsked;

    bool canKill(const Board &board, const posFigure &opponent, std::shared_ptr<ChessFigure> &selectedFigure);
    void removeUsed(const std::shared_ptr<ChessFigure> &figure);
};

#endif