// TODO: 实现Dealer类 - 管理牌堆和弃牌堆，负责洗牌和发牌
#pragma once
#include <vector>
#include "card.h"

// 牌桌管理者：主牌堆 + 弃牌堆，负责造牌、洗牌、发牌、抽牌
class Dealer
{
public:
    Dealer();

    // 初始化完整 108 张 UNO 牌组
    void initFullDeck();

    // 洗牌
    void shuffleDeck();

    // 从主牌堆抽一张牌，牌堆为空返回空牌
    Card drawCard();

    // 批量发牌：给单个玩家发指定数量的牌
    void dealCards(Player& player, int count);

    // 将打出的牌放入弃牌堆
    void addToDiscard(const Card& card);

    // 获取当前弃牌堆顶牌（桌面当前牌）
    Card getTopDiscard() const;

    // 重置：把弃牌堆（除顶牌外）洗回主牌堆，牌堆耗尽时调用
    void recycleDiscard();

private:
    std::vector<Card> deck;        // 主牌堆
    std::vector<Card> discardPile; // 弃牌堆
};