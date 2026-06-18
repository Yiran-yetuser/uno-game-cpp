// TODO: 实现Round类 - 管理单回合游戏流程、玩家轮次、特殊牌效果
#pragma once
#include <vector>
#include "dealer.h"
#include "player.h"

// 单回合管理类：轮次、出牌顺序、特殊牌效果
class Round
{
public:
    Round(std::vector<Player*>& players, Dealer& dealer);

    // 回合初始化：开局发初始手牌
    void start();

    // 执行一轮完整出牌流程
    void playTurn();

    // 判断当前玩家是否获胜（手牌为空）
    bool checkWin() const;

private:
    // 处理特殊卡牌效果
    void handleSpecialCard(const Card& card);

    std::vector<Player*>& playerList;  // 所有玩家列表
    Dealer& tableDealer;               // 牌桌管理器
    int currentPlayerIndex;            // 当前行动玩家下标
    bool isReverse;                    // 出牌方向：true=反转，false=正序
    int skipCount;                     // 跳过回合计数
};