#include "King.hpp"
#include "check_input.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>


King::King(int id, char color) : ChessFigure(id, color, 0), m_wasMoved(false) {}

void King::loadMoves(const Board &board)
{
    Position pos = board.getCurrentPos(*this);
    m_possibleMoves.clear();
    std::array<int, 3> diff = {-1, 0, 1};
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (isPosValid(pos.column() + diff[i], pos.row() + diff[j]))
                m_possibleMoves.emplace_back(pos.column() + diff[i], pos.row() + diff[j]);
    m_reachable = m_possibleMoves;
    removeSameColor(board);
}

bool King::isCheck(const Board &board) const
{
    if (canReachMe(board, opponentColor()))
        return true;
    return false;
}

void King::castling() {
    // Check Rooks
    // Check m_wasMoved
    // Check isUnderrAttack
    // Blocked

    // do castling
    return;
}

std::string King::printShort() const
{
    if (m_isAlive)
        return "Kg";
    return "";
}

std::string King::printLong() const
{
    return "King";
}
