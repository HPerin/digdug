cmake_minimum_required(VERSION 2.8)

project(digdug2)
include_directories(lib)
add_definitions("-std=c++11")
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
        entity.h)
add_executable(digdug2 ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(digdug2 -lsfml-window -lsfml-system)
target_link_libraries(digdug2 -lGLEW -lGL -lGLU -lm)