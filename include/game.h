#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "player.h"
#include "ai_player.h"
#include <vector>

class Game
{
private:
    Deck deck;
    std::vector<Player*> players;
    Card topCard;
    Color currentColor;
    int currentPlayerIdx;
    int direction;
    int drawStack; // 叠加罚牌总数，+2/+4叠加记录

    // 判断玩家手牌是否存在指定颜色
    bool playerHasColor(Player* p, Color target) const;
    // 玩家是否有可叠加的+2
    bool playerHasDrawTwo(Player* p) const;
public:
    Game();
    ~Game();
    void initDeal();
    void randomFirstPlayer(); // 随机先手
    void nextPlayer();
    Card getTopCard() const;
    Color getCurrentColor() const;
    void setTopCard(const Card& c);
    void setCurrentColor(Color c);
    Player* getCurrentPlayer();

    // 判断当前牌是否合法可出
    bool canPlayCard(Player* p, const Card& play) const;
    // 打出牌后执行效果
    void executeEffect(const Card& played);
    bool checkWin() const;
    void gameLoop();
};
#endif