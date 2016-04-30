//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_PLAYER_H
#define DIGDUG2_PLAYER_H

#include <SFML/Window/Event.hpp>
#include "entity.h"

typedef struct {
    float x;
    float y;
    float z;
    float rotx;
    float roty;
} position_t;

class Player : Entity {
public:
    Player();

    void setCamera();
    void event(sf::Event event);
    void update(float dt);
    void render();

private:
    position_t current;

    bool leftPressed;
    bool rightPressed;
    bool upPressed;
    bool downPressed;
};


#endif //DIGDUG2_PLAYER_H
