#include "Player_AI_1.hpp"
#include <random>

PlayerAI1::PlayerAI1(char color) : Player(color, "AI Player " + color) {}

int PlayerAI1::askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos)
{
    vectorPosFigure figures = board.getFigures(m_color);
    bool figureCannotMove = true;
    while(figureCannotMove) {
        posFigure selected = figures[randomIndex(figures.size() - 1)];
        currentPos = selected.first;
        selectedFigure = selected.second;
        if (! selectedFigure->getMoves(board).empty()) // Vector of possible moves is not empty
            figureCannotMove = false;
    }
    return 0;
}

int PlayerAI1::askWhere(Board &board, Position &currentPos, Position &newPos)
{
    std::shared_ptr<ChessFigure> selectedFigure = board.whoOccupies(currentPos);
    std::vector<Position> possibleMoves = selectedFigure->getMoves(board);
    std::shared_ptr<ChessFigure> attackedFigure;
    Position attackedPos, tmp = {'x', 0}, deathField = {0, 0};

    int goToCode = 1;
    while (goToCode == 1) {
        if (possibleMoves.size() == 0)
            return -2;
        int randPos = randomIndex(possibleMoves.size() - 1);
        newPos = possibleMoves[randPos];
        possibleMoves.erase(possibleMoves.begin() + randPos);

        goToCode = selectedFigure->canGoTo(currentPos, newPos, board);
        switch (goToCode) {
        case 0:
        case 3:
            board.changePos(currentPos, newPos);
            if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) {
                board.changePos(newPos, currentPos);
                goToCode = 1;
            } else
                std::cout << std::endl << selectedFigure->printLong() << " moved to " << newPos << std::endl;
            break;
        case 2:
            board.changePos(newPos, tmp); // Tmp pos for killed figure
            board.changePos(currentPos, newPos);
            if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) {
                board.changePos(newPos, currentPos);
                board.changePos(tmp, newPos);
                goToCode = 1;
            } else {
                attackedFigure = board.whoOccupies(tmp);
                attackedFigure->killed();
                std::cout << std::endl << selectedFigure->printLong() << " killed " << attackedFigure->printLong()
                            << " at " << newPos << std::endl;
                board.changePos(tmp, deathField); // Move to death field -----nutné zobecnit a odlišit pro barvy-----
            }
            break;
        case 4:
            if (selectedFigure->whatColor() == 'W') {
                attackedPos = {newPos.column(), newPos.row() - 1};
                attackedFigure = board.whoOccupies(attackedPos);
            } else {
                attackedPos = {newPos.column(), newPos.row() + 1};
                attackedFigure = board.whoOccupies(attackedPos);
            }
            board.changePos(attackedPos, tmp); // attacked figure goes to tmp pos
            board.changePos(currentPos, newPos);
            if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) {
                board.changePos(newPos, currentPos); // selected figure goes back to starting pos
                board.changePos(tmp, attackedPos); // attacked figure is back from tmp pos
                goToCode = 1; // code for invalid move
            } else {
                attackedFigure->killed();
                std::cout << std::endl << selectedFigure->printLong() << " killed " << attackedFigure->printLong()
                            << " at " << attackedPos << " en passant" << std::endl;
                board.changePos(tmp, deathField); // attacked figure goes to Death field
            }
            break;
        default:
            break;
        }
    }
    return goToCode;
}
