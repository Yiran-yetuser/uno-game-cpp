#include "card.h"

Card::Card(CardColor color, CardType type, int value)
    : color(color), type(type), value(value) {}

CardColor Card::getColor() const {
    return color;
}

CardType Card::getType() const {
    return type;
}

int Card::getValue() const {
    return value;
}

std::string Card::toString() const {
    std::string result = colorToString(color) + " ";
    if (type == CardType::NUMBER) {
        result += std::to_string(value);
    } else {
        result += typeToString(type);
    }
    return result;
}

std::string colorToString(CardColor color) {
    switch (color) {
        case CardColor::RED:    return "RED";
        case CardColor::BLUE:   return "BLUE";
        case CardColor::GREEN:  return "GREEN";
        case CardColor::YELLOW: return "YELLOW";
        case CardColor::BLACK:  return "BLACK";
        default: return "UNKNOWN";
    }
}

std::string typeToString(CardType type) {
    switch (type) {
        case CardType::NUMBER:         return "NUM";
        case CardType::SKIP:           return "SKIP";
        case CardType::REVERSE:        return "REV";
        case CardType::DRAW_TWO:       return "+2";
        case CardType::WILD:           return "WILD";
        case CardType::WILD_DRAW_FOUR: return "WILD+4";
        default: return "UNKNOWN";
    }
}
