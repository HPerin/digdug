//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_WORLD_H
#define DIGDUG2_WORLD_H

#include <SFML/Graphics.hpp>
#include "entity.h"

#define GRASS_TEXTURE "resource/grass.jpg"
#define WATER_TEXTURE "resource/water.jpg"
#define BASE_TEXTURE  "resource/base.jpg"
#define HOLE_TEXTURE  "resource/hole.jpg"
#define CRACK_TEXTURE "resource/crack.jpg"

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
    bool hasWater(int x, int y);

private:
    void renderWater (int x, int y);
    void renderGrass(int x, int y);
    void renderHole  (int x, int y);
    void renderCrack (int x, int y);
    void renderDelimiter(int x, int y);

    void loadField();
    void loadTextures();

    void destroySeparated(int x, int y);
    bool checkSeparated(int x, int y, DIRECTION * direction);
    bool arrivesWater(int x, int y, DIRECTION direction);

    int fillAlgorithm(int x, int y);
    void fillWater(int x, int y);

    int field_width;
    int field_height;
    GROUND_TYPE ** field;
    GROUND_TYPE ** fieldCopy;

    sf::Texture grassTexture;
    sf::Texture baseTexture;
    sf::Texture holeTexture;
    sf::Texture crackTexture;
    sf::Texture waterTexture;
    float waterOffset = 0;

    void renderBase(int x, int y);
};


#endif //DIGDUG2_WORLD_H
