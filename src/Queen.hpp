#ifndef QUEEN_H
#define QUEEN_H

#include "ChessFigure.hpp"

class Queen : public ChessFigure {
public:
    Queen(int id, char color);

    void loadMoves(const Board &board) override;

    std::string printShort() const override;
    std::string printLong() const override;

};

#endif
