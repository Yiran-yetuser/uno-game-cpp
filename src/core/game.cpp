// TODO: 实现Game类方法 - addPlayer、start、play、getWinner
#include "game.h"
#include <iostream>

Game::Game() = default;

Game::~Game()
{
    if (round != nullptr) // 增加判空
    {
        delete round;
        round = nullptr;
    }
}

void Game::addPlayer(Player* player)
{
    if (player != nullptr)
    {
        players.push_back(player);
    }
}

void Game::start()
{
    if (players.empty())
    {
        std::cout << "暂无玩家，无法开始游戏！" << std::endl;
        return;
    }

    // 初始化回合对象
    round = new Round(players, dealer);
    // 开局发牌、设置桌面首张牌
    round->start();
    std::cout << "游戏正式开始！\n" << std::endl;
}

void Game::play()
{
    if (round == nullptr)
    {
        std::cout << "请先调用 start() 初始化游戏！" << std::endl;
        return;
    }

    // 循环执行回合，直到有人获胜
    while (!round->checkWin())
    {
        round->playTurn();
    }
}

Player* Game::getWinner() const
{
    for (Player* p : players)
    {
        if (!p->hasHandCard())
        {
            return p;
        }
    }
    return nullptr;
}