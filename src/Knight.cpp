#include "Knight.hpp"
#include "Board.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <array>


Knight::Knight(int id, char color) : ChessFigure(id, color, /*value*/ 3) {}

void Knight::loadMoves(const Board &board)
{
    Position pos = board.getCurrentPos(*this);
    m_possibleMoves.clear();
    std::array<int, 8> columnDiff = {-2, -1, 1, 2,  2,  1, -1, -2};
    std::array<int, 8> rowDiff =    { 1,  2, 2, 1, -1, -2, -2, -1};
    for (int i = 0; i < 8; i++) {
        char newColumn = pos.column() + columnDiff[i];
        int newRow = pos.row() + rowDiff[i];
        if (newColumn >= 'a' && newRow > 0 && newColumn <= 'h' && newRow < 9)
            m_possibleMoves.emplace_back(newColumn, newRow);
    }
    m_reachable = m_possibleMoves;
    removeSameColor(board);
}

std::string Knight::printShort() const
{
    if (m_isAlive)
        return "Kt";
    return "  ";
}

std::string Knight::printLong() const
{
    return "Knight";
}
