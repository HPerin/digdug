//
// Created by lucas on 30/04/16.
//

#include <GL/glew.h>
#include "enemy.h"

Enemy::Enemy() {

}

void Enemy::event(sf::Event event) {

}

void Enemy::update(float dt) {

}

void Enemy::render() {
    glPushMatrix();
    glTranslatef(10, 0, 10);
    glScalef(2.f, 2.f, 2.f);

    glPopMatrix();
}









