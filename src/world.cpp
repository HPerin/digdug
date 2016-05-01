//
// Created by lucas on 30/04/16.
//

#include <GL/gl.h>
#include <iostream>
#include <cstring>
#include "world.h"

World::World() {
    loadField();

    loadTextures();
}

World::~World() {
    freeField();
}

void World::loadTextures() {
    grassTexture.loadFromFile(GRASS_TEXTURE);
    grassTexture.setSmooth(true);

    waterTexture.loadFromFile(WATER_TEXTURE);
    waterTexture.setSmooth(true);

    baseTexture.loadFromFile(BASE_TEXTURE);
    baseTexture.setSmooth(true);

    holeTexture.loadFromFile(HOLE_TEXTURE);
    holeTexture.setSmooth(true);

    crackTexture.loadFromFile(CRACK_TEXTURE);
    crackTexture.setSmooth(true);
}

void World::freeField() {
    for (int x = 0; x < field_width; x++) {
        free(field[x]);
        free(fieldCopy[x]);
    }
    free(field);
    free(fieldCopy);
}

void World::loadField() {
    sf::Image image;
    if (!image.loadFromFile(MAP_BITMAP)) {
        std::cerr << "UNABLE TO LOAD MAP!!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    field_width = image.getSize().x;
    field_height = image.getSize().y;

    field = new GROUND_TYPE*[field_width];
    for (int x = 0; x < field_width; x++) {
        field[x] = new GROUND_TYPE[field_height];
    }

    fieldCopy = new GROUND_TYPE*[field_width];
    for (int x = 0; x < field_width; x++) {
        fieldCopy[x] = new GROUND_TYPE[field_height];
    }

    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            sf::Color color = image.getPixel((unsigned int) x, (unsigned int) y);
            if (color.g == 255) {
                field[x][y] = TERRAIN;
            } else if (color.r == 255) {
                field[x][y] = HOLE;
            }
        }
    }
}

void World::event(sf::Event event) {

}

void World::update(float dt) {
    waterOffset += dt;
    while(waterOffset > 1) waterOffset -= 1;
}

void World::render() {
    for (int x = -30; x < 30; x++) {
        for (int y = -30; y < 30; y++) {
            renderWater(x, y);
        }
    }

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            if (field[x][y] != WATER) {
                renderGrass(x - 1, y - 1);
                switch (field[x][y]) {
                    case STONE: break;
                    case CRACK: renderCrack(x - 1, y - 1); break;
                    case HOLE: renderHole(x - 1, y - 1); break;
                    default: break;
                }
            }

            //renderDelimiter(x-1, y-1);
        }
    }
}

void World::renderWater(int x, int y) {
    glPushMatrix();
    glTranslatef(x, -1.f, y + waterOffset);

    sf::Texture::bind(&waterTexture);

    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 0.f, 1.f);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);

    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderGrass(int x, int y) {
    renderBase(x, y);

    glPushMatrix();
    glTranslatef(  x, -0.5f,   y);

    sf::Texture::bind(&grassTexture);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 1.f, 0.f);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);

    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderHole(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.49f,   y);

    sf::Texture::bind(&holeTexture);

    glBegin(GL_QUADS);
    glColor3f   (0.41,0.25,0.14);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderCrack(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.49f,   y);

    sf::Texture::bind(&crackTexture);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderDelimiter(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.49f,   y);

    glBegin(GL_LINE_LOOP);
    glColor3f   ( 0.3f, 0.3f, 0.3f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    glPopMatrix();
}

