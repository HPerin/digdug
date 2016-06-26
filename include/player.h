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

#define FOOTSTEP_SOUND "resource/footstep.wav"
#define FALLING_SOUND  "resource/falling.wav"

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

    sf::SoundBuffer footstepSoundBuffer;
    sf::Sound footstepSound;

    sf::SoundBuffer fallingSoundBuffer;
    sf::Sound fallingSound;

    World * world;
    sf::Window * window;

    sf::Clock pushEnemyClock;

    void loadAudio();

    void changeCamera();
};


#endif //DIGDUG2_PLAYER_H
