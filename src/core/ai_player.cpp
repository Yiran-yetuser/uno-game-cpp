// TODO: 实现AIPlayer类方法 - chooseCard随机选择合法卡牌
#include "judger.h"
#include "ai_player.h"
#include <random>
#include <ctime>

AIPlayer::AIPlayer(std::string playerName)
    : Player(std::move(playerName))
{
}

int AIPlayer::chooseCard(const Card& topDiscard)
{
    const auto& hand = getHand();
    int cardCount = static_cast<int>(hand.size());
    if (cardCount == 0)
    {
        return -1;
    }

    // 临时存储所有合法可出的卡牌下标
    std::vector<int> validIndex;

    // 先筛选出手牌中所有符合规则的牌
    // 临时简易判断逻辑（后续Judger类完成后再统一替换）
    for (int i = 0; i < cardCount; ++i)
    {
        const Card& card = hand[i];
        if (Judger::canPlayCard(card, topDiscard))
        {
            validIndex.push_back(i);
        }
    }

    // 没有合法牌，返回-1
    if (validIndex.empty())
    {
        return -1;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validIndex.size() - 1);
    int randPos = dis(gen);
    return validIndex[randPos];
}