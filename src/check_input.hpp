#ifndef CHECK_INPUT_H
#define CHECK_INPUT_H

#include <string>
#include "Position.hpp"
#include "Board.hpp"

std::string makeLower(std::string &str);
bool checkInput(std::string &input, std::string word);
bool isPosValid(char c, int i);
Position validatePos(std::string &input);
std::string &removeSpace(std::string &str);
std::shared_ptr<ChessFigure> loadFigure(std::string &command, const Board &board);

#endif
