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

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} DIRECTION;

class World : Entity {
public:
    World();

    void event(sf::Event event);
    void update(float dt);
    void render();

    void generateCrack(int ax, int ay, DIRECTION direction);

private:
    void renderWater ();
    void renderGround(int x, int y);
    void renderHole  (int x, int y);
    void renderCrack (int x, int y);
    void renderDelimiter(int x, int y);

    void loadField();

    void destroySeparated(int x, int y);
    bool checkSeparated(int x, int y, DIRECTION * direction);
    bool arrivesWater(int x, int y, DIRECTION direction);

    int fillAlgorithm(int x, int y);
    void fillWater(int x, int y);

    int field_width;
    int field_height;
    GROUND_TYPE ** field;
    GROUND_TYPE ** fieldCopy;
};


#endif //DIGDUG2_WORLD_H
