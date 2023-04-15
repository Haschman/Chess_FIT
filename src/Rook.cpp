#include "Rook.hpp"
#include "Board.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>


Rook::Rook(int id, char color) : ChessFigure(id, color, /*value*/ 5) {}

void Rook::loadMoves(const Board &board)
{
    Position pos = board.getCurrentPos(*this);
    m_possibleMoves.clear();
    for (int i = 1; i < 9; i++) // move in column
        m_possibleMoves.emplace_back(pos.column(), i);
    for (char c = 'a'; c <= 'h'; c++) // move in row
        m_possibleMoves.emplace_back(c, pos.row());
    removeBlocked(board);
    m_reachable = m_possibleMoves;
    removeSameColor(board);
}

std::string Rook::printShort() const
{
    if (m_isAlive)
        return "Rk";
    return "  ";
}

std::string Rook::printLong() const
{
    return "Rook";
}
