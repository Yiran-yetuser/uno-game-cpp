#include "deck.h"
#include <algorithm>
#include <random>
#include <ctime>

Deck::Deck()
{
    buildFullDeck();
    shuffle();
}
Deck::~Deck() = default;

void Deck::buildFullDeck()
{
    cards.clear();
    std::vector<Color> colors = {Color::Red, Color::Yellow, Color::Green, Color::Blue};
    // 数字牌 0各1张，1-9各2张
    for (Color c : colors)
    {
        cards.emplace_back(c, CardType::Number, 0);
        for (int num = 1; num <= 9; ++num)
        {
            cards.emplace_back(c, CardType::Number, num);
            cards.emplace_back(c, CardType::Number, num);
        }
        // 功能牌各2张
        for (int i = 0; i < 2; ++i)
        {
            cards.emplace_back(c, CardType::Skip);
            cards.emplace_back(c, CardType::Reverse);
            cards.emplace_back(c, CardType::DrawTwo);
        }
    }
    // 万能牌4张，万能+4四张
    for (int i = 0; i < 4; ++i)
    {
        cards.emplace_back(Color::Wild, CardType::Wild);
        cards.emplace_back(Color::Wild, CardType::WildDrawFour);
    }
}

void Deck::shuffle()
{
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::draw()
{
    if (cards.empty())
    {
        return Card(Color::Wild, CardType::Wild);
    }
    Card res = cards.back();
    cards.pop_back();
    return res;
}

bool Deck::empty() const
{
    return cards.empty();
}