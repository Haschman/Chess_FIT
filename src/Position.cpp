#include "Position.hpp"
#include <utility>

Position::Position(char x, int y) : m_X(x), m_Y(y) {}
Position::Position(std::pair<char, int> pos) : m_X(pos.first), m_Y(pos.second) {}

bool operator == (const Position &lhs, const Position &rhs)
{
    return lhs.m_X == rhs.m_X && lhs.m_Y == rhs.m_Y;
}
bool operator == (const Position &lhs, std::pair<char, int> &rhs)
{
    return lhs.m_X == rhs.first && lhs.m_Y == rhs.second;
}
bool operator != (const Position &lhs, const Position &rhs)
{
    return !(lhs == rhs);
}
bool operator != (const Position &lhs, std::pair<char, int> &rhs)
{
    return !(lhs == rhs);
}

std::pair<char, int> Position::getCoord() const
{
    return {m_X, m_Y};
}

char Position::column() const
{
    return m_X;
}
int Position::row() const
{
    return m_Y;
}

/*
bool Position::isValid() const
{
    if (m_X >= 'a' && m_X <= 'h' && m_Y > 0 && m_Y < 9)
        return true;
    return false;
}
*/

std::ostream &operator << (std::ostream &os, const Position &pos)
{
    return os << pos.m_X << pos.m_Y;
}
