//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_ENEMY_H
#define DIGDUG2_ENEMY_H


#include "entity.h"

class Enemy : Entity {
    Enemy();

    void event(sf::Event event);
    void update(float dt);
    void render();
};


#endif //DIGDUG2_ENEMY_H
