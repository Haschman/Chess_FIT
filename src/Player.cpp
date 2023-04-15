#include "Player.hpp"
#include <random>

Player::Player(char color, std::string name) : m_color(color), m_name(name) {}

char Player::whatColor() const
{
    return m_color;
}

char Player::opponentColor() const
{
    if (m_color == 'W')
        return 'B';
    return 'W';
}

size_t Player::randomIndex(size_t last) const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomPos(0, last);

    return randomPos(gen);
}