cmake_minimum_required(VERSION 3.10.0)
project(uno-game-cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17) # C++17版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(uno-game-cpp 
    src/main.cpp
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

