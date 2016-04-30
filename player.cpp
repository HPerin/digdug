//
// Created by lucas on 30/04/16.
//

#include <SFML/OpenGL.hpp>
#include <iostream>
#include "player.h"

Player::Player() {
    current = { 0, 0, 0, 0, 0 };
}

void Player::setCamera() {
    glRotatef(current.rotx, 1.f, 0.f, 0.f);
    glRotatef(current.roty, 0.f, 1.f, 0.f);
    glTranslatef(current.x, current.y, current.z);
}

void Player::event(sf::Event event) {
    switch (event.type) {
        case sf::Event::KeyPressed: {
            switch (event.key.code) {
                case sf::Keyboard::Up: upPressed = true; break;
                case sf::Keyboard::Left: leftPressed = true; break;
                case sf::Keyboard::Right: rightPressed = true; break;
                case sf::Keyboard::Down: downPressed = true; break;
                default: break;
            }
        } break;
        case sf::Event::KeyReleased: {
            switch (event.key.code) {
                case sf::Keyboard::Up: upPressed = false; break;
                case sf::Keyboard::Left: leftPressed = false; break;
                case sf::Keyboard::Right: rightPressed = false; break;
                case sf::Keyboard::Down: downPressed = false; break;
                default: break;
            }
        } break;
        default: break;
    }
}

void Player::update(float dt) {
    static const float PI = 3.1415;

    if (upPressed) {
        current.z += dt * 2 * std::cos(current.roty * PI/180);
        current.x -= dt * 2 * std::sin(current.roty * PI/180);
    } else if (downPressed) {
        current.z -= dt * 2 * std::cos(current.roty * PI/180);
        current.x += dt * 2 * std::sin(current.roty * PI/180);
    }

    if (leftPressed) {
        current.roty -= dt * 100.f;
    }

    if (rightPressed) {
        current.roty += dt * 100.f;
    }
}

void Player::render() {

}



