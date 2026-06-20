#include "player.h"
#include <iostream>

Player::Player(std::string n) : name(n) {}
Player::~Player() = default;

void Player::drawCard(const Card& c)
{
    hand.push_back(c);
}

int Player::getHandCount() const
{
    return hand.size();
}

std::vector<Card>& Player::getHand()
{
    return hand;
}

Card Player::playCard(int idx)
{
    Card out = hand[idx];
    hand.erase(hand.begin() + idx);
    return out;
}

void Player::showHand() const
{
    for (int i = 0; i < hand.size(); ++i)
    {
        std::cout << "[" << i << "] " << hand[i].toString() << "  ";
    }
    std::cout << "\n";
}

std::string Player::getName() const
{
    return name;
}