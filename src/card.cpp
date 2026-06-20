#include "card.h"

Card::Card()
    : color(Color::Wild), type(CardType::Number), number(-1)
{
}

Card::Card(Color color, CardType type, int number)
    : color(color), type(type), number(number)
{
}

Card::~Card()
{
}

bool Card::canPlayOn(const Card &topCard) const
{
    // 万能牌可以接在任何牌后面
    if (this->type == CardType::Wild || this->type == CardType::WildDrawFour)
        return true;

    // 颜色相同可以出
    if (this->color == topCard.color)
        return true;

    // 类型相同可以出（同为 Skip、同为 Reverse 等）
    if (this->type == topCard.type)
        return true;

    // 同为数字牌且数字相同可以出
    if (this->type == CardType::Number && topCard.type == CardType::Number &&
        this->number == topCard.number)
        return true;

    return false;
}

std::string Card::toString() const
{
    // 颜色名称
    std::string colorName;
    switch (color) {
    case Color::Red:
        colorName = "红";
        break;
    case Color::Yellow:
        colorName = "黄";
        break;
    case Color::Green:
        colorName = "绿";
        break;
    case Color::Blue:
        colorName = "蓝";
        break;
    case Color::Wild:
        colorName = "万能";
        break;
    }

    // 牌面名称
    switch (type) {
    case CardType::Number:
        return colorName + std::to_string(number);
    case CardType::Skip:
        return colorName + "跳过";
    case CardType::Reverse:
        return colorName + "反转";
    case CardType::DrawTwo:
        return colorName + "+2";
    case CardType::Wild:
        return "万能牌";
    case CardType::WildDrawFour:
        return "万能+4";
    }

    return "未知牌";
}
