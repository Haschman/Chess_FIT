#ifndef PAWN_H
#define PAWN_H

#include "ChessFigure.hpp"

class Pawn : public ChessFigure {
public:
    Pawn(int id, char color);

    void loadMoves(const Board &board) override;

    std::string printShort() const override;
    std::string printLong() const override;

    void makeAvailableEP() override;
    void unavailableEP() override;
    bool isEnPassant() const override;

private:
    bool pawnCheck(const Position &pos, const Board &board);
    bool m_enPassant;

    class canGoEnPassantNow : public std::exception {};
};

#endif
