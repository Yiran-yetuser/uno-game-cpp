// TODO: 实现Player基类 - 玩家名称、手牌管理、出牌接口
#pragma once
#include <string>
#include <vector>
#include "card.h"

// 玩家基类：纯业务层，无任何界面代码，控制台/图形界面通用
class Player
{
public:
    explicit Player(std::string playerName);
    virtual ~Player() = default;

    // 基础信息
    std::string getName() const;

    // 手牌操作（发牌、摸牌）
    void addCard(const Card& card);

    // 打出指定索引的牌，返回被打出的牌
    Card playCard(int handIndex);

    // 获取手牌列表（只读，供界面/规则查询）
    const std::vector<Card>& getHand() const;

    // 是否还有手牌
    bool hasHandCard() const;

    // 判断：当前手牌是否有能出的牌（依赖桌面顶牌，规则判断）
    virtual bool hasValidPlay(const Card& topDiscard) const;

    /**
     * 纯虚函数：选择要打出的牌
     * @param topDiscard 桌面上当前顶牌
     * @return 选中手牌下标，无合法牌返回 -1
     * 【关键点】只做逻辑决策，不包含界面交互
     */
    virtual int chooseCard(const Card& topDiscard) = 0;

protected:
    std::string name;
    std::vector<Card> hand;  // 手牌
};