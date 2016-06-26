//
// Created by lucas on 30/04/16.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <set>
#include "world.h"
#include "enemy.h"

bool C3DObject_Load_New_World(const char *pszFilename, GLMmodel **model) {
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

World::World() {
    loadField();

    loadTextures();
}

World::~World() {
    freeField();
}

void World::loadTextures() {
    grassTexture.loadFromFile(GRASS_TEXTURE);
    grassTexture.setSmooth(true);

    waterTexture.loadFromFile(WATER_TEXTURE);
    waterTexture.setSmooth(true);

    baseTexture.loadFromFile(BASE_TEXTURE);
    baseTexture.setSmooth(true);

    holeTexture.loadFromFile(HOLE_TEXTURE);
    holeTexture.setSmooth(true);

    crackTexture.loadFromFile(CRACK_TEXTURE);
    crackTexture.setSmooth(true);
}

void World::freeField() {
    for (int x = 0; x < field_width; x++) {
        free(field[x]);
        free(fieldCopy[x]);
    }
    free(field);
    free(fieldCopy);
}

void World::loadField() {
    sf::Image image;
    if (!image.loadFromFile(MAP_BITMAP)) {
        std::cerr << "UNABLE TO LOAD MAP!!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    field_width = image.getSize().x + 2;
    field_height = image.getSize().y + 2;

    field = new GroundType*[field_width];
    for (int x = 0; x < field_width; x++) {
        field[x] = new GroundType[field_height];
    }

    fieldCopy = new GroundType*[field_width];
    for (int x = 0; x < field_width; x++) {
        fieldCopy[x] = new GroundType[field_height];
    }

    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            field[x][y] = Water;
            if (!(x == 0 || x == (field_width - 1) || y == 0 || y == (field_height - 1))) {
                sf::Color color = image.getPixel((unsigned int) x - 1, (unsigned int) y - 1);
                if (color.r == 127) {
                    initialPlayerPosition.x = x;
                    initialPlayerPosition.y = y;
                    field[x][y] = Terrain;
                } else if (color.g == 127) {
                    field[x][y] = Crack;
                } else if (color.r == 255 && color.g == 255) {
                    field[x][y] = Stone;
                } else if (color.g == 255) {
                    field[x][y] = Terrain;
                } else if (color.r == 255 && color.b == 255) {
                    field[x][y] = Hole;
                } else if (color.r == 255) {
                    field[x][y] = Terrain;
                    enemyList.push_back((Entity*) new Enemy(x, y, this));
                }
            }
        }
    }
}

void World::event(sf::Event event) {

}

void World::update(float dt) {
    waterOffset += dt;
    while(waterOffset > 1) waterOffset -= 1;

    deleteCracks();
}

void World::render() {
    for (int x = -field_width; x < 2 * field_width; x++) {
        for (int y = -field_height; y < 2 * field_height; y++) {
            renderWater(x, y);
        }
    }

    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            if (field[x][y] != Water) {
                renderGrass(x - 1, y - 1);
                switch (field[x][y]) {
                    case Stone: renderStone(x - 1, y - 1); break;
                    case Crack: renderCrack(x - 1, y - 1); break;
                    case Hole: renderHole(x - 1, y - 1); break;
                    default: break;
                }
            }

            //renderDelimiter(x-1, y-1);
        }
    }
}

void World::renderWater(int x, int y) {
    glPushMatrix();
    glTranslatef(x, -1.f, y + waterOffset);

    sf::Texture::bind(&waterTexture);

    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 0.f, 1.f);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);

    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderGrass(int x, int y) {
    renderBase(x, y);

    glPushMatrix();
    glTranslatef(  x, -0.5f,   y);

    sf::Texture::bind(&grassTexture);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   ( 0.f, 1.f, 0.f);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);

    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderHole(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.49f,   y);

    sf::Texture::bind(&holeTexture);

    glBegin(GL_QUADS);
    glColor3f   (0.41,0.25,0.14);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderCrack(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.49f,   y);

    sf::Texture::bind(&crackTexture);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::renderDelimiter(int x, int y) {
    glPushMatrix();
    glTranslatef(  x, -0.49f,   y);

    glBegin(GL_LINE_LOOP);
    glColor3f   ( 0.3f, 0.3f, 0.3f);
    glVertex3f  ( 1.f, 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);
    glVertex3f  ( 0.f, 0.f, 0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    glPopMatrix();
}

void World::renderBase(int x, int y) {
    glPushMatrix();
    glTranslatef(x, -0.5f, y);

    sf::Texture::bind(&baseTexture);

    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);

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
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

void World::generateCrack(int x, int y, Direction direction) {
    int ax = x;
    int ay = y;

    if (field[ax][ay] != Hole)
        return;

    switch (direction) {
        case Left: {
            ax--;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ax--;
            }
        } break;
        case Right: {
            ax++;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ax++;
            }
        } break;
        case Up: {
            ay++;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ay++;
            }
        } break;
        case Down: {
            ay--;
            while(field[ax][ay] == Terrain) {
                field[ax][ay] = Crack;
                ay--;
            }
        } break;
    }

    while (destroySeparated());
}

