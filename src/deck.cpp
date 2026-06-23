#include "deck.h"
#include <algorithm>
#include <ctime>
#include <random>

Deck::Deck()
{
    buildFullDeck();
    shuffle(drawPile);
}
Deck::~Deck() = default;

void Deck::buildFullDeck()
{
    drawPile.clear();
    std::vector<Color> colors = {Color::Red, Color::Yellow, Color::Green, Color::Blue};
    // 数字牌 0各1张，1-9各2张
    for (Color c : colors) {
        drawPile.emplace_back(c, CardType::Number, 0);
        for (int num = 1; num <= 9; ++num) {
            drawPile.emplace_back(c, CardType::Number, num);
            drawPile.emplace_back(c, CardType::Number, num);
        }
        // 功能牌各2张
        for (int i = 0; i < 2; ++i) {
            drawPile.emplace_back(c, CardType::Skip);
            drawPile.emplace_back(c, CardType::Reverse);
            drawPile.emplace_back(c, CardType::DrawTwo);
        }
    }
    // 万能牌4张，万能+4四张
    for (int i = 0; i < 4; ++i) {
        drawPile.emplace_back(Color::Wild, CardType::Wild);
        drawPile.emplace_back(Color::Wild, CardType::WildDrawFour);
    }
}

void Deck::shuffle(std::vector<Card> &pile)
{
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::shuffle(pile.begin(), pile.end(), rng); // C++标准库实现Fisher-Yates洗牌算法
}

Card Deck::draw()
{
    if (drawPile.empty()) {
        reshuffleDiscard();
    }
    if (drawPile.empty()) {
        return Card(Color::Wild, CardType::Wild);
    }
    Card res = drawPile.back();
    drawPile.pop_back();
    return res;
}
void Deck::reshuffleDiscard()
{
    if (discardPile.size() <= 1)
        return;

    Card top = discardPile.back(); // 保留顶牌
    drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end() - 1);
    discardPile.clear();
    discardPile.push_back(top); // 把顶牌放回去
    shuffle(drawPile);
}
void Deck::addDiscard(const Card &card)
{
    discardPile.push_back(card);
}

Card Deck::topCard() const
{
    if (discardPile.empty()) {
        return Card(Color::Wild, CardType::Wild);
    }
    return discardPile.back();
}

bool Deck::empty() const
{
    return drawPile.empty() && discardPile.empty();
}
