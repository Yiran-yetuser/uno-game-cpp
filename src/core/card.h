#pragma once

#include <string>

// UNO卡牌颜色枚举
enum class CardColor {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    BLACK // 万能牌
};

// UNO卡牌类型枚举
enum class CardType {
    NUMBER,        // 数字牌 0-9
    SKIP,          // 跳过
    REVERSE,       // 反转
    DRAW_TWO,      // +2
    WILD,          // 万能牌
    WILD_DRAW_FOUR // 万能+4
};

// Card类 - 表示一张UNO卡牌
class Card
{
public:
    Card(CardColor color, CardType type, int value = 0);

    CardColor getColor() const;
    CardType getType() const;
    int getValue() const;
    std::string toString() const;

private:
    CardColor color;
    CardType type;
    int value; // 仅对数字牌有效
};

// 辅助函数：枚举转字符串
std::string colorToString(CardColor color);
std::string typeToString(CardType type);
