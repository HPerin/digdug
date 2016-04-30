//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_PLAYER_H
#define DIGDUG2_PLAYER_H

#include <SFML/Window/Event.hpp>
#include "entity.h"
#include "world.h"

typedef struct {
    float x;
    float y;
    float z;
    float rotx;
    float roty;
} position_t;

class Player : Entity {
public:
    Player(World * world);

    void setCamera();
    void event(sf::Event event);
    void update(float dt);
    void resetMouse(int x, int y);
    void render();

private:
    position_t current;

    bool leftPressed = false;
    bool rightPressed = false;
    bool upPressed = false;
    bool downPressed = false;

    int xOldMouse = 0;
    int yOldMouse = 0;
    int xDeltaMouse = 0;
    int yDeltaMouse = 0;

    World * world;

    void generateCrack();
};


#endif //DIGDUG2_PLAYER_H
