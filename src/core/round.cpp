// TODO: 实现Round类方法 - start发牌、playTurn处理回合、特殊牌效果(Skip/Reverse/+2/+4)
#include "round.h"
#include "judger.h"
#include <iostream>

Round::Round(std::vector<Player*>& players, Dealer& dealer)
    : playerList(players),
    tableDealer(dealer),
    currentPlayerIndex(0),
    isReverse(false),
    skipCount(0)
{
}

void Round::start()
{
    // 标准UNO：每人初始发7张牌
    const int initCardNum = 7;
    for (Player* p : playerList)
    {
        tableDealer.dealCards(*p, initCardNum);
    }

    // 翻开第一张牌作为弃牌堆顶牌（保证不是万能+4，符合规则）
    Card firstCard;
    do
    {
        firstCard = tableDealer.drawCard();
    } while (firstCard.getType() == CardType::WILD_DRAW_FOUR);

    tableDealer.addToDiscard(firstCard);
    std::cout << "游戏开始，桌面首张牌：" << firstCard.toString() << "\n" << std::endl;
}

void Round::playTurn()
{
    if (playerList.empty())
        return;

    // 处理跳过效果
    if (skipCount > 0)
    {
        std::cout << playerList[currentPlayerIndex]->getName() << " 本回合被跳过！\n" << std::endl;
        skipCount--;
        // 切换到下一位玩家
        if (isReverse)
            currentPlayerIndex = (currentPlayerIndex - 1 + playerList.size()) % playerList.size();
        else
            currentPlayerIndex = (currentPlayerIndex + 1) % playerList.size();
        return;
    }

    Player* curPlayer = playerList[currentPlayerIndex];
    Card topCard = tableDealer.getTopDiscard();

    std::cout << "轮到玩家：" << curPlayer->getName() << std::endl;

    // 1. 判断有无可出牌
    if (!curPlayer->hasValidPlay(topCard))
    {
        // 无牌可出，摸一张牌
        Card newCard = tableDealer.drawCard();
        curPlayer->addCard(newCard);
        std::cout << "无合法卡牌可出，摸牌：" << newCard.toString() << "\n" << std::endl;
    }
    else
    {
        // 2. 选择并打出卡牌
        int selectIdx = curPlayer->chooseCard(topCard);
        if (selectIdx >= 0)
        {
            const Card& cardToPlay = curPlayer->getHand()[selectIdx];
            // 校验出牌合法性
            if (!Judger::canPlayCard(cardToPlay, topCard))
            {
                std::cout << "出牌不合法，本轮放弃出牌\n" << std::endl;
            }
            else
            {
                Card playedCard = curPlayer->playCard(selectIdx);
                tableDealer.addToDiscard(playedCard);
                std::cout << "打出卡牌：" << playedCard.toString() << "\n" << std::endl;
                handleSpecialCard(playedCard);
                if (checkWin())
                    return;
            }
        }
    }

    // 3. 切换下一位玩家
    if (isReverse)
        currentPlayerIndex = (currentPlayerIndex - 1 + playerList.size()) % playerList.size();
    else
        currentPlayerIndex = (currentPlayerIndex + 1) % playerList.size();
}

void Round::handleSpecialCard(const Card& card)
{
    switch (card.getType())
    {
    case CardType::SKIP:
        // 跳过下一位玩家
        skipCount = 1;
        std::cout << "触发效果：下一位玩家被跳过\n" << std::endl;
        break;

    case CardType::REVERSE:
        // 反转出牌方向
        isReverse = !isReverse;
        std::cout << "触发效果：出牌方向已反转\n" << std::endl;
        break;

    case CardType::DRAW_TWO:
        // 下家摸2张牌
    {
        int nextIdx;
        if (isReverse)
            nextIdx = (currentPlayerIndex - 1 + playerList.size()) % playerList.size();
        else
            nextIdx = (currentPlayerIndex + 1) % playerList.size();

        Player* nextP = playerList[nextIdx];
        tableDealer.dealCards(*nextP, 2);
        std::cout << "触发效果：" << nextP->getName() << " 摸2张牌\n" << std::endl;
        break;
    }

    case CardType::WILD_DRAW_FOUR:
    {
        // 万能+4：下家摸4张牌
        int nextIdx;
        if (isReverse)
            nextIdx = (currentPlayerIndex - 1 + playerList.size()) % playerList.size();
        else
            nextIdx = (currentPlayerIndex + 1) % playerList.size();

        Player* nextP = playerList[nextIdx];
        tableDealer.dealCards(*nextP, 4);
        std::cout << "触发效果：" << nextP->getName() << " 摸4张牌\n" << std::endl;
        break;
    }

    case CardType::WILD:
    {
        std::cout << "触发效果：万能牌，请选择新颜色(0:RED 1:BLUE 2:GREEN 3:YELLOW)\n";
        int colorSel = 0;
        std::cin >> colorSel;
        // 简易模拟：直接用一张对应颜色空牌覆盖顶牌颜色
        CardColor newColor = CardColor::RED;
        switch (colorSel)
        {
        case 0: newColor = CardColor::RED; break;
        case 1: newColor = CardColor::BLUE; break;
        case 2: newColor = CardColor::GREEN; break;
        case 3: newColor = CardColor::YELLOW; break;
        }
        // 替换弃牌堆顶（模拟改色，不改动原有结构）
        tableDealer.addToDiscard(Card(newColor, CardType::NUMBER, 0));
        std::cout << "已切换为新颜色\n" << std::endl;
        break;
    }

    default:
        break;
    }
}

bool Round::checkWin() const
{
    // 任意玩家手牌为空即获胜
    for (const Player* p : playerList)
    {
        if (!p->hasHandCard())
        {
            std::cout << "========== 玩家 " << p->getName() << " 获胜！ ==========\n" << std::endl;
            return true;
        }
    }
    return false;
}