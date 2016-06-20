//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_GAME_H
#define DIGDUG2_GAME_H

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "player.h"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void stepEvents();
    void stepRender();

    void windowReshape();
    void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

    sf::Window * window;
    sf::Clock clock;

    Player * player;
    World * world;
    std::vector<Entity *> entities;

    void stepUpdates();
};


#endif //DIGDUG2_GAME_H
