//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_WORLD_H
#define DIGDUG2_WORLD_H

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <glm.h>
#include "entity.h"

#define GRASS_TEXTURE "resource/grass.jpg"
#define WATER_TEXTURE "resource/water.jpg"
#define BASE_TEXTURE  "resource/base.jpg"
#define HOLE_TEXTURE  "resource/hole.jpg"
#define CRACK_TEXTURE "resource/crack.jpg"

#define MAP_BITMAP    "resource/map.png"

typedef enum {
    Water,
    Terrain,
    Stone,
    Crack,
    Hole
} GroundType;

typedef enum {
    Left,
    Right,
    Up,
    Down
} Direction;

class World : Entity {
public:
    World();
    ~World();

    void event(sf::Event event);
    void update(float dt);
    void render();

    void generateCrack(int ax, int ay, Direction direction);
    bool hasWater(int x, int y);
    bool hasTerrain(int x, int y);

    std::vector<Entity*> enemyList;
    sf::Vector2i initialPlayerPosition = {1, 1};

    bool hasStone(int x, int y);

private:
    void renderWater (int x, int y);
    void renderGrass(int x, int y);
    void renderHole  (int x, int y);
    void renderCrack (int x, int y);
    void renderBase(int x, int y);
    void renderDelimiter(int x, int y);

    void loadField();
    void freeField();
    void loadTextures();

    int field_width;
    int field_height;
    GroundType ** field;
    GroundType ** fieldCopy;

    sf::Texture grassTexture;
    sf::Texture baseTexture;
    sf::Texture holeTexture;
    sf::Texture crackTexture;
    sf::Texture waterTexture;
    float waterOffset = 0;

    bool destroySeparated();
    void fillWater(int x, int y);
    void fillBool(bool ** bool_field, int x, int y);
    int fillSize(bool ** bool_field, int x, int y);
    void initFalse(bool ** bool_field);

    GLMmodel * sunModel;

    void deleteCracks();

    void renderStone(int x, int y);
};


#endif //DIGDUG2_WORLD_H
