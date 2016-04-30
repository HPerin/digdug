//
// Created by lucas on 30/04/16.
//

#include <GL/gl.h>
#include <iostream>
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

    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            field[x][y] = TERRAIN;
        }
    }
}

void World::event(sf::Event event) {

}

void World::update(float dt) {

}

void World::render() {
    renderWater();

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            if (field[x][y] == TERRAIN) {
                renderGround(x, y);
            }
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

    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 1.f, 0.f);

    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f, 1.f);
    glVertex3f  (-1.f, 0.f,-1.f);
    glVertex3f  ( 1.f, 0.f,-1.f);
    glEnd();

    glPopMatrix();
}











