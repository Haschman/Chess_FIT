#include "Board.hpp"
#include "construct_laout.hpp"
#include <algorithm>

namespace color {
    static const char *RESET = "\033[38;5;15m";
    static const char *BLACK = "\033[38;2;44;160;240m";
    static const char *RESET_BACK = "\033[48;5;0m";
    static const char *WHITE_BACK = "\033[48;5;235m";
}

Board::Board()
{
    constructLaout(m_figures);
}

vectorPosFigure Board::getFigures(char color) const
{
    vectorPosFigure oneColorFigures;
        for (const auto &x : m_figures)
            if (x.second->whatColor() == color && x.second->isAlive())
                oneColorFigures.push_back(x);
    return oneColorFigures;
}

std::shared_ptr<ChessFigure> Board::whoOccupies(char column, int row) const
{
    figureIt foundFigure = std::find_if(m_figures.begin(), m_figures.end(),
        [row, column](const std::pair<Position, std::shared_ptr<ChessFigure>> pos) {
            return pos.first == std::make_pair(column, row);
        });
    if (foundFigure == m_figures.end())
        return nullptr;
    return foundFigure->second;
}
std::shared_ptr<ChessFigure> Board::whoOccupies(Position pos) const
{
    figureIt foundFigure = std::find_if(m_figures.begin(), m_figures.end(),
        [pos](const std::pair<Position, std::shared_ptr<ChessFigure>> Fpos) {
            return Fpos.first == pos;
        });
    if (foundFigure == m_figures.end())
        return nullptr;
    return foundFigure->second;
}

Position Board::getCurrentPos(const ChessFigure &figure) const
{
    figureIt foundPos = std::find_if(m_figures.begin(), m_figures.end(),
        [&figure](const std::pair<Position, std::shared_ptr<ChessFigure>> Fpos) {
            return *(Fpos.second) == figure;
        });
    return foundPos->first;
}

int Board::isMoveValid(const Position &currentPos, const Position &newPos, ChessFigure &figure) const
{
    // remove enpassant flag
    if (currentPos == newPos) // Move is to current position
        return 1;

    // Replace isMoveValid for canGoTo and calls that in Game for selectedFigure
    if (figure.canGoTo(currentPos, newPos, *this))
        return 0;
    return 1;
}

// Pomocí pozice najde iterator a pres nej pak pozici aktualizuje
void Board::changePos(const Position &oldPos, const Position &newPos)
{
    auto itForChange = std::find_if(m_figures.begin(), m_figures.end(),
        [oldPos](const std::pair<Position, std::shared_ptr<ChessFigure>> pos) {
            return pos.first == oldPos;
        });
    itForChange->first = newPos;
}

bool Board::findIfCheckMate() const
{
    std::shared_ptr<ChessFigure> king = findIfCheck();
    if (king == nullptr)
        return false; // King is not under attack

    std::vector<Position> kingMoves = king->getMoves(*this);
    if (! kingMoves.empty())
        for (const auto move : kingMoves)
            if (! isPositionAttacked(move, king->opponentColor())) // Position is not under attack
                return false; // King can make move

    Position kingPos = getCurrentPos(*king);
    std::vector<Position> attackers = posOfAttackers(kingPos, king->opponentColor());
    if (attackers.size() > 1)
        return true; // There is more then one attacker -> attack can be blocked only from one at a time

// There is just one attacker, King cannot move anywhere and his only hope is that some lifeguard can save him
    Position attackerPos = attackers[0];
    std::vector<Position> lifeguards = posOfAttackers(attackerPos, king->whatColor());
    for (const auto guard : lifeguards)
        if (guard != kingPos)
            return false; // Attacker can be killed
// Find if someone can block the attack:
    int rowFirst = std::min(attackerPos.row(), kingPos.row());
    int rowSecond = std::max(attackerPos.row(), kingPos.row());
    char columnFirst = std::min(attackerPos.column(), kingPos.column());
    char columnSecond = std::max(attackerPos.column(), kingPos.column());

    if (columnFirst == columnSecond) { // Attacker and king are in same row
        for (int i = rowFirst + 1; i < rowSecond; i++) {
            std::vector<Position> lifeguards = whoCanGoThere({kingPos.column(), i}, king->whatColor());
            for (const auto guardPos : lifeguards)
                if (guardPos != kingPos)
                    return false; // Someone who is not king can block the attack
        }
    } else if (rowFirst == rowSecond) { // Attacker and king are in same column
        for (char c = columnFirst + 1; c < columnSecond; c++) {
            std::vector<Position> lifeguards = whoCanGoThere({c, kingPos.row()}, king->whatColor());
            for (const auto guardPos : lifeguards)
                if (guardPos != kingPos)
                    return false; // Someone who is not king can block the attack
        }
    // Diagonal check
    } else if (kingPos.column() > attackerPos.column() && kingPos.row() > attackerPos.row()) { // R -> L & Down -> Up
        int row = kingPos.row() - 1;
        for (char col = kingPos.column() - 1; col > attackerPos.column(); col--) {
            std::vector<Position> lifeguards = whoCanGoThere({col, row}, king->whatColor());
            for (const auto guardPos : lifeguards)
                if (guardPos != kingPos)
                    return false;
            row--;
        }
    } else if (kingPos.column() > attackerPos.column() && kingPos.row() < attackerPos.row()) { // R -> L & Up -> Down
        int row = kingPos.row() + 1;
        for (char col = kingPos.column() - 1; col > attackerPos.column(); col--) {
            std::vector<Position> lifeguards = whoCanGoThere({col, row}, king->whatColor());
            for (const auto guardPos : lifeguards)
                if (guardPos != kingPos)
                    return false;
            row++;
        }
    } else if (kingPos.column() < attackerPos.column() && kingPos.row() > attackerPos.row()) { // L -> R & Down -> Up
        int row = kingPos.row() - 1;
        for (char col = kingPos.column() + 1; col < attackerPos.column(); col++) {
            std::vector<Position> lifeguards = whoCanGoThere({col, row}, king->whatColor());
            for (const auto guardPos : lifeguards)
                if (guardPos != kingPos)
                    return false;
            row--;
        }
    } else if (kingPos.column() < attackerPos.column() && kingPos.row() < attackerPos.row()) { // L -> R & Up -> Down
        int row = kingPos.row() + 1;
        for (char col = kingPos.column() + 1; col < attackerPos.column(); col++) {
            std::vector<Position> lifeguards = whoCanGoThere({col, row}, king->whatColor());
            for (const auto guardPos : lifeguards)
                if (guardPos != kingPos)
                    return false;
            row++;
        }
    }
    return true; // Check Mate
}

