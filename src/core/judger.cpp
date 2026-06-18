// TODO: 实现Judger类方法 - isValidMove、canPlayCard判断颜色/类型/数值匹配
#include "judger.h"

bool Judger::canPlayCard(const Card& played, const Card& topDiscard)
{
    // 万能牌、万能+4 随时可出
    if (played.getType() == CardType::WILD || played.getType() == CardType::WILD_DRAW_FOUR)
    {
        return true;
    }

    // 颜色相同 可出
    if (played.getColor() == topDiscard.getColor())
    {
        return true;
    }

    // 两张都是数字牌，数字相同 可出
    if (played.getType() == CardType::NUMBER && topDiscard.getType() == CardType::NUMBER)
    {
        if (played.getValue() == topDiscard.getValue())
        {
            return true;
        }
    }

    // 两张都是功能牌，牌型相同 可出
    if (played.getType() != CardType::NUMBER && topDiscard.getType() != CardType::NUMBER)
    {
        if (played.getType() == topDiscard.getType())
        {
            return true;
        }
    }

    return false;
}

bool Judger::isValidMove(const Card& played, const Card& topDiscard)
{
    // 此处复用核心判断逻辑，后续可扩展额外规则
    return canPlayCard(played, topDiscard);
}