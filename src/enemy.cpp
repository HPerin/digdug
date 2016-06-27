//
// Created by lucas on 30/04/16.
//

#include <iostream>
#include "enemy.h"

bool C3DObject_Load_New_Enemy(const char *pszFilename, GLMmodel **model) {
    char aszFilename[256];
    strcpy(aszFilename, pszFilename);

    if (*model) {

        free(*model);
        *model = NULL;
    }

    *model = glmReadOBJ(aszFilename);
    if (!(*model))
        return false;

    glmUnitize(*model);
    //glmScale(model,sFactor); // USED TO SCALE THE OBJECT
    glmFacetNormals(*model);
    glmVertexNormals(*model, 90.0);

    return true;
}

Enemy::Enemy(int x, int y, World * world) {
    position.x = x;
    position.y = y;

    target.x = x;
    target.y = y;

    this->world = world;
    dead = false;

    C3DObject_Load_New_Enemy("resource/drone/drone.obj", &enemyModel);
}

void Enemy::event(sf::Event event) {
    if (dead) return;
}

void Enemy::update(float dt) {
    if (dead) return;

    if (world->hasWater((int) round(position.x), (int) round(position.y))) {
        die();
        return;
    }

    if (float(target.x) < position.x) position.x -= dt * (accelerated ? 3 : 1);
    else if (float(target.x) > position.x) position.x += dt * (accelerated ? 3 : 1);

    if (float(target.y) < position.y) position.y -= dt * (accelerated ? 3 : 1);
    else if (float(target.y) > position.y) position.y += dt * (accelerated ? 3 : 1);

    if (target.x == round(position.x) && target.y == round(position.y)) {
        accelerated = false;
        following = false;
        int dir_x = rand() % 3 - 1;
        int dir_y = rand() % 3 - 1;
        if (!world->hasTerrain(target.x + dir_x, target.y + dir_y)) {
            dir_x = 0;
            dir_y = 0;
            if (!world->hasTerrain(target.x + dir_x, target.y + dir_y)) {
                dir_x = 1;
                dir_y = 0;
                if (!world->hasTerrain(target.x + dir_x, target.y + dir_y)) {
                    dir_x = 0;
                    dir_y = 1;
                    if (!world->hasTerrain(target.x + dir_x, target.y + dir_y)) {
                        dir_x = 0;
                        dir_y = -1;
                        if (!world->hasTerrain(target.x + dir_x, target.y + dir_y)) {
                            dir_x = -1;
                            dir_y = 0;
                            if (!world->hasTerrain(target.x + dir_x, target.y + dir_y)) {
                                die();
                            }
                        }
                    }
                }
            }
        }
        target.x += dir_x;
        target.y += dir_y;
    }
}

void Enemy::render() {
    if (dead) return;

    forceRender();
}

void Enemy::tryFollow(int x, int y) {
    if (dead) return;
    if (following) return;
    if (accelerated) return;

    int tx = (int) round(position.x), ty = (int) round(position.y);

    if (x < target.x) tx = (target.x - 1);
    if (x > target.x) tx = (target.x + 1);

    if (tx != target.x && world->hasTerrain(tx, ty)) {
        following = true;
        target.x = tx;
        target.y = ty;
        return;
    } else {
        tx = (int) round(position.x);
    }

    if (y < target.y) ty = (target.y - 1);
    if (y > target.y) ty = (target.y + 1);

    if (ty != target.y && world->hasTerrain(tx, ty)) {
        following = true;
        target.x = tx;
        target.y = ty;
        return;
    }
}

void Enemy::die() {
    dead = true;
    position.x = -1;
    position.y = -1;
    target.x = -1;
    target.y = -1;
}

int Enemy::getCurrentX() {
    return (int) round(position.x);
}

int Enemy::getCurrentY() {
    return (int) round(position.y);
}

void Enemy::forceFollow(int x, int y) {
    accelerated = true;
    target.x = x;
    target.y = y;
}

void Enemy::forceRender() {
    int angle = 0;
    if (target.x > round(position.x)) angle = 90;
    else if (target.x < round(position.x)) angle = 270;
    else if (target.y > round(position.y)) angle = 0;
    else if (target.y < round(position.y)) angle = 180;

    glPushMatrix();
    GLfloat emission[] = { 0.5f, 0.f, 0.f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glColor3f   (1.0f,0.0f,0.0f);
    glTranslatef(position.x-0.5f, 0, position.y-0.5f);
    glScalef(0.5f, 0.5f, 0.5f);
    glRotatef(angle, 0.f, 1.f, 0.f);
    glmDraw(enemyModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    GLfloat noEmission[] = { 0.f, 0.f, 0.f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glPopMatrix();

    /*glPushMatrix();
    glScalef(1.0f, 0.5f, 1.0f);
    glTranslatef(position.x-1, 0, position.y-1);

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

    glVertex3f  ( 0.f, 0.0f, 0.f);
    glVertex3f  ( 1.f, 0.0f, 0.f);
    glVertex3f  ( 1.f, 0.0f, 1.f);
    glVertex3f  ( 0.f, 0.0f, 1.f);
    glEnd();

    glPopMatrix();*/
}















