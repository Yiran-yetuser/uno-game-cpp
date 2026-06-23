cmake_minimum_required(VERSION 3.10.0)
project(uno-game-cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17) # C++17版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 新增两行：指定输入源码UTF8、程序输出字符串UTF8
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

add_executable(uno-game-cpp
    src/main.cpp
    src/card.cpp
    src/player.cpp
    src/ai_player.cpp
    src/deck.cpp
    src/game.cpp
)

# 控制台版 UNO 四方位布局（基于 src/ 现有类）
add_executable(uno-console
    console_ui.cpp
    src/card.cpp
    src/player.cpp
    src/ai_player.cpp
    src/deck.cpp
    src/game.cpp
)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

include(CTest)
enable_testing()

