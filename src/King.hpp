#ifndef KING_H
#define KING_H

#include "ChessFigure.hpp"

class King : public ChessFigure {
public:
    King(int id, char color);

    void loadMoves(const Board &board) override;

    bool isCheck(const Board &board) const;

    void castling();

    std::string printShort() const override;
    std::string printLong() const override;

private:
    bool m_wasMoved;

};

#endif
