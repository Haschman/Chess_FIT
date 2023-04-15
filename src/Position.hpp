#ifndef POSITION_H
#define POSITION_H

#include <string>
#include <iostream>

class Position {
public:
    Position() : m_X(0), m_Y(0) {}
    Position(char x, int y);
    Position(std::pair<char, int>);

    friend bool operator == (const Position &lhs, const Position &rhs);
    friend bool operator == (const Position &lhs, std::pair<char, int> &rhs);
    friend bool operator != (const Position &lhs, const Position &rhs);
    friend bool operator != (const Position &lhs, std::pair<char, int> &rhs);
    friend std::ostream &operator << (std::ostream &os, const Position &pos);

    std::pair<char, int> getCoord() const;
    char column() const;
    int row() const;

    // bool isValid() const;

private:
    char m_X;
    int m_Y;

};

#endif
