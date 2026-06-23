<<<<<<< HEAD
// TODO: 实现main函数 - Qt应用程序入口，创建并显示MainWindow
=======
#include "game.h"
#include <iostream>
#include <windows.h>

int main()
{
    // 只保留这两行，删掉 locale 相关代码
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Game game;
    game.gameLoop();
    std::cout << "按回车退出...";
    std::cin.get();
    std::cin.get();
    return 0;
}
>>>>>>> dev
