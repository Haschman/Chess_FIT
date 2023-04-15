#include "Game.hpp"
#include "Board.hpp"
#include "Pawn.hpp"
#include "check_input.hpp"
#include "Player.hpp"
#include "Player_real.hpp"

Game::Game(vectorOfPlayers players) : m_players(std::move(players)) {}

int Game::play()
{
    Board board;
    std::shared_ptr<ChessFigure> selectedFigure;
    std::shared_ptr<ChessFigure> enPassantFigure;
    Position currentPos, newPos;
    int i = 0;

    std::cout << board;

    while (! board.findIfCheckMate()) {

        int askWhat = m_players[i]->askWhat(board, selectedFigure, currentPos);
        if (askWhat == 1) // Called exit
            return 1;

        int askWhere = 1;
        do {
            askWhere = m_players[i]->askWhere(board, currentPos, newPos);
            if (askWhere == -1) // Called exit
                return 1;
            if (askWhere == -2) { // Change figure
                break;
            }
            if (askWhere == 3) { // Someone is available to en passant
                if (enPassantFigure != nullptr)
                    enPassantFigure->unavailableEP();
                enPassantFigure = selectedFigure;
            }
        } while (askWhere == 1);

        if (askWhere != 1 && askWhere != -2) {
            i = abs(i - 1);
            if (enPassantFigure != nullptr && enPassantFigure->whatColor() == m_players[i]->whatColor()) {
                enPassantFigure->unavailableEP();
                enPassantFigure = nullptr;
            }
            std::cout << board;
        }
    }
    std::cout << m_players[abs(i - 1)]->whatColor() << " PLAYER WON!\n\n";
    return 0;
}
