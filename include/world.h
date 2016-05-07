//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_WORLD_H
#define DIGDUG2_WORLD_H

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "entity.h"

#define GRASS_TEXTURE "resource/grass.jpg"
#define WATER_TEXTURE "resource/water.jpg"
#define BASE_TEXTURE  "resource/base.jpg"
#define HOLE_TEXTURE  "resource/hole.jpg"
#define CRACK_TEXTURE "resource/crack.jpg"

#define MAP_BITMAP    "resource/map2.png"

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

private:
    void renderWater (int x, int y);
    void renderGrass(int x, int y);
    void renderHole  (int x, int y);
    void renderCrack (int x, int y);
    void renderDelimiter(int x, int y);

    void loadField();
    void freeField();
    void loadTextures();

    void destroySeparated();
    bool checkSeparated(int x, int y, int x2, int y2);
    bool arrivesWater(int x, int y, Direction direction);

    int fillAlgorithm(int x, int y);
    void fillWater(int x, int y);

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

    void renderBase(int x, int y);

    void fillPointsAlgorithm(int x, int y, std::set<int> * xSet, std::set<int> * ySet);
};


#endif //DIGDUG2_WORLD_H
