//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_WORLD_H
#define DIGDUG2_WORLD_H

#include "entity.h"

typedef enum {
    WATER,
    TERRAIN,
    STONE,
    CRACK,
    HOLE
} GROUND_TYPE;

class World : Entity {
public:
    World();

    void event(sf::Event event);
    void update(float dt);
    void render();

private:
    void renderWater();
    void renderGround(int x, int y);

    void loadField();

    int field_width;
    int field_height;
    GROUND_TYPE ** field;
};


#endif //DIGDUG2_WORLD_H
