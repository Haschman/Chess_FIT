#include "check_input.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>

std::string makeLower(std::string &str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}

bool checkInput(std::string &input, std::string word)
{
    input = makeLower(input);
    word = makeLower(word);
    if (input.length() == 1 && input[0] == word[0]) {
        return true;
    } else if (input == word)
        return true;
    return false;
}

std::string &removeSpace(std::string &str)
{
    str.erase(remove_if(str.begin(), str.end(), [](const char x){return isspace(x);}), str.end());
    return str;
}

bool isPosValid(char c, int i)
{
    if (c >= 'a' && c <= 'h' && i > 0 && i < 9)
        return true;
    return false;
}

// Presunout do Position
Position validatePos(std::string &input)
{
    if (removeSpace(input).length() != 2)
        throw std::invalid_argument("Invalid position\n");
    makeLower(input);

    if (isdigit(input[0]) && isalpha(input[1])) { // First char is number, second is alphabetic
        int row = input[0] - '0';
        char column = input[1];
        if (isPosValid(column, row))
            return Position(column, row);
        throw std::invalid_argument("Invalid position\n");

    } else if (isalpha(input[0]) && isdigit(input[1])) { // First char is alphabetic and second is number
        char column = input[0];
        int row = input[1] - '0';
        if (isPosValid(column, row))
            return Position(column, row);
        throw std::invalid_argument("Invalid position\n");

    } else
        throw std::invalid_argument("Invalid position\n");
}

// Presunout do Board
std::shared_ptr<ChessFigure> loadFigure(std::string &command, const Board &board) {
    Position inputPos;
    inputPos = validatePos(command); // Check command and make it to right format 'a5'

    char column = inputPos.getCoord().first;
    int row = inputPos.getCoord().second;

    return board.whoOccupies(column, row);
}
