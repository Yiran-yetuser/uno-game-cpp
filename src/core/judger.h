// TODO: 实现Judger类 - 判断出牌是否合法、验证卡牌匹配规则
#pragma once
#include "card.h"

// 规则裁判类：判断出牌是否合法
class Judger
{
public:
    Judger() = default;

    // 判断单张牌能否接在桌面顶牌之后
    static bool canPlayCard(const Card& played, const Card& topDiscard);

    // 完整校验本次出牌动作合法性
    static bool isValidMove(const Card& played, const Card& topDiscard);
};