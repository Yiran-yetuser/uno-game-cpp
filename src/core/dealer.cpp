// TODO: 实现Dealer类方法 - 初始化108张UNO牌、洗牌、抽牌、管理弃牌堆
#include "dealer.h"
#include "player.h"
#include <algorithm>
#include <ctime>
#include <random>

Dealer::Dealer()
{
    initFullDeck();
    shuffleDeck();
}

void Dealer::initFullDeck()
{
    deck.clear();
    discardPile.clear();

    // 四种普通颜色
    const std::vector<CardColor> colors = {
        CardColor::RED,
        CardColor::BLUE,
        CardColor::GREEN,
        CardColor::YELLOW};

    // 1. 数字牌 0~9
    for (CardColor c : colors) {
        // 数字0 每种颜色各1张
        deck.emplace_back(c, CardType::NUMBER, 0);
        // 数字1~9 每种颜色各2张
        for (int v = 1; v <= 9; ++v) {
            deck.emplace_back(c, CardType::NUMBER, v);
            deck.emplace_back(c, CardType::NUMBER, v);
        }

        // 2. 功能牌：跳过、反转、+2 每种颜色各2张
        for (int i = 0; i < 2; ++i) {
            deck.emplace_back(c, CardType::SKIP, 0);
            deck.emplace_back(c, CardType::REVERSE, 0);
            deck.emplace_back(c, CardType::DRAW_TWO, 0);
        }
    }

    // 3. 万能牌 & 万能+4：各4张，颜色为BLACK
    for (int i = 0; i < 4; ++i) {
        deck.emplace_back(CardColor::BLACK, CardType::WILD, 0);
        deck.emplace_back(CardColor::BLACK, CardType::WILD_DRAW_FOUR, 0);
    }
}

void Dealer::shuffleDeck()
{
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

Card Dealer::drawCard()
{
    // 主牌堆空了，回收弃牌堆再抽
    if (deck.empty()) {
        recycleDiscard();
    }
    if (deck.empty()) {
        return Card(CardColor::BLACK, CardType::NUMBER, 0);
    }

    Card card = deck.front();
    deck.erase(deck.begin());
    return card;
}

void Dealer::dealCards(Player &player, int count)
{
    for (int i = 0; i < count; ++i) {
        player.addCard(drawCard());
    }
}

void Dealer::addToDiscard(const Card &card)
{
    discardPile.push_back(card);
}

Card Dealer::getTopDiscard() const
{
    if (discardPile.empty()) {
        return Card(CardColor::BLACK, CardType::NUMBER, 0);
    }
    return discardPile.back();
}

void Dealer::recycleDiscard()
{
    if (discardPile.size() <= 1)
        return;

    // 保留弃牌堆顶牌，其余洗回主牌堆
    size_t keepIdx = discardPile.size() - 1;
    for (size_t i = 0; i < keepIdx; ++i) {
        deck.push_back(discardPile[i]);
    }
    // 清空旧弃牌堆，只保留顶牌
    Card top = discardPile.back();
    discardPile.clear();
    discardPile.push_back(top);

    shuffleDeck();
}