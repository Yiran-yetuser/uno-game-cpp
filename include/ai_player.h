// ai_player.h 缁ф壙 Player锛孉I鍐崇瓥閫昏緫
#ifndef AI_PLAYER_H
#define AI_PLAYER_H
#include "player.h"
#include "card.h"

class AIPlayer : public Player
{
public:
    AIPlayer(std::string name = "AI对手");
    ~AIPlayer() override;
    // AI自动选择一张能出的牌下标，无牌可出返回-1
    int chooseCard(const Card& topCard) const;
};
#endif