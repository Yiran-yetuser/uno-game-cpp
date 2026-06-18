// TODO: 实现Player基类方法 - addCard、playCard、hasValidMove等
#include "player.h"
#include "judger.h"

Player::Player(std::string playerName)
    : name(std::move(playerName))
{
}

std::string Player::getName() const
{
    return name;
}

void Player::addCard(const Card& card)
{
    hand.push_back(card);
}

Card Player::playCard(int handIndex)
{
    // 越界保护
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size()))
    {
        return Card(CardColor::BLACK, CardType::NUMBER, 0);
    }

    Card selected = hand[handIndex];
    hand.erase(hand.begin() + handIndex);
    return selected;
}

const std::vector<Card>& Player::getHand() const
{
    return hand;
}

bool Player::hasHandCard() const
{
    return !hand.empty();
}


bool Player::hasValidPlay(const Card& topDiscard) const
{
    const auto& hand = getHand();
    for (const auto& card : hand)
    {
        if (Judger::canPlayCard(card, topDiscard))
        {
            return true;
        }
    }
    return false;
}