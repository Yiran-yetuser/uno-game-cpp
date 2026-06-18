// TODO: 实现AIPlayer类 - AI玩家自动选择出牌策略
#pragma once
#include "player.h"

// AI玩家类，继承自Player基类
class AIPlayer : public Player
{
public:
    explicit AIPlayer(std::string playerName);

    // 重写选牌：随机选择一张合法卡牌，无合法牌返回-1
    int chooseCard(const Card &topDiscard) override;
};