//
// Created by lucas on 30/04/16.
//

#include <iostream>
#include <enemy.h>
#include "game.h"
#include "data.h"

Game::Game() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    settings.depthBits = 24;

    window = new sf::Window(sf::VideoMode(800, 600), "DIG DUG II", sf::Style::Default, settings);
    window->setVerticalSyncEnabled(1);
    window->setMouseCursorVisible(false);

    windowReshape();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    World * world = new World();
    player = new Player(world, window);

    entities.push_back((Entity *const &) player);
    entities.push_back((Entity *const &) world);
    entities.push_back((Entity *const &) new Enemy());
}

Game::~Game() {
    delete window;
}

void Game::run() {
    while(window->isOpen()) {
        stepRender();
        stepEvents();
        stepUpdates();
    }
}

void Game::windowReshape() {
    glViewport      (0, 0, window->getSize().x, window->getSize().y);

    glMatrixMode    (GL_PROJECTION);
    glLoadIdentity  ();
    setPerspective  (45, float(window->getSize().x)/float(window->getSize().y), 0.1f, 1000.f);

    glMatrixMode    (GL_MODELVIEW);
    glLoadIdentity  ();
}

void Game::stepEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window->close();
                break;
            case sf::Event::Resized:
                windowReshape();
                break;
            default: break;
        }

        for (Entity * entity : entities) {
            entity->event(event);
        }
    }
}

void Game::stepRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(10.f, 10.f, 10.f);

    player->setCamera();
    for (Entity * entity : entities) {
        entity->render();
    }

    glFlush();

    window->display();
}

void Game::setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    GLdouble fW, fH;
    fH = tan( fovY / 360 * PI ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void Game::stepUpdates() {
    sf::Time elapsed = clock.getElapsedTime();
    clock.restart();

    for (Entity * entity : entities) {
        entity->update(elapsed.asSeconds());
    }
}











