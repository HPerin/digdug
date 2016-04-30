//
// Created by lucas on 30/04/16.
//

#ifndef DIGDUG2_ENTITY_H
#define DIGDUG2_ENTITY_H

#include <SFML/Window.hpp>

class Entity {
public:
    virtual void event(sf::Event event) = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
};


#endif //DIGDUG2_ENTITY_H
