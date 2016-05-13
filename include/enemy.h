//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_ENEMY_H
#define DIGDUG2_ENEMY_H

#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include "entity.h"

class Enemy : Entity {
public:
    Enemy();

    void event(sf::Event event);
    void update(float dt);
    void render();

private:

};


#endif //DIGDUG2_ENEMY_H
