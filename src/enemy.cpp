//
// Created by lucas on 30/04/16.
//

#include <iostream>
#include "enemy.h"

Enemy::Enemy(int x, int y, World * world) {
    position.x = x;
    position.y = y;

    target.x = x;
    target.y = y;

    this->world = world;
}

void Enemy::event(sf::Event event) {
    if (dead) return;
}

void Enemy::update(float dt) {
    if (dead || (!world->hasTerrain(target.x+1, target.y+1) &&
                 !world->hasTerrain(target.x, target.y+1) &&
                 !world->hasTerrain(target.x+2, target.y+1) &&
                 !world->hasTerrain(target.x+1, target.y) &&
                 !world->hasTerrain(target.x+1, target.y+2))) {
        die();
        return;
    }

    if (float(target.x) < position.x) position.x -= dt;
    else if (float(target.x) > position.x) position.x += dt;

    if (float(target.y) < position.y) position.y -= dt;
    else if (float(target.y) > position.y) position.y += dt;

    if (target.x == round(position.x) && target.y == round(position.y)) {
        bool has_terrain = false;
        while(!has_terrain) {
            int x_or_y = rand() % 2;
            int dir = rand() % 3 - 1;
            if (x_or_y == 0) {
                target.x += dir;
            } else {
                target.y += dir;
            }

            has_terrain = world->hasTerrain(target.x+1, target.y+1);

            if (!has_terrain) {
                if (x_or_y == 0) {
                    target.x -= dir;
                } else {
                    target.y -= dir;
                }
            }
        }
    }
}

void Enemy::render() {
    if (dead) return;

    glPushMatrix();
    glScalef(1.0f, 0.5f, 1.0f);
    glTranslatef(position.x, 0, position.y);

    glBegin(GL_QUADS);
    glColor3f   (1.0f,0.0f,0.0f);

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

    glPopMatrix();
}

void Enemy::tryFollow(int x, int y) {
    if (dead) return;

    int tx = target.x, ty = target.y;

    if (x < target.x) tx = (target.x - 1);
    if (x > target.x) tx = (target.x + 1);

    if (y < target.y) ty = (target.y - 1);
    if (y > target.y) ty = (target.y + 1);

    if (world->hasTerrain(tx+1, ty+1)) {
        target.x = tx;
        target.y = ty;
    }
}

void Enemy::die() {
    dead = true;
    position.x = -1;
    position.y = -1;
    target.x = -1;
    target.y = -1;
}











