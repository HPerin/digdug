//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_ENEMY_H
#define DIGDUG2_ENEMY_H

#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include "entity.h"
#include "world.h"

class Enemy : Entity {
public:
    Enemy(int x, int y, World * world);

    void event(sf::Event event);
    void update(float dt);
    void render();

    void tryFollow(int x, int y);


    sf::Vector2f position;

private:
    sf::Vector2i target;

    World * world;

    void die();
    bool dead;
};


#endif //DIGDUG2_ENEMY_H
