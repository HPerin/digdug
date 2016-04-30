//
// Created by lucas on 30/04/16.
//

#include <SFML/OpenGL.hpp>
#include <iostream>
#include "player.h"

Player::Player(World * world, sf::Window * window) {
    this->world = world;
    this->window = window;
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
                case sf::Keyboard::W:
                case sf::Keyboard::Up: upPressed = true; break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left: leftPressed = true; break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right: rightPressed = true; break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down: downPressed = true; break;
                case sf::Keyboard::Space: generateCrack(); break;
                default: break;
            }
        } break;
        case sf::Event::KeyReleased: {
            switch (event.key.code) {
                case sf::Keyboard::W:
                case sf::Keyboard::Up: upPressed = false; break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left: leftPressed = false; break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right: rightPressed = false; break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down: downPressed = false; break;
                default: break;
            }
        } break;
        case sf::Event::MouseMoved: {
            unsigned int xMiddle = window->getSize().x / 2;
            unsigned int yMiddle = window->getSize().y / 2;
            if (event.mouseMove.x != xMiddle || event.mouseMove.y != yMiddle) {
                xDeltaMouse = event.mouseMove.x - xMiddle;
                yDeltaMouse = event.mouseMove.y - yMiddle;
                sf::Mouse::setPosition(sf::Vector2i(xMiddle, yMiddle), *window);
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
        current.z += dt * 2 * std::sin(current.roty * PI/180);
        current.x += dt * 2 * std::cos(current.roty * PI/180);
    }
    if (rightPressed) {
        current.z -= dt * 2 * std::sin(current.roty * PI/180);
        current.x -= dt * 2 * std::cos(current.roty * PI/180);
    }

    if (xDeltaMouse) {
        current.roty += 0.25 * xDeltaMouse;
        xDeltaMouse = 0;
    }
    if (yDeltaMouse) {
        current.rotx += 0.25 * yDeltaMouse;
        if (current.rotx > 20)
            current.rotx = 20;
        if (current.rotx < -20)
            current.rotx = -20;
        yDeltaMouse = 0;
    }
}

void Player::render() {

}

void Player::generateCrack() {
    current.roty = int(current.roty) % 360;
    while (current.roty < 0) current.roty += 360;
    DIRECTION direction;
    if (current.roty < 45 || current.roty >= 315) {
        direction = DOWN;
    } else if (current.roty < 135) {
        direction = RIGHT;
    } else if (current.roty < 225) {
        direction = UP;
    } else {
        direction = LEFT;
    }
    world->generateCrack((int) std::abs(current.x) + 1, (int) std::abs(current.z) + 1, direction);
}

void Player::resetMouse(int x, int y) {
    xOldMouse = x;
    yOldMouse = y;
}





