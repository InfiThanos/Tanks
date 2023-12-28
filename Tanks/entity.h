#ifndef ENTITY_H
#define ENTITY_H

#include <SFML\Graphics.hpp>
#include "map.h"

using namespace sf;

class Entity
{
protected:
    enum { left, right, up, down, stay } state;
    float dx, dy, x, y, speed, moveTimer, CurrentFrame;
    int w, h, health, direction;
    bool life, shootReady;
    Texture texture;
    Sprite sprite;
    std::string name;

public:
    Entity(Image image, float X, float Y, int W, int H, std::string Name);
    FloatRect getRect();
    virtual void update(float& time, Map& map) = 0;
    void getCoordinates(float& X, float& Y);
    void setCoordinates(float X, float Y);
    bool getLife();
    void setLife();
    Sprite getSprite();
    int getDirection();
    void setDirection(int dir);
    bool getShootReady();
    void setShootReady();
    int getHealth();
    void setHealth(int Health);
    std::string getName();
};
#endif // ENTITY_H
