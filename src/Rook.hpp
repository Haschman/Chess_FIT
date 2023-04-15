#ifndef ROOK_H
#define ROOK_H

#include "ChessFigure.hpp"

class Rook : public ChessFigure {
public:
    Rook(int id, char color);

    void loadMoves(const Board &board) override;

    std::string printShort() const override;
    std::string printLong() const override;

};

#endif