void World::renderBase(int x, int y) {
    glPushMatrix();
    glTranslatef(x, -0.5f, y);

    sf::Texture::bind(&baseTexture);

    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);

    glTexCoord2f(      1.f, 0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glTexCoord2f(      1.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glTexCoord2f(      0.f, 1.f);
    glVertex3f  ( 0.f,-1.f, 1.f);
    glTexCoord2f(      0.f, 0.f);
    glVertex3f  ( 0.f,-1.f, 0.f);

    glTexCoord2f(      1.f, 1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glTexCoord2f(      1.f, 0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glTexCoord2f(      0.f, 0.f);
    glVertex3f  ( 1.f,-1.f, 0.f);
    glTexCoord2f(      0.f, 1.f);
    glVertex3f  ( 1.f,-1.f, 1.f);

    glTexCoord2f( 1.f, 1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glTexCoord2f( 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glTexCoord2f( 0.f, 0.f);
    glVertex3f  ( 0.f,-1.f, 1.f);
    glTexCoord2f( 1.f, 0.f);
    glVertex3f  ( 1.f,-1.f, 1.f);

    glTexCoord2f( 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glTexCoord2f( 1.f, 1.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glTexCoord2f( 1.f, 0.f);
    glVertex3f  ( 1.f,-1.f, 0.f);
    glTexCoord2f( 0.f, 0.f);
    glVertex3f  ( 0.f,-1.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::generateCrack(int x, int y, DIRECTION direction) {
    int ax = x;
    int ay = y;

    if (field[ax][ay] != HOLE)
        return;

    switch (direction) {
        case LEFT: {
            ax--;
            while(field[ax][ay] == TERRAIN) {
                field[ax][ay] = CRACK;
                ax--;
            }
        } break;
        case RIGHT: {
            ax++;
            while(field[ax][ay] == TERRAIN) {
                field[ax][ay] = CRACK;
                ax++;
            }
        } break;
        case UP: {
            ay++;
            while(field[ax][ay] == TERRAIN) {
                field[ax][ay] = CRACK;
                ay++;
            }
        } break;
        case DOWN: {
            ay--;
            while(field[ax][ay] == TERRAIN) {
                field[ax][ay] = CRACK;
                ay--;
            }
        } break;
    }

    destroySeparated(x, y);
}

void World::destroySeparated(int x, int y) {
    DIRECTION direction[2];

    if (!checkSeparated(x, y, direction))
        return;

    int x1 = 0, y1 = 0;
    switch (direction[0]) {
        case LEFT: x1 = x - 1; y1 = y + 1; break;
        case RIGHT: x1 = x + 1; y1 = y - 1; break;
        case UP: x1 = x + 1; y1 = y + 1; break;
        case DOWN: x1 = x - 1; y1 = y - 1; break;
    }

    int x2 = 0, y2 = 0;
    switch (direction[1]) {
        case LEFT: x2 = x - 1; y2 = y + 1; break;
        case RIGHT: x2 = x + 1; y2 = y - 1; break;
        case UP: x2 = x + 1; y2 = y + 1; break;
        case DOWN: x2 = x - 1; y2 = y - 1; break;
    }

    for (int xi = 0; xi < field_width; xi++) {
        for (int yi = 0; yi <  field_height; yi++) {
            fieldCopy[xi][yi] = field[xi][yi];
        }
    }
    int field1 = fillAlgorithm(x1, y1);
    for (int xi = 0; xi < field_width; xi++) {
        for (int yi = 0; yi <  field_height; yi++) {
            fieldCopy[xi][yi] = field[xi][yi];
        }
    }
    int field2 = fillAlgorithm(x2, y2);
    if (field1 > field2) {
        fillWater(x2, y2);
    } else {
        fillWater(x1, y1);
    }
}

bool World::checkSeparated(int x, int y, DIRECTION * direction) {
    int cur_dir = 0;

    if (arrivesWater(x, y, LEFT) && field[x-1][y+1] == TERRAIN) {
        direction[cur_dir] = LEFT;
        cur_dir++;
    }
    if (arrivesWater(x, y, RIGHT) && field[x+1][y-1] == TERRAIN) {
        direction[cur_dir] = RIGHT;
        cur_dir++;
    }
    if (arrivesWater(x, y, UP) && field[x+1][y+1] == TERRAIN) {
        direction[cur_dir] = UP;
        cur_dir++;
    }
    if (arrivesWater(x, y, DOWN) && field[x-1][y-1] == TERRAIN) {
        direction[cur_dir] = DOWN;
        cur_dir++;
    }

    return cur_dir >= 2;
}

bool World::arrivesWater(int x, int y, DIRECTION direction) {
    int ax = x;
    int ay = y;

    switch (direction) {
        case LEFT: {
            ax--;
            while(field[ax][ay] == CRACK) {
                ax--;
            }
        } break;
        case RIGHT: {
            ax++;
            while(field[ax][ay] == CRACK) {
                ax++;
            }
        } break;
        case UP: {
            ay++;
            while(field[ax][ay] == CRACK) {
                ay++;
            }
        } break;
        case DOWN: {
            ay--;
            while(field[ax][ay] == CRACK) {
                ay--;
            }
        } break;
    }

    return field[ax][ay] == WATER;
}

int World::fillAlgorithm(int x, int y) {
    if (fieldCopy[x][y] == TERRAIN) {
        fieldCopy[x][y] = WATER;
        return fillAlgorithm(x+1, y) +
               fillAlgorithm(x-1, y) +
               fillAlgorithm(x, y-1) +
               fillAlgorithm(x, y+1) +
               1;
    }

    return 0;
}

void World::fillWater(int x, int y) {
    if (field[x][y] == TERRAIN) {
        field[x][y] = WATER;
        fillWater(x+1, y);
        fillWater(x-1, y);
        fillWater(x, y-1);
        fillWater(x, y+1);
    }
}

bool World::hasWater(int x, int y) {
    return x < 0
           || x > field_width
           || y < 0
           || y > field_height
           || field[x][y] == WATER;
}























