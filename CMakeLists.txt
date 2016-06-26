cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_NAME digdug2)
project(${EXECUTABLE_NAME})
include_directories(include)
include_directories(lib/OBJLOADER)
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
set(OBJLOADER_FILES
        lib/OBJLOADER/glm.cpp
        lib/OBJLOADER/glm.h
        lib/OBJLOADER/glmimg.cpp
        lib/OBJLOADER/Texture.cpp
        lib/OBJLOADER/Texture.h
        lib/OBJLOADER/vector3f.h include/modelhelper.h)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${OBJLOADER_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/lib/SFML")

find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} m)

include_directories(lib/freeglut/include)
target_link_libraries(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/lib/freeglut/lib/libfreeglut.a)

