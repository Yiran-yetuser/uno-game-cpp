// deck.h 实现牌组，108张牌的生成、洗牌、抽牌
#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <vector>

class Deck
{
private:
    std::vector<Card> drawPile;    // 抽牌堆
    std::vector<Card> discardPile; // 弃牌堆
public:
    Deck();
    ~Deck();
    // 生成标准108张UNO牌
    void buildFullDeck();
    // 洗牌
    void shuffle(std::vector<Card> &pile);
    // 抽一张牌，牌堆空返回空万能牌
    Card draw();
    // 重新洗牌弃牌堆到抽牌堆
    void reshuffleDiscard();
    // 牌堆是否为空
    bool empty() const;
    // 将牌放入弃牌堆
    void addDiscard(const Card &card);
    // 获取弃牌堆顶的牌
    Card topCard() const;
};
#endif