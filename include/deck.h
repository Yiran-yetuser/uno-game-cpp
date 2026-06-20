// deck.h 实现牌组，108张牌的生成、洗牌、抽牌
#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <vector>

class Deck
{
private:
    std::vector<Card> cards;
public:
    Deck();
    ~Deck();
    // 生成标准108张UNO牌
    void buildFullDeck();
    // 洗牌
    void shuffle();
    // 抽一张牌，牌堆空返回空万能牌
    Card draw();
    // 牌堆是否为空
    bool empty() const;
};
#endif