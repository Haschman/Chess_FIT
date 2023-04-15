#ifndef CHESSFIGURE_H
#define CHESSFIGURE_H

#include "Position.hpp"
#include <utility>
#include <string>
#include <vector>

class Board;

class ChessFigure {
public:
    ChessFigure(int id, char color, int val);
    virtual ~ChessFigure() = default;

    bool isAlive() const;

    int canGoTo(const Position &currentPos, const Position &newPos, const Board &board);

    virtual void loadMoves(const Board &board) = 0;
    std::vector<Position> getMoves(const Board &board);

    char whatColor() const;
    char opponentColor() const;

    ChessFigure& killed();

    virtual bool isCheck(const Board &board) const;
    bool canReachMe(const Board &board, char color) const;
    bool canReachPos(const Position pos, const Board &board);

    virtual std::string printShort() const = 0;
    virtual std::string printLong() const = 0;

    virtual void makeAvailableEP();
    virtual void unavailableEP();
    virtual bool isEnPassant() const;

    class canMoveEnPassant : public std::exception {};

    friend bool operator == (const ChessFigure &lhs, const ChessFigure &rhs);
    friend bool operator > (const ChessFigure &lhs, const ChessFigure &rhs);
    friend bool operator < (const ChessFigure &lhs, const ChessFigure &rhs);

protected:
    int m_id;
    char m_color;
    int m_value;
    bool m_isAlive;
    std::vector<Position> m_possibleMoves;
    std::vector<Position> m_reachable;

    void removeSameColor(const Board &board);
    void removeBlocked(const Board &board);
};

#endif
