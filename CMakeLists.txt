cmake_minimum_required(VERSION 2.8)

project(digdug2)
include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(SOURCE_FILES
        src/main.cpp
        src/game.cpp
        src/player.cpp
        src/world.cpp
        src/entity.cpp
        src/enemy.cpp)
set(HEADER_FILES
        include/game.h
        include/player.h
        include/world.h
        include/entity.h
        include/enemy.h
        include/data.h)
add_executable(digdug2 ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(digdug2 -lsfml-window -lsfml-system -lsfml-graphics -lsfml-audio)
target_link_libraries(digdug2 -lGLEW -lGL -lGLU -lm)