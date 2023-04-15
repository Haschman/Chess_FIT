#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessFigure.hpp"

class Knight : public ChessFigure {
public:
    Knight(int id, char color);

    void loadMoves(const Board &board) override;

    std::string printShort() const override;
    std::string printLong() const override;

};

#endif