#include "Queen.hpp"
#include "Board.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>


Queen::Queen(int id, char color) : ChessFigure(id, color, /*value*/ 9) {}

void Queen::loadMoves(const Board &board)
{
    Position pos = board.getCurrentPos(*this);
    m_possibleMoves.clear();
    for (int i = 1; i < 9; i++) // move in column
        m_possibleMoves.emplace_back(pos.column(), i);
    for (char c = 'a'; c <= 'h'; c++) // move in row
        m_possibleMoves.emplace_back(c, pos.row());
    for (char cD = 'a'; cD <= 'h'; cD++) // move diagonal
        for (int iD = 1; iD <= 8; iD++)
            if (cD - pos.column() == iD - pos.row() || cD - pos.column() == -(iD - pos.row()))
                m_possibleMoves.emplace_back(cD, iD);
    removeBlocked(board);
    m_reachable = m_possibleMoves;
    removeSameColor(board);
}

std::string Queen::printShort() const
{
    if (m_isAlive)
        return "Qn";
    return "  ";
}

std::string Queen::printLong() const
{
    return "Queen";
}
