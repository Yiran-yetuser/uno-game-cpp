add_library(uno_core STATIC
    card.cpp
    dealer.cpp
    judger.cpp
    player.cpp
    ai_player.cpp
    human_player.cpp
    round.cpp
    game.cpp
)

target_include_directories(uno_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(uno_core PUBLIC cxx_std_17)

add_subdirectory(test)