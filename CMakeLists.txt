cmake_minimum_required(VERSION 2.8)

project(digdug2)
include_directories(lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(SOURCE_FILES
        main.cpp
        game.cpp
        player.cpp
        world.cpp
        entity.cpp)
set(HEADER_FILES
        game.h
        player.h
        world.h
        entity.h enemy.cpp enemy.h)
add_executable(digdug2 ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(digdug2 -lsfml-window -lsfml-system -lsfml-graphics)
target_link_libraries(digdug2 -lGLEW -lGL -lGLU -lm)