void World::fillBool(bool ** bool_field, int x, int y) {
    if (field[x][y] == Terrain && !bool_field[x][y]) {
        bool_field[x][y] = true;
        fillBool(bool_field, x+1, y);
        fillBool(bool_field, x-1, y);
        fillBool(bool_field, x, y-1);
        fillBool(bool_field, x, y+1);
    }
}

int World::fillSize(bool ** bool_field, int x, int y) {
    if (field[x][y] == Terrain && !bool_field[x][y]) {
        bool_field[x][y] = true;
        return 1 +
               fillSize(bool_field, x+1, y) +
               fillSize(bool_field, x-1, y) +
               fillSize(bool_field, x, y+1) +
               fillSize(bool_field, x, y-1);
    }
    return 0;
}

bool World::destroySeparated() {
    int found_x=-1, found_y=-1;
    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            if (field[x][y] == GroundType::Terrain) {
                if (found_x == -1) found_x = x;
                if (found_y == -1) found_y = y;
            }
        }
    }

    bool ** bool_field = new bool*[field_width];
    for (int x = 0; x < field_width; x++) bool_field[x] = new bool[field_height];
    initFalse(bool_field);
    fillBool(bool_field, found_x, found_y);

    bool is_separated = false;
    int separated_x = -1, separated_y = -1;
    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            if (field[x][y] == GroundType::Terrain && !bool_field[x][y]) {
                is_separated = true;
                separated_x = x;
                separated_y = y;
            }
        }
    }

    if (is_separated) {
        bool ** first_bool_field = new bool*[field_width];
        for (int x = 0; x < field_width; x++) first_bool_field[x] = new bool[field_height];
        initFalse(first_bool_field);
        int first_size = fillSize(first_bool_field, found_x, found_y);

        bool ** second_bool_field = new bool*[field_width];
        for (int x = 0; x < field_width; x++) second_bool_field[x] = new bool[field_height];
        initFalse(second_bool_field);
        int second_size = fillSize(second_bool_field, separated_x, separated_y);

        if (first_size < second_size) fillWater(found_x, found_y);
        else fillWater(separated_x, separated_y);

        for (int x = 0; x < field_width; x++) free(first_bool_field[x]);
        free(first_bool_field);
        for (int x = 0; x < field_width; x++) free(second_bool_field[x]);
        free(second_bool_field);
    }

    for (int x = 0; x < field_width; x++) free(bool_field[x]);
    free(bool_field);

    return is_separated;
}

void World::fillWater(int x, int y) {
    if (field[x][y] == Terrain) {
        field[x][y] = Water;
        fillWater(x+1, y);
        fillWater(x-1, y);
        fillWater(x, y-1);
        fillWater(x, y+1);
    }
}

bool World::hasWater(int x, int y) {
    return x <= 0
           || x >= field_width
           || y <= 0
           || y >= field_height
           || field[x][y] == Water;
}

void World::initFalse(bool ** bool_field) {
    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            bool_field[x][y] = false;
        }
    }
}

bool World::hasTerrain(int x, int y) {
    if (x < 0 || x > field_width) return false;
    if (y < 0 || y > field_height) return false;
    return field[x][y] == GroundType::Terrain;
}

void World::deleteCracks() {
    for (int x = 0; x < field_width; x++) {
        for (int y = 0; y < field_height; y++) {
            if (field[x][y] == Crack || field[x][y] == Hole || field[x][y] == Stone) {
                int counter = 0;
                if (field[x-1][y] == Water) counter++;
                if (field[x+1][y] == Water) counter++;
                if (field[x][y-1] == Water) counter++;
                if (field[x][y+1] == Water) counter++;
                if (counter >= 3) field[x][y] = Water;
            }

            if (field[x][y] == Crack) {
                int counter = 0;
                if (field[x-1][y] == Terrain) counter++;
                if (field[x+1][y] == Terrain) counter++;
                if (field[x][y-1] == Terrain) counter++;
                if (field[x][y+1] == Terrain) counter++;
                if (counter == 0) field[x][y] = Water;
            }
        }
    }
}

void World::renderStone(int x, int y) {
    renderBase(x, y);

    glPushMatrix();
    glTranslatef(  x, 0.5f,   y);

    sf::Texture::bind(&baseTexture);

    /*
     * Draw this positions floor
     */
    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);

    glTexCoord2f( 1.f,      1.f);
    glVertex3f  ( 1.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      1.f);
    glVertex3f  ( 0.f, 0.f, 1.f);

    glTexCoord2f( 0.f,      0.f);
    glVertex3f  ( 0.f, 0.f, 0.f);

    glTexCoord2f( 1.f,      0.f);
    glVertex3f  ( 1.f, 0.f, 0.f);
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, 0.5f, y);

    sf::Texture::bind(&baseTexture);

    glBegin(GL_QUADS);
    glColor3f   (0.55,0.41,0.13);

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
    glEnd();

    sf::Texture::bind(NULL);

    glPopMatrix();
}

bool World::hasStone(int x, int y) {
    return field[x][y] == Stone;
}

























