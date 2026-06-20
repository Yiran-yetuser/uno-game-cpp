// game.h 实现游戏，游戏主循环、回合管理、规则判定
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
    Card topCard;          // 场上当前牌
    int currentPlayerIdx;  // 当前行动玩家下标
    int direction;          // 1顺向 -1逆向
public:
    // 初始化：1人类玩家 + 3AI
    Game();
    ~Game();
    // 开局发7张牌给所有人
    void initDeal();
    // 切换下一位玩家
    void nextPlayer();
    // 获取当前场上顶牌
    Card getTopCard() const;
    // 设置场上顶牌（出牌后更新）
    void setTopCard(const Card& c);
    // 获取当前玩家
    Player* getCurrentPlayer();
    // 执行一张牌的特效（跳过、反转、+2、万能+4）
    void executeCardEffect(const Card& played);
    // 判断是否有人赢（手牌空）
    bool checkWin() const;
    // 游戏主循环控制台交互
    void gameLoop();
};
#endif