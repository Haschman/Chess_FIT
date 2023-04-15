#include "Player_real.hpp"
#include "check_input.hpp"

PlayerReal::PlayerReal(char color) : Player(color, "Real Player " + color) {}

int PlayerReal::askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos)
{
    if (m_color == 'W') 
        std::cout << "White is playing\n\n";
    else
        std::cout << "Black is playing\n\n";

    if (board.findIfCheck() != nullptr)
        std::cout << "King in Check\n\n";

    while (true) {
        std::cout << "Position of figure you want to move: ";
        std::string command;
        if (! std::getline(std::cin, command))                                                           // Read command
            throw std::runtime_error("Fatal input error: " + std::to_string(std::cin.rdstate()) + "\n");
        if (checkInput(command, "Quit"))
            return 1;
        try {
            currentPos = validatePos(command); // returns Position !!!
        } catch (const std::invalid_argument &e) {
            std::cout << e.what();
            continue;
        }

        selectedFigure = board.whoOccupies(currentPos);                                                   // Load figure
        if (selectedFigure == nullptr) {
            std::cout << "No figure on " << currentPos << std::endl;
            continue;
        }

        if (selectedFigure->whatColor() != m_color) {
            std::cout << "This is not your figure!" << std::endl;
            continue;
        }

        std::cout << "Selected figure is " << selectedFigure->printLong() << " at " << currentPos << std::endl;
        return 0; // CurrentPos is valid
    }
}

int PlayerReal::askWhere(Board &board, Position &currentPos, Position &newPos)
{
    while (true) {
        std::cout << "For change press X" << std::endl;
        std::cout << "Where do you want to move it: ";
        std::string command;
        if (! std::getline(std::cin, command))                                                           // Read command
            throw std::runtime_error("Fatal input error: " + std::to_string(std::cin.rdstate()) + "\n");
        if (checkInput(command, "Quit"))
            return -1;
        if (checkInput(command, "X")) { // Change of selected figure
            std::cout << board;
            return -2;
        }

        try {
            newPos = validatePos(command);                                                       // Transfer to Position
        } catch (const std::invalid_argument &e) {
            std::cout << e.what();
            continue;
        }
        break;
    }

    std::shared_ptr<ChessFigure> attackedFigure;
    Position attackedPos, tmp = {'x', 0}, deathField = {0, 0};
    std::shared_ptr<ChessFigure> selectedFigure = board.whoOccupies(currentPos);
    int goToCode = selectedFigure->canGoTo(currentPos, newPos, board);

    switch (goToCode) {
    case 0: // Move figure
    case 3: // Return different code because of fucking en passant
        board.changePos(currentPos, newPos);
        if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) { // If I move with figure but my King is still in check
            board.changePos(newPos, currentPos);
            goToCode = 1;
            std::cout << "Invalid move" << std::endl;
        } else
            std::cout << std::endl << selectedFigure->printLong() << " moved to " << newPos << std::endl;
        return goToCode;
    case 2: // Kill figure
        board.changePos(newPos, tmp); // Tmp pos for killed figure
        board.changePos(currentPos, newPos);
        if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) {
            board.changePos(newPos, currentPos);
            board.changePos(tmp, newPos);
            goToCode = 1;
            std::cout << "Invalid move" << std::endl;
        } else {
            attackedFigure = board.whoOccupies(tmp);
            attackedFigure->killed();
            std::cout << std::endl << selectedFigure->printLong() << " killed " << attackedFigure->printLong()
                        << " at " << newPos << std::endl;
            board.changePos(tmp, deathField); // Move to death field -----nutné zobecnit a odlišit pro barvy-----
        }
        return goToCode;
    case 4: // Kill en passant
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
            std::cout << "Invalid move" << std::endl;
        } else {
            attackedFigure->killed();
            std::cout << std::endl << selectedFigure->printLong() << " killed " << attackedFigure->printLong()
                        << " at " << attackedPos << " en passant" << std::endl;
            board.changePos(tmp, deathField); // attacked figure goes to Death field
        }
        return goToCode;
    default:
        std::cout << "Invalid move" << std::endl;
        return goToCode;
    }
}
