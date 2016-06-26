//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_ENEMY_H
#define DIGDUG2_ENEMY_H

#include <SFML/Graphics.hpp>
#include <glm.h>
#include "entity.h"
#include "world.h"

class Enemy : Entity {
public:
    Enemy(int x, int y, World * world);

    void event(sf::Event event);
    void update(float dt);
    void render();
    void forceRender();

    void tryFollow(int x, int y);
    void forceFollow(int x, int y);
    int getCurrentX();
    int getCurrentY();
    bool dead;

    sf::Vector2f position;

private:
    sf::Vector2i target;

    World * world;

    GLMmodel * enemyModel;

    void die();
    bool following = false;
    bool accelerated = false;
};


#endif //DIGDUG2_ENEMY_H
