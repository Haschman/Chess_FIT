#include "Bishop.hpp"
#include "Board.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>


Bishop::Bishop(int id, char color) : ChessFigure(id, color, /*value*/ 3) {}

void Bishop::loadMoves(const Board &board)
{
    Position pos = board.getCurrentPos(*this);
    m_possibleMoves.clear();
    for (char c = 'a'; c <= 'h'; c++)
        for (int i = 1; i <= 8; i++)
            if (c - pos.column() == i - pos.row() || c - pos.column() == -(i - pos.row()))
                m_possibleMoves.emplace_back(c, i);
    removeBlocked(board);
    m_reachable = m_possibleMoves;
    removeSameColor(board);
}

std::string Bishop::printShort() const
{
    if (m_isAlive)
        return "Bp";
    return "  ";
}

std::string Bishop::printLong() const
{
    return "Bishop";
}
