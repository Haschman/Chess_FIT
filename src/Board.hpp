#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <memory>
#include "ChessFigure.hpp"

typedef std::vector<std::pair<Position, std::shared_ptr<ChessFigure>>> vectorPosFigure;
typedef vectorPosFigure::const_iterator figureIt;

class Board {
public:
    Board();

    std::shared_ptr<ChessFigure> whoOccupies(char column, int row) const;
    std::shared_ptr<ChessFigure> whoOccupies(Position pos) const;
    Position getCurrentPos(const ChessFigure &figure) const;

    int isMoveValid(const Position &currentPos, const Position &newPos, ChessFigure &figure) const;
    void changePos(const Position &oldPos, const Position &newPos);
    std::shared_ptr<ChessFigure> findIfCheck() const;
    bool findIfCheckMate() const;
    bool isPositionAttacked(const Position &pos, char color) const;
    std::vector<Position> whoCanGoThere(const Position &pos, char color) const;
    std::vector<Position> posOfAttackers(const Position &pos, char color) const;

    vectorPosFigure getFigures(char color) const;

    void print(std::ostream &os) const;
    friend std::ostream &operator << (std::ostream &os, const Board &board);

private:
    vectorPosFigure m_figures;
};

#endif
