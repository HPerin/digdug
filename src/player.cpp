//
// Created by lucas on 30/04/16.
//

#include <SFML/OpenGL.hpp>
#include <iostream>
#include <player.h>
#include "player.h"
#include "data.h"

bool C3DObject_Load_New_Player(const char *pszFilename, GLMmodel **model) {
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

Player::Player(World * world, sf::Window * window) {
    this->world = world;
    this->window = window;
    current = { (float) -world->initialPlayerPosition.x, 0, (float) -world->initialPlayerPosition.y, 0, 0 };

    camera = ThirdPerson;

    C3DObject_Load_New_Player("resource/drone/drone.obj", &playerModel);

    loadAudio();
}

void Player::loadAudio() {
    fallingSoundBuffer.loadFromFile(FALLING_SOUND);
    fallingSound.setBuffer(fallingSoundBuffer);
    fallingSound.setVolume(0.5);

    airBlastSoundBuffer.loadFromFile(AIRBLAST_SOUND);
    airBlastSound.setBuffer(airBlastSoundBuffer);
}

void Player::setCamera() {
    if (camera == FirstPerson) {
        glRotatef(current.rotx, 1.f, 0.f, 0.f);
        glRotatef(current.roty, 0.f, 1.f, 0.f);
        glTranslatef(current.x, current.y, current.z);
    } else if (camera == ThirdPerson) {
        glTranslatef(0.0f, -0.5f, -3.0f);
        glRotatef(current.rotx, 1.f, 0.f, 0.f);
        glRotatef(current.roty, 0.f, 1.f, 0.f);
        glTranslatef(current.x, current.y, current.z);
    } else if (camera == Above2D) {
        glRotatef(90, 1.f, 0.f, 0.f);
        glTranslatef(current.x, current.y, current.z);
        glTranslatef(0.f, -10.f, 0.f);
    }
}

void Player::event(sf::Event event) {
    switch (event.type) {
        case sf::Event::KeyPressed: {
            switch (event.key.code) {
                case sf::Keyboard::W: upPressed = true; break;
                case sf::Keyboard::A: leftPressed = true; break;
                case sf::Keyboard::D: rightPressed = true; break;
                case sf::Keyboard::S: downPressed = true; break;
                case sf::Keyboard::F: pushEnemy = true; break;
                case sf::Keyboard::V: changeCamera(); break;
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

    if (pushEnemy) {
        pushEnemy = false;
        if (pushEnemyClock.getElapsedTime().asSeconds() > 2) {
            airBlastSound.play();
            pushEnemyClock.restart();
            for (Entity * entity : world->enemyList) {
                Enemy * enemy = (Enemy*) entity;
                switch(getDirection()) {
                    case Left:
                        if (std::abs(enemy->getCurrentY() - getCurrentGridZ()) <= 1 && enemy->getCurrentX() < getCurrentGridX() && std::abs(enemy->getCurrentX() - getCurrentGridX()) <= 2)
                            enemy->forceFollow(enemy->getCurrentX() - 2, enemy->getCurrentY());
                        break;
                    case Right:
                        if (std::abs(enemy->getCurrentY() - getCurrentGridZ()) <= 1 && enemy->getCurrentX() > getCurrentGridX() && std::abs(enemy->getCurrentX() - getCurrentGridX()) <= 2)
                            enemy->forceFollow(enemy->getCurrentX() + 2, enemy->getCurrentY());
                        break;
                    case Down:
                        if (enemy->getCurrentY() < getCurrentGridZ() && std::abs(enemy->getCurrentX() - getCurrentGridX()) <= 1 && std::abs(enemy->getCurrentY() - getCurrentGridZ()) <= 2)
                            enemy->forceFollow(enemy->getCurrentX(), enemy->getCurrentY() - 2);
                        break;
                    case Up:
                        if (enemy->getCurrentY() > getCurrentGridZ() && std::abs(enemy->getCurrentX() - getCurrentGridX()) <= 1 && std::abs(enemy->getCurrentY() - getCurrentGridZ()) <= 2)
                            enemy->forceFollow(enemy->getCurrentX(), enemy->getCurrentY() + 2);
                        break;
                }
            }
        }
    }

    for (Entity * entity : world->enemyList) {
        Enemy * enemy = (Enemy*) entity;
        if (round(enemy->position.x) == getCurrentGridX() &&
            round(enemy->position.y) == getCurrentGridZ()) {
            falling = true;
            fallingSound.play();
        }

        int dist_x = (int) std::abs(round(enemy->position.x) - getCurrentGridX());
        int dist_y = (int) std::abs(round(enemy->position.y) - getCurrentGridZ());
        int dist = (int) sqrt(dist_x * dist_x + dist_y + dist_y);
        if (dist < 5) {
            enemy->tryFollow(getCurrentGridX(), getCurrentGridZ());
        }
    }

    if (upPressed) {
        current.z += dt * 2 * std::cos(current.roty * PI/180);
        current.x -= dt * 2 * std::sin(current.roty * PI/180);
        if (world->hasStone((int) (-current.x + 1), (int) (-current.z + 1))) {
            current.z -= dt * 2 * std::cos(current.roty * PI/180);
            current.x += dt * 2 * std::sin(current.roty * PI/180);
        }
    } else if (downPressed) {
        current.z -= dt * 2 * std::cos(current.roty * PI/180);
        current.x += dt * 2 * std::sin(current.roty * PI/180);
        if (world->hasStone((int) (-current.x + 1), (int) (-current.z + 1))) {
            current.z += dt * 2 * std::cos(current.roty * PI/180);
            current.x -= dt * 2 * std::sin(current.roty * PI/180);
        }
    }
    if (leftPressed) {
        current.z += dt * 2 * std::sin(current.roty * PI/180);
        current.x += dt * 2 * std::cos(current.roty * PI/180);
        if (world->hasStone((int) (-current.x + 1), (int) (-current.z + 1))) {
            current.z -= dt * 2 * std::sin(current.roty * PI/180);
            current.x -= dt * 2 * std::cos(current.roty * PI/180);
        }
    }
    if (rightPressed) {
        current.z -= dt * 2 * std::sin(current.roty * PI/180);
        current.x -= dt * 2 * std::cos(current.roty * PI/180);
        if (world->hasStone((int) (-current.x + 1), (int) (-current.z + 1))) {
            current.z += dt * 2 * std::sin(current.roty * PI/180);
            current.x += dt * 2 * std::cos(current.roty * PI/180);
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
    if (camera == FirstPerson) return;

    forceRender();
}

void Player::generateCrack() {
    world->generateCrack(getCurrentGridX(), getCurrentGridZ(), getDirection());
}

Direction Player::getDirection() {
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
    return direction;
}

int Player::getCurrentGridX() {
    return int(-current.x + 1);
}

int Player::getCurrentGridZ() {
    return int(-current.z + 1);
}

void Player::changeCamera() {
    if (camera == FirstPerson) camera = ThirdPerson;
    else if (camera == ThirdPerson) camera = Above2D;
    else camera = FirstPerson;
}

void Player::forceRender() {
    glPushMatrix();
    GLfloat emission[] = { 0.5f, 0.5f, 0.5f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glColor3f(1.f, 1.f, 1.f);
    glTranslatef((GLfloat) (std::abs(current.x)), -current.y*2, (GLfloat) (std::abs(current.z)));
    glRotatef(-current.roty, 0.f, 1.f, 0.f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glTranslatef(-0.5f, 0.0f, -0.5f);
    glmDraw(playerModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    GLfloat noEmission[] = { 0.f, 0.f, 0.f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glPopMatrix();

    /*glPushMatrix();
    glScalef(1.0f, 0.5f, 1.0f);
    glTranslatef((GLfloat) (std::abs(current.x)), -current.y*2, (GLfloat) (std::abs(current.z)));
    glRotatef(-current.roty, 0.f, 1.f, 0.f);
    glTranslatef(-0.5f, 0.0f, -0.5f);

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

    glVertex3f  ( 0.f, 0.f, 0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glEnd();

    glPopMatrix();*/
}











