// TODO: 实现控制台测试程序 - 测试核心游戏逻辑，无需Qt依赖
#include "game.h"
#include "human_player.h"
#include "ai_player.h"
#include <iostream>

int main()
{
    // 1. 创建游戏对象
    Game unoGame;

    // 2. 创建玩家：1个人类 + 2个AI
    HumanPlayer human("玩家(你)");
    AIPlayer ai1("AI一号");
    AIPlayer ai2("AI二号");

    // 3. 加入游戏
    unoGame.addPlayer(&human);
    unoGame.addPlayer(&ai1);
    unoGame.addPlayer(&ai2);

    // 4. 启动并运行游戏
    unoGame.start();
    unoGame.play();

    // 5. 输出最终胜者
    Player* winner = unoGame.getWinner();
    if (winner != nullptr)
    {
        std::cout << "\n最终胜利者：" << winner->getName() << std::endl;
    }

    return 0;
}