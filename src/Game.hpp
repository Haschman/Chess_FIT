#ifndef GAME_H
#define GAME_H

#include <string>
#include "Board.hpp"
#include "Player.hpp"

typedef std::vector<std::unique_ptr<Player>> vectorOfPlayers;

class Game {
public:
    Game(vectorOfPlayers players);
    int play();

private:
    vectorOfPlayers m_players;
};

// void endGame();


#endif