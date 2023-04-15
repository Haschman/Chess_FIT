#include "Player_AI_2.hpp"
#include <algorithm>

PlayerAI2::PlayerAI2(char color) : Player(color, "AI Player " + color), m_toAttack(nullptr), m_wasAsked(false) {}

bool PlayerAI2::canKill(const Board &board, const posFigure &opponent, std::shared_ptr<ChessFigure> &selectedFigure)
{
    figureIt killer = std::find_if(m_canKillWith.begin(), m_canKillWith.end(), [opponent, board](const posFigure &my) {
        std::vector<Position> myMoves = my.second->getMoves(board);
        for (const Position &pos : myMoves)
            if (pos == opponent.first)
                return true;
        return false;
    });
    if (killer != m_canKillWith.end()) {
        selectedFigure = killer->second;
        return true;
    }
    return false;
}

void PlayerAI2::removeUsed(const std::shared_ptr<ChessFigure> &figure)
{
    m_canKillWith.erase(std::remove_if(m_canKillWith.begin(), m_canKillWith.end(), [&figure](const posFigure &a) {
        return *a.second == *figure;
    }), m_canKillWith.end());
}


int PlayerAI2::askWhat(Board &board, std::shared_ptr<ChessFigure> &selectedFigure, Position &currentPos)
{
    m_toAttack = nullptr;
    if (m_canKillWith.empty() && ! m_wasAsked)
        m_canKillWith = board.getFigures(m_color);
    m_wasAsked = true;
    // Erase figures which I cannot move with
    for (size_t i = 0; i < m_canKillWith.size();) {
        if (m_canKillWith[i].second->getMoves(board).empty())
            m_canKillWith.erase(m_canKillWith.begin() + i);
        else
            i++;
    }

    if (! m_canKillWith.empty()) {
        vectorPosFigure opponentFigures = board.getFigures(opponentColor()), opponentUnderAttack;

        for (const auto &figure : opponentFigures)
            if (figure.second->canReachMe(board, m_color))
                opponentUnderAttack.push_back(figure); // Figures that I can kill

        if (! opponentUnderAttack.empty()) {
            std::sort(opponentUnderAttack.begin(), opponentUnderAttack.end(),
                [](const posFigure &a, const posFigure &b) {
                    return *a.second > *b.second;
                });
            for (const auto opponent : opponentUnderAttack) {
                if (canKill(board, opponent, selectedFigure)) {
                    currentPos = board.getCurrentPos(*selectedFigure);
                    m_toAttack = opponent.second;
                    removeUsed(selectedFigure);
                    return 0;
                }
            }
        }
    }
    // None of opponent figures is under attack or I can kill him without having king in check -> select one by random
    vectorPosFigure myFigures = board.getFigures(m_color);
    posFigure selected = myFigures[randomIndex(myFigures.size() - 1)];
    currentPos = selected.first;
    selectedFigure = selected.second;
    removeUsed(selectedFigure);
    return 0;
}

int PlayerAI2::askWhere(Board &board, Position &currentPos, Position &newPos)
{
    std::shared_ptr<ChessFigure> selectedFigure = board.whoOccupies(currentPos);
    std::vector<Position> possibleMoves = selectedFigure->getMoves(board);
    std::shared_ptr<ChessFigure> attackedFigure;
    Position attackedPos, tmp = {'x', 0}, deathField = {0, 0};

    int goToCode = 1;
    while (goToCode == 1) {
        if (m_toAttack != nullptr)
            newPos = board.getCurrentPos(*m_toAttack);
        else {
            if (possibleMoves.size() == 0)
                return -2;
            int randPos = randomIndex(possibleMoves.size() - 1);
            newPos = possibleMoves[randPos];
            possibleMoves.erase(possibleMoves.begin() + randPos);
        }

        goToCode = selectedFigure->canGoTo(currentPos, newPos, board);
        switch (goToCode) {
        case 0:
        case 3:
            board.changePos(currentPos, newPos);
            if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) {
                board.changePos(newPos, currentPos);
                goToCode = -2;
            } else {
                std::cout << std::endl << selectedFigure->printLong() << " moved to " << newPos << std::endl;
                m_wasAsked = false;
                m_canKillWith.clear();
            }
            break;
        case 2:
            board.changePos(newPos, tmp); // Tmp pos for killed figure
            board.changePos(currentPos, newPos);
            if (board.findIfCheck() != nullptr && board.findIfCheck()->whatColor() == selectedFigure->whatColor()) {
                board.changePos(newPos, currentPos);
                board.changePos(tmp, newPos);
                goToCode = -2;
            } else {
                attackedFigure = board.whoOccupies(tmp);
                attackedFigure->killed();
                std::cout << std::endl << selectedFigure->printLong() << " killed " << attackedFigure->printLong()
                            << " at " << newPos << std::endl;
                board.changePos(tmp, deathField); // Move to death field -----nutné zobecnit a odlišit pro barvy-----
                m_wasAsked = false;
                m_canKillWith.clear();
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
                goToCode = -2; // code for invalid move
            } else {
                attackedFigure->killed();
                std::cout << std::endl << selectedFigure->printLong() << " killed " << attackedFigure->printLong()
                            << " at " << attackedPos << " en passant" << std::endl;
                board.changePos(tmp, deathField); // attacked figure goes to Death field
                m_wasAsked = false;
                m_canKillWith.clear();
            }
            break;
        default:
            break;
        }
    }
    return goToCode;
}
