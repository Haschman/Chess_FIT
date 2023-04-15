#ifndef CONSTRUCT_LAYOUT_H
#define CONSTRUCT_LAYOUT_H

#include <vector>
#include <memory>
#include "ChessFigure.hpp"
#include "King.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Pawn.hpp"

typedef std::vector<std::pair<Position, std::shared_ptr<ChessFigure>>> vectorPosFigure;

void constructLaout(vectorPosFigure &figures) {
    int id = 0;

    figures.push_back({Position('e', 8), std::make_shared<King>(id++, 'B')});
    figures.push_back({Position('e', 1), std::make_shared<King>(id++, 'W')});

    figures.push_back({Position('d', 8), std::make_shared<Queen>(id++, 'B')});
    figures.push_back({Position('d', 1), std::make_shared<Queen>(id++, 'W')});

    figures.push_back({Position('a', 8), std::make_shared<Rook>(id++, 'B')});
    figures.push_back({Position('h', 8), std::make_shared<Rook>(id++, 'B')});
    figures.push_back({Position('a', 1), std::make_shared<Rook>(id++, 'W')});
    figures.push_back({Position('h', 1), std::make_shared<Rook>(id++, 'W')});

    figures.push_back({Position('b', 8), std::make_shared<Knight>(id++, 'B')});
    figures.push_back({Position('g', 8), std::make_shared<Knight>(id++, 'B')});
    figures.push_back({Position('b', 1), std::make_shared<Knight>(id++, 'W')});
    figures.push_back({Position('g', 1), std::make_shared<Knight>(id++, 'W')});

    figures.push_back({Position('c', 8), std::make_shared<Bishop>(id++, 'B')});
    figures.push_back({Position('f', 8), std::make_shared<Bishop>(id++, 'B')});
    figures.push_back({Position('c', 1), std::make_shared<Bishop>(id++, 'W')});
    figures.push_back({Position('f', 1), std::make_shared<Bishop>(id++, 'W')});

    for (char i = 'a'; i != 'i'; i++)
        figures.push_back({Position(i, 2), std::make_shared<Pawn>(id++, 'W')});
    for (char i = 'a'; i != 'i'; i++)
        figures.push_back({Position(i, 7), std::make_shared<Pawn>(id++, 'B')});
}

#endif
