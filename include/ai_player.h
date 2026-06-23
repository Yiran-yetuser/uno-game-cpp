// ai_player.h AI玩家类，继承自玩家类Player，AI决策逻辑
#ifndef AI_PLAYER_H
#define AI_PLAYER_H
#include "card.h"
#include "player.h"

class AIPlayer : public Player
{
public:
    AIPlayer(std::string name = "AI对手");
    ~AIPlayer() override;
    // AI自动选择一张能出的牌下标，无牌可出返回-1
    int chooseCard(const Card &topCard) const;
};
#endif