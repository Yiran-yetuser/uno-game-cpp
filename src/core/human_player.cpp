// TODO: 实现HumanPlayer类方法 - setSelectedCard、chooseCard
#include "human_player.h"
#include <iostream>

HumanPlayer::HumanPlayer(std::string playerName)
    : Player(std::move(playerName))
{
}

void HumanPlayer::setSelectedCard(int idx)
{
    // 简单边界校验
    if (idx >= -1 && idx < static_cast<int>(hand.size()))
    {
        selectedIdx = idx;
    }
}

int HumanPlayer::chooseCard(const Card& topDiscard)
{
    // 控制台交互逻辑：打印手牌 + 引导玩家输入下标
    std::cout << "\n===== " << getName() << " 的手牌 =====" << std::endl;
    const auto& handCards = getHand();
    for (int i = 0; i < handCards.size(); ++i)
    {
        std::cout << "[" << i << "] " << handCards[i].toString() << std::endl;
    }

    int input = -1;
    std::cout << "请输入要打出的卡牌下标（无牌可出输入-1）：";
    std::cin >> input;

    // 调用自身方法记录选中下标
    setSelectedCard(input);
    return selectedIdx;
}