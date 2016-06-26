//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_PLAYER_H
#define DIGDUG2_PLAYER_H

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "entity.h"
#include "world.h"
#include "enemy.h"

#define FALLING_SOUND  "resource/falling.wav"
#define AIRBLAST_SOUND "resource/airblast.wav"

typedef struct {
    float x;
    float y;
    float z;
    float rotx;
    float roty;
} position_t;

class Player : Entity {
public:
    Player(World * world, sf::Window * window);

    void setCamera();
    void event(sf::Event event);
    void update(float dt);
    void render();
    void forceRender();

    enum {
        FirstPerson,
        ThirdPerson,
        Above2D
    } camera;

private:
    void generateCrack();
    int getCurrentGridX();
    int getCurrentGridZ();
    Direction getDirection();

    position_t current;

    bool leftPressed = false;
    bool rightPressed = false;
    bool upPressed = false;
    bool downPressed = false;
    bool rotLeftPressed = false;
    bool rotRightPressed = false;
    bool pushEnemy = true;
    bool falling = false;

    int xDeltaMouse = 0;
    int yDeltaMouse = 0;

    sf::SoundBuffer fallingSoundBuffer;
    sf::Sound fallingSound;

    sf::SoundBuffer airBlastSoundBuffer;
    sf::Sound airBlastSound;

    World * world;
    sf::Window * window;

    sf::Clock pushEnemyClock;

    GLMmodel * playerModel;

    void loadAudio();

    void changeCamera();
};


#endif //DIGDUG2_PLAYER_H
