#include "Pawn.hpp"
#include "Board.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>


Pawn::Pawn(int id, char color) : ChessFigure(id, color, /*value*/ 1), m_enPassant(false) {}

bool Pawn::pawnCheck(const Position &pos, const Board &board)
{
    bool canGoEnPassant = false;
    for (size_t i = 0; i < m_possibleMoves.size();) {
        std::shared_ptr<ChessFigure> figureOnNewPos = board.whoOccupies(m_possibleMoves[i]);
        std::shared_ptr<ChessFigure> figureOnSide = board.whoOccupies(m_possibleMoves[i].column(), pos.row());

        if (pos.column() == m_possibleMoves[i].column()) {
            if (figureOnNewPos != nullptr) { // Pawn can't kill in normal move
                m_possibleMoves.erase(m_possibleMoves.begin() + i);
                continue; // inc of i by erase
            }
        } else if (figureOnNewPos == nullptr && figureOnSide != nullptr) { // Check en Passant move
            try {
                if (figureOnSide->isEnPassant())
                    canGoEnPassant = true;
                else {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    continue;
                }
            } catch (const std::logic_error &e) {
                m_possibleMoves.erase(m_possibleMoves.begin() + i);
                continue;
            }
        } else if (figureOnNewPos == nullptr) { // Pawn can't move diagonal without kill
            m_possibleMoves.erase(m_possibleMoves.begin() + i);
            continue;
        }
        i++;
    }
    return canGoEnPassant;
}

void Pawn::loadMoves(const Board &board)
{
    Position pos = board.getCurrentPos(*this);
    char column = pos.column();
    int row = pos.row();
    m_possibleMoves.clear();
    m_reachable.clear();
    if (m_color == 'W') {
        if (row == 2) {
            m_possibleMoves.emplace_back(column, row + 2);
        }
        m_possibleMoves.emplace_back(column - 1, row + 1);
        m_reachable.emplace_back(column - 1, row + 1);
        m_possibleMoves.emplace_back(column,     row + 1);
        m_possibleMoves.emplace_back(column + 1, row + 1);
        m_reachable.emplace_back(column + 1, row + 1);
    } else if (m_color == 'B') {
        if (row == 7) {
            m_possibleMoves.emplace_back(column, row - 2);
        }
        m_possibleMoves.emplace_back(column - 1, row - 1);
        m_reachable.emplace_back(column - 1, row - 1);
        m_possibleMoves.emplace_back(column,     row - 1);
        m_possibleMoves.emplace_back(column + 1, row - 1);
        m_reachable.emplace_back(column + 1, row - 1);
    }
    removeSameColor(board);
    removeBlocked(board);
    if (pawnCheck(pos, board))
        throw canGoEnPassantNow();
}

std::string Pawn::printShort() const
{
    if (m_isAlive)
        return "Pn";
    return "  ";
}

std::string Pawn::printLong() const
{
    return "Pawn";
}

void Pawn::makeAvailableEP()
{
    m_enPassant = true;
}
void Pawn::unavailableEP()
{
    m_enPassant = false;
}

bool Pawn::isEnPassant() const
{
    return m_enPassant;
}
