// player.h 实现玩家，包括玩家的牌组、出牌、牌组
#ifndef PLAYER_H
#define PLAYER_H
#include "card.h"
#include <vector>
#include <string>

class Player
{
protected:
    std::vector<Card> hand; // 手牌
    std::string name;
public:
    Player(std::string name = "玩家");
    virtual ~Player();

    // 抽一张牌到手牌
    void drawCard(const Card& c);
    // 获取手牌数量
    int getHandCount() const;
    // 获取所有手牌
    std::vector<Card>& getHand();
    // 打出指定下标卡牌，返回打出的牌
    Card playCard(int idx);
    // 打印手牌
    void showHand() const;
    std::string getName() const;
};
#endif