#ifndef BISHOP_H
#define BISHOP_H

#include "ChessFigure.hpp"

class Bishop : public ChessFigure {
public:
    Bishop(int id, char color);

    void loadMoves(const Board &board) override;

    std::string printShort() const override;
    std::string printLong() const override;

};

#endif
