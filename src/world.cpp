//
// Created by lucas on 30/04/16.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <set>
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

    field_width = image.getSize().x + 2;
    field_height = image.getSize().y + 2;

    field = new GroundType*[field_width];
    for (int x = 0; x < field_width; x++) {
        field[x] = new GroundType[field_height];
    }

    fieldCopy = new GroundType*[field_width];
    for (int x = 0; x < field_width; x++) {
        fieldCopy[x] = new GroundType[field_height];
    }

    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            field[x][y] = Water;
            if (!(x == 0 || x == (field_width - 1) || y == 0 || y == (field_height - 1))) {
                sf::Color color = image.getPixel((unsigned int) x - 1, (unsigned int) y - 1);
                if (color.g == 255) {
                    field[x][y] = Terrain;
                } else if (color.r == 255) {
                    field[x][y] = Hole;
                }
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
            if (field[x][y] != Water) {
                renderGrass(x - 1, y - 1);
                switch (field[x][y]) {
                    case Stone: break;
                    case Crack: renderCrack(x - 1, y - 1); break;
                    case Hole: renderHole(x - 1, y - 1); break;
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

void World::generateCrack(int x, int y, Direction direction) {
    int ax = x;
    int ay = y;

    if (field[ax][ay] != Hole)
        return;

    switch (direction) {
        case Left: {
            ax--;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ax--;
            }
        } break;
        case Right: {
            ax++;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ax++;
            }
        } break;
        case Up: {
            ay++;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ay++;
            }
        } break;
        case Down: {
            ay--;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ay--;
            }
        } break;
    }

    destroySeparated();
}

void World::destroySeparated() {
    for (int x1 = 0; x1 < field_width; x1++) {
        for (int x2 = 0; x2 < field_width; x2++) {
            for (int y1 = 0; y1 < field_width; y1++) {
                for (int y2 = 0; y2 < field_width; y2++) {
                    if (checkSeparated(x1, y1, x2, y2)) {

                    }
                }
            }
        }
    }
}

bool World::checkSeparated(int x1, int y1, int x2, int y2) {
    std::set<int> x1Set;
    std::set<int> x2Set;
    std::set<int> y1Set;
    std::set<int> y2Set;

    fillPointsAlgorithm(x1, y2, &x1Set, &y1Set);
}

void World::fillPointsAlgorithm(int x, int y, std::set<int> * xSet, std::set<int> * ySet) {

}

bool World::arrivesWater(int x, int y, Direction direction) {
    int ax = x;
    int ay = y;

    switch (direction) {
        case Left: {
            ax--;
            while(field[ax][ay] == Crack) {
                ax--;
            }
        } break;
        case Right: {
            ax++;
            while(field[ax][ay] == Crack) {
                ax++;
            }
        } break;
        case Up: {
            ay++;
            while(field[ax][ay] == Crack) {
                ay++;
            }
        } break;
        case Down: {
            ay--;
            while(field[ax][ay] == Crack) {
                ay--;
            }
        } break;
    }

    return field[ax][ay] == Water;
}

int World::fillAlgorithm(int x, int y) {
    if (fieldCopy[x][y] == Terrain) {
        fieldCopy[x][y] = Water;
        return fillAlgorithm(x+1, y) +
               fillAlgorithm(x-1, y) +
               fillAlgorithm(x, y-1) +
               fillAlgorithm(x, y+1) +
               1;
    }

    return 0;
}

void World::fillWater(int x, int y) {
    if (field[x][y] == Terrain) {
        field[x][y] = Water;
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
           || field[x][y] == Water;
}

























