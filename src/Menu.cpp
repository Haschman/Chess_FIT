#include "Menu.hpp"
#include "check_input.hpp"
#include "Player_real.hpp"
#include "Player_AI_1.hpp"
#include "Player_AI_2.hpp"
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>

Menu::Menu() {}

int Menu::initialize() const
{
    bool inputError = false;
    bool exitGame = false;
    std::string input;
    do {
        if (inputError) {
            std::cout << "\n\n " << std::setw(26) << std::setfill('-') << "" << std::endl;
            std::cout << "| Wrong, please try again. |\n";
            std::cout << " " << std::setw(26) << std::setfill('-') << "" << std::endl;
        }
        inputError = false;

        std::cout << "\nEnter one of the options: \n";
        std::cout << "  For play press P\n";
        std::cout << "  For exit press Q\n";

        if (! std::getline(std::cin, input))
            throw std::runtime_error("Fatal input error: " + std::to_string(std::cin.rdstate()) + "\n");

        if (checkInput(input, "Play")) {
            // exception should go to main
            if (startGame() == 0) { // From game back to menu
                std::cout << "Press Enter to return to menu ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
            exitGame = true;

        } else if (checkInput(input, "Quit"))
            return 0;

        else
            inputError = true;

    } while (inputError || exitGame);

    // Unexpected behaviour
    return 1;
}

vectorOfPlayers Menu::makePlayers() const
{
    bool inputError = false;
    std::string input;
    do {
        std::cout << "\n\n" << "Who is playing?" << std::endl;
        std::cout << "  For two players press               -1-" << std::endl;
        std::cout << "  For play against AI level 1 press   -2-" << std::endl;
        std::cout << "  For play against AI level 2 press   -3-" << std::endl;
        std::cout << "  For exit press Q" << std::endl << std::endl;

        if (! std::getline(std::cin, input))
            throw std::runtime_error("Fatal input error: " + std::to_string(std::cin.rdstate()) + "\n");
        if (checkInput(input, "Quit"))
            throw exit();
        if (checkInput(input, "1")) {
            vectorOfPlayers tmp;
            tmp.emplace_back(std::make_unique<PlayerReal>('W'));
            tmp.emplace_back(std::make_unique<PlayerReal>('B'));
            return tmp;
        }
        if (checkInput(input, "2")) {
            vectorOfPlayers tmp;
            tmp.emplace_back(std::make_unique<PlayerReal>('W'));
            tmp.emplace_back(std::make_unique<PlayerAI1>('B'));
            return tmp;
        }
        if (checkInput(input, "3")) {
            vectorOfPlayers tmp;
            tmp.emplace_back(std::make_unique<PlayerReal>('W'));
            tmp.emplace_back(std::make_unique<PlayerAI2>('B'));
            return tmp;
        }
        inputError = true;

    } while (inputError);
    throw std::runtime_error("makePlayers unreachable return.");
}

int Menu::startGame() const
{
    vectorOfPlayers players;
    try {
        players = makePlayers();
    } catch (const exit &e) {return 1;}
    Game game(std::move(players));
    return game.play();
}