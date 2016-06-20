//
// Created by lucas on 30/04/16.
//

#include <SFML/OpenGL.hpp>
#include <iostream>
#include <player.h>
#include "player.h"
#include "data.h"

Player::Player(World * world, sf::Window * window) {
    this->world = world;
    this->window = window;
    current = { -10, 0, -10, 0, 0 };

    loadAudio();
}

void Player::loadAudio() {
    footstepSoundBuffer.loadFromFile(FOOTSTEP_SOUND);
    footstepSound.setBuffer(footstepSoundBuffer);

    fallingSoundBuffer.loadFromFile(FALLING_SOUND);
    fallingSound.setBuffer(fallingSoundBuffer);
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
                case sf::Keyboard::W: upPressed = true; break;
                case sf::Keyboard::A: leftPressed = true; break;
                case sf::Keyboard::D: rightPressed = true; break;
                case sf::Keyboard::S: downPressed = true; break;
                case sf::Keyboard::Left: rotLeftPressed = true; break;
                case sf::Keyboard::Right: rotRightPressed = true; break;
                case sf::Keyboard::Space: generateCrack(); break;
                default: break;
            }
        } break;
        case sf::Event::KeyReleased: {
            switch (event.key.code) {
                case sf::Keyboard::W: upPressed = false; break;
                case sf::Keyboard::A: leftPressed = false; break;
                case sf::Keyboard::D: rightPressed = false; break;
                case sf::Keyboard::S: downPressed = false; break;
                case sf::Keyboard::Left: rotLeftPressed = false; break;
                case sf::Keyboard::Right: rotRightPressed = false; break;
                default: break;
            }
        } break;
        case sf::Event::MouseMoved: {
            unsigned int xMiddle = window->getSize().x / 2;
            unsigned int yMiddle = window->getSize().y / 2;
            if (event.mouseMove.x != int(xMiddle) || event.mouseMove.y != int(yMiddle)) {
                xDeltaMouse = event.mouseMove.x - xMiddle;
                yDeltaMouse = event.mouseMove.y - yMiddle;
                sf::Mouse::setPosition(sf::Vector2i(xMiddle, yMiddle), *window);
            }
        } break;
        default: break;
    }
}

void Player::update(float dt) {
    if (!falling && world->hasWater(getCurrentGridX(), getCurrentGridZ())) {
        falling = true;
        fallingSound.play();
    }

    if (falling) {
        current.y += 0.5f * dt;
        if (fallingSound.getStatus() != sf::SoundSource::Status::Playing)
            window->close();
        return;
    }

    for (Entity * entity : world->enemyList) {
        Enemy * enemy = (Enemy*) entity;
        if (round(enemy->position.x+1) == std::abs(round(current.x)) &&
            round(enemy->position.y+1) == std::abs(round(current.z))) {
            falling = true;
            fallingSound.play();
        }

        int dist_x = (int) std::abs(round(enemy->position.x + 1) - std::abs(round(current.x)));
        int dist_y = (int) std::abs(round(enemy->position.y + 1) - std::abs(round(current.z)));
        int dist = (int) sqrt(dist_x * dist_x + dist_y + dist_y);
        if (dist < 5) {
            enemy->tryFollow((int) (std::abs(round(current.x)) - 1), (int) (std::abs(round(current.z))) - 1);
        }
    }

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

    if (upPressed || downPressed || leftPressed || rightPressed) {
        if (footstepSound.getStatus() != sf::SoundSource::Status::Playing) {
            footstepSound.play();
        }
    }

    if (rotLeftPressed) {
        current.rotx = 10;
        current.roty -= 180 * dt;
    } else if (rotRightPressed) {
        current.rotx = 10;
        current.roty += 180 * dt;
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
    Direction direction;
    if (current.roty < 45 || current.roty >= 315) {
        direction = Down;
    } else if (current.roty < 135) {
        direction = Right;
    } else if (current.roty < 225) {
        direction = Up;
    } else {
        direction = Left;
    }
    world->generateCrack(getCurrentGridX(), getCurrentGridZ(), direction);
}

int Player::getCurrentGridX() {
    return int(-current.x + 1);
}

int Player::getCurrentGridZ() {
    return int(-current.z + 1);
}









