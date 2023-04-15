#include "ChessFigure.hpp"
#include "Board.hpp"
#include <algorithm>
#include <utility>

ChessFigure::ChessFigure(int id, char color, int val) : m_id(id), m_color(color), m_value(val), m_isAlive(true) {}

ChessFigure& ChessFigure::killed()
{
    m_isAlive = false;
    return *this;
}

bool ChessFigure::isAlive() const
{
    return m_isAlive;
}

void ChessFigure::makeAvailableEP()
{
    throw std::logic_error("Cannot make en passant move");
}
bool ChessFigure::isEnPassant() const
{
    throw std::logic_error("Cannot make en passant move");
}
void ChessFigure::unavailableEP()
{
    throw std::logic_error("Cannot make en passant move");
}

char ChessFigure::whatColor() const
{
    return m_color;
}

void ChessFigure::removeSameColor(const Board &board)
{
    m_possibleMoves.erase(
        std::remove_if(m_possibleMoves.begin(), m_possibleMoves.end(),
                [board, this](const Position &pos){
                    const auto fig = board.whoOccupies(pos);
                    if (fig == nullptr)
                        return false;
                    return m_color == fig->whatColor();
                }), m_possibleMoves.end()
    );
}

void ChessFigure::removeBlocked(const Board &board)
{
    Position curPos = board.getCurrentPos(*this);
    for (size_t i = 0; i < m_possibleMoves.size();) {
        bool erased = false;
        Position ctrlPos = m_possibleMoves[i];
        int firstRow = std::min(curPos.row(), ctrlPos.row());
        char firstColumn = std::min(curPos.column(), ctrlPos.column());
        int secondRow = std::max(curPos.row(), ctrlPos.row());
        char secondColumn = std::max(curPos.column(), ctrlPos.column());

        if (secondRow - firstRow == 1 || secondColumn - firstColumn == 1) {
            i++;
            continue;
        }

        if (firstRow == secondRow) { // same row
            for (char j = firstColumn + 1; j < secondColumn; j++) 
                if (board.whoOccupies(j, firstRow) != nullptr) {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    erased = true;
                    break;
                }

        } else if (firstColumn == secondColumn) { // same column
            for (int j = firstRow + 1; j < secondRow; j++)
                if (board.whoOccupies(firstColumn, j) != nullptr) {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    erased = true;
                    break;
                }

    // diagonal
        } else if (curPos.column() > ctrlPos.column() && curPos.row() > ctrlPos.row()) { // R -> L & Down -> Up
            int j = curPos.row() - 1;
            for (char k = curPos.column() - 1; k > ctrlPos.column(); k--) {
                if (board.whoOccupies(k, j) != nullptr) {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    erased = true;
                    break;
                }
                j--;
            }
        } else if (curPos.column() > ctrlPos.column() && curPos.row() < ctrlPos.row()) { // R -> L & Up -> Down
            int j = curPos.row() + 1;
            for (char k = curPos.column() - 1; k > ctrlPos.column(); k--) {
                if (board.whoOccupies(k, j) != nullptr) {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    erased = true;
                    break;
                }
                j++;
            }
        } else if (curPos.column() < ctrlPos.column() && curPos.row() > ctrlPos.row()) { // L -> R & Down -> Up
            int j = curPos.row() - 1;
            for (char k = curPos.column() + 1; k < ctrlPos.column(); k++) {
                if (board.whoOccupies(k, j) != nullptr) {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    erased = true;
                    break;
                }
                j--;
            }
        } else if (curPos.column() < ctrlPos.column() && curPos.row() < ctrlPos.row()) { // L -> R & Up -> Down
            int j = curPos.row() + 1;
            for (char k = curPos.column() + 1; k < ctrlPos.column(); k++) {
                if (board.whoOccupies(k, j) != nullptr) {
                    m_possibleMoves.erase(m_possibleMoves.begin() + i);
                    erased = true;
                    break;
                }
                j++;
            }
        }
        if (! erased)
            i++;
    }
}

int ChessFigure::canGoTo(const Position &currentPos, const Position &newPos, const Board &board)
{
    bool canGoEnPassant = false;

    try {
        loadMoves(board);
    } catch (const std::exception &e) {canGoEnPassant = true;}

    // newPos not found in available moves
    if (std::find(m_possibleMoves.begin(), m_possibleMoves.end(), newPos) == m_possibleMoves.end())
        return 1;

    // newPos is occupied so return code is for kill
    if (board.whoOccupies(newPos) != nullptr)
        return 2;

    // Make available to be killed by en passant
    if (std::abs(currentPos.row() - newPos.row()) == 2) {
        try {
            makeAvailableEP();
            return 3;
        } catch (const std::logic_error &e) {}
    }

    if (canGoEnPassant)
        return 4;

    // Normal move
    return 0;
}

// Bool if position of figure is under attack - using board.isPositionAttacked()
bool ChessFigure::canReachMe(const Board &board, char color) const
{
    Position currentPos = board.getCurrentPos(*this);
    if (board.isPositionAttacked(currentPos, color))
        return true;
    return false;
}

// Can this figure reach given pos?
bool ChessFigure::canReachPos(const Position pos, const Board &board)
{
    try {
        this->loadMoves(board);
    } catch (const std::exception &e) {}
    if (std::find(m_reachable.begin(), m_reachable.end(), pos) != m_reachable.end())
        return true;
    return false;
}

// Returns m_possibleMoves
std::vector<Position> ChessFigure::getMoves(const Board &board)
{
    try {
        this->loadMoves(board);
    } catch (const std::exception &e) {}
    return m_possibleMoves;
}

bool ChessFigure::isCheck(const Board &board) const
{
    (void)board;
    return false;
}

char ChessFigure::opponentColor() const
{
    if (m_color == 'W')
        return 'B';
    return 'W';
}

bool operator == (const ChessFigure &lhs, const ChessFigure &rhs)
{
    return lhs.m_id == rhs.m_id;
}
// Returns true if lhs is smaller value than rhs
bool operator > (const ChessFigure &lhs, const ChessFigure &rhs)
{
    return lhs.m_value > rhs.m_value;
}
bool operator < (const ChessFigure &lhs, const ChessFigure &rhs)
{
    return lhs.m_value < rhs.m_value;
}
