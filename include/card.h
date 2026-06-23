// Card.h 实现牌的类型、颜色、特殊牌
#ifndef CARD_H
#define CARD_H

#include <string>

enum class Color {
    Red,
    Yellow,
    Green,
    Blue,
    Wild // 万能牌（颜色待定）
};

enum class CardType {
    Number,      // 数字牌 0-9
    Skip,        // 跳过
    Reverse,     // 反转
    DrawTwo,     // +2
    Wild,        // 万能牌（选颜色）
    WildDrawFour // +4
};

class Card
{
public:
    Color color;
    CardType type;
    int number; // 仅 Number 类型有效，0-9

    Card();
    Card(Color color, CardType type, int number = -1);
    ~Card();

    // 判断两张牌是否可以接龙（颜色相同 / 类型相同 / 数字相同）
    bool canPlayOn(const Card &topCard) const;

    // 获取牌的显示名称，如 "红7"、"蓝+2"、"万能+4"
    std::string toString() const;
};

#endif