std::shared_ptr<ChessFigure> Board::findIfCheck() const
{
    for (const auto &figure : m_figures)
        if (figure.second->isCheck(*this))
            return figure.second;
    return nullptr;
}

// Find figures that can move to given pos. Figures of same color can't go to same place -> this function doesn't tell if someone is guarding
std::vector<Position> Board::whoCanGoThere(const Position &pos, char color) const
{
    std::vector<Position> weCanGoThere;
    for (const auto figure : m_figures) {
        if (figure.second->whatColor() == color)
            if (figure.second->canGoTo(figure.first, pos, *this) != 1)
                weCanGoThere.push_back(figure.first);
    }
    return weCanGoThere;
}

// Find attackers of position and returns vector of their positions
std::vector<Position> Board::posOfAttackers(const Position &pos, char color) const
{
    std::vector<Position> attackersPos;
    for (const auto figure : m_figures) {
        if (figure.second->whatColor() == color)
            if (figure.second->canReachPos(pos, *this))
                attackersPos.push_back(figure.first);
    }
    return attackersPos;
}

// If someone can reach there
bool Board::isPositionAttacked(const Position &pos, char color) const // could be usefull for AI
{
    for (const auto figure : m_figures) {
        if (figure.second->whatColor() == color)
            if (figure.second->canReachPos(pos, *this))
                return true;
    }
    return false;
}

void Board::print(std::ostream &os) const
{
// top
    os << std::endl;
    std::vector<char> letters {'A','B','C','D','E','F','G','H'};
       os << "    ";
    for (const char i : letters)
        os << " " << i << " ";
    os << std::endl;

    int whiteSquare = 0;
    for (int row = 8; row > 0; row--) {
    // line
        os << "     ";
        for (int k = 0; k < 8; k++)
            os << "-- ";
        os << std::endl;

    // squares
        os << "  " << row << " ";
        for (char column = 'a'; column <= 'h'; column++) {
            bool found = true;
            std::shared_ptr<ChessFigure> foundFigure;
            
            foundFigure = this->whoOccupies(column, row);
            if (foundFigure == nullptr)
                found = false;

            if (found == true) {
                if (foundFigure->whatColor() == 'W') {
                    if (whiteSquare % 2 == 0)
                        os << "|" << color::WHITE_BACK << foundFigure->printShort() << color::RESET_BACK;
                    else
                        os << "|" << foundFigure->printShort();
                } else {
                    if (whiteSquare % 2 == 0)
                        os << "|" << color::BLACK << color::WHITE_BACK << foundFigure->printShort() << color::RESET << color::RESET_BACK;
                    else
                        os << "|" << color::BLACK << foundFigure->printShort() << color::RESET;
                }
            } else {
                if (whiteSquare % 2 == 0)
                    os << "|" << color::WHITE_BACK << "  " << color::RESET_BACK;
                else
                    os << "|  ";
            }
            whiteSquare += 1;
        }
        os << "| " << row << std::endl;
        whiteSquare += 1;
    }

// bottom
    os << "     ";
    for (int i = 0; i < 8; i++)
        os << "-- ";
    os << std::endl;
    os << "    ";
    for (const char i : letters)
        os << " " << i << " ";
    os << std::endl << std::endl;
}


std::ostream &operator << (std::ostream &os, const Board &board)
{
    board.print(os);
    return os;
}

