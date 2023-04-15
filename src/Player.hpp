#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.hpp"

typedef std::pair<Position, std::shared_ptr<ChessFigure>> posFigure;

class Player {
public:
    Player(char color, std::string name);
    virtual int askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos) = 0;
    virtual int askWhere(Board &board, Position &currentPos, Position &newPos) = 0;

    char whatColor() const;
    char opponentColor() const;
    size_t randomIndex(size_t last) const;

protected:
    char m_color;
    std::string m_name;
};

#endif
