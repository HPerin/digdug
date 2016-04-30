//
// Created by lucas on 30/04/16.
//

#include <GL/gl.h>
#include <iostream>
#include <cstring>
#include "world.h"

World::World() {
    loadField();
}

void World::loadField() {
    field_width = 20;
    field_height = 20;
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
            if (x == 0 || x == 19)
                field[x][y] = WATER;
            else if (y == 0 || y == 19) {
                field[x][y] = WATER;
            } else {
                field[x][y] = TERRAIN;
            }
        }
    }

    field[2][2] = HOLE;
}

void World::event(sf::Event event) {

}

void World::update(float dt) {

}

void World::render() {
    renderWater();

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            switch (field[x][y]) {
                case WATER: break;
                case TERRAIN: renderGround(x, y); break;
                case STONE:break;
                case CRACK: renderCrack(x, y); break;
                case HOLE: renderHole(x, y); break;
            }

            renderDelimiter(x, y);
        }
    }
}

void World::renderWater() {
    glPushMatrix();
    glTranslatef(0.f, -0.51f, 0.f);
    glScalef    (100.f, 1.f,100.f);

    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 0.f, 1.f);

    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f,-1.f);
    glVertex3f  ( 1.f, 0.f,-1.f);
    glEnd();

    glPopMatrix();
}

void World::renderGround(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.5f,   y);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 1.f, 0.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f,-1.f);
    glVertex3f  ( 1.f, 0.f,-1.f);
    glEnd();

    glPopMatrix();
}

void World::renderHole(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.5f,   y);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   (0.41,0.25,0.14);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f,-1.f);
    glVertex3f  ( 1.f, 0.f,-1.f);
    glEnd();

    glPopMatrix();
}

void World::renderCrack(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.5f,   y);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f,-1.f);
    glVertex3f  ( 1.f, 0.f,-1.f);
    glEnd();

    glPopMatrix();
}

void World::renderDelimiter(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.5f,   y);

    glBegin(GL_LINE_LOOP);
    glColor3f   ( 0.3f, 0.3f, 0.3f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f,-1.f);
    glVertex3f  ( 1.f, 0.f,-1.f);
    glEnd();

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
    switch (direction[0]) {
        case LEFT: x2 = x - 1; y2 = y + 1; break;
        case RIGHT: x2 = x + 1; y2 = y - 1; break;
        case UP: x2 = x + 1; y2 = y + 1; break;
        case DOWN: x2 = x - 1; y2 = y - 1; break;
    }

    std::memcpy(fieldCopy, field, sizeof(20 * 20 * sizeof(DIRECTION)));
    if (fillAlgorithm(x1, y1) > fillAlgorithm(x2, y2)) {
        fillWater(x2, y2);
    } else {
        fillWater(x1, y1);
    }
}

bool World::checkSeparated(int x, int y, DIRECTION * direction) {
    int cur_dir = 0;

    if (arrivesWater(x, y, LEFT)) {
        cur_dir++;
        direction[cur_dir] = LEFT;
    }
    if (arrivesWater(x, y, RIGHT)) {
        cur_dir++;
        direction[cur_dir] = RIGHT;
    }
    if (arrivesWater(x, y, UP)) {
        cur_dir++;
        direction[cur_dir] = UP;
    }
    if (arrivesWater(x, y, DOWN)) {
        cur_dir++;
        direction[cur_dir] = DOWN;
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





















