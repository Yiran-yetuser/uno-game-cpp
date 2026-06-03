cmake_minimum_required(VERSION 3.16)
project(uno-game-cpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR "/opt/homebrew/lib/cmake/Qt6" CACHE PATH "Qt6 cmake directory")

add_subdirectory(src/core)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)

add_executable(uno_game
    src/main.cpp
    src/ui/main_window.cpp
    src/ui/game_scene.cpp
    src/ui/card_widget.cpp
    src/ui/hand_widget.cpp
    src/ui/player_info_widget.cpp
)

target_link_libraries(uno_game PRIVATE uno_core Qt6::Widgets)
target_include_directories(uno_game PRIVATE src)