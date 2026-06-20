#include "ai_player.h"

AIPlayer::AIPlayer(std::string n) : Player(n) {}
AIPlayer::~AIPlayer() = default;

int AIPlayer::chooseCard(const Card& topCard) const
{
    for (int i = 0; i < hand.size(); ++i)
    {
        if (hand[i].canPlayOn(topCard))
            return i;
    }
    return -1;
}