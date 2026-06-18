// TODO: 实现Game类 - 游戏主控制器，管理多回合、玩家列表、游戏状态
#pragma once
#include <vector>
#include <string>
#include "player.h"
#include "dealer.h"
#include "round.h"

// 游戏主控制器
class Game
{
public:
    Game();
    ~Game();

    // 添加玩家（人类/AI均可）
    void addPlayer(Player* player);

    // 启动整局游戏
    void start();

    // 循环执行游戏流程
    void play();

    // 获取获胜玩家，无胜者返回nullptr
    Player* getWinner() const;

private:
    std::vector<Player*> players;
    Dealer dealer;
    Round* round = nullptr;
};