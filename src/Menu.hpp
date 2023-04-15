#ifndef MENU_H
#define MENU_H

#include "Game.hpp"

class Menu {
public:
    Menu();
    int initialize() const;

private:
    vectorOfPlayers makePlayers() const;
    int startGame() const;

    class exit : public std::exception {};
};


#endif
