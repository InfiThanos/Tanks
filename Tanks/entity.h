#ifndef ENTITY_H
#define ENTITY_H
#include <SFML\Graphics.hpp>
#include <sstream>
#include "map.h"

using namespace sf;

class Entity
{
public:
    enum { left, right, up, down, stay} state;
    float dx, dy, x, y, speed, moveTimer;
    int w, h, health;
    bool life;
    Texture texture;
    Sprite sprite;
    float CurrentFrame;
    std::string name;
    Entity(Image &image, float X, float Y, int W, int H, std::string Name);
    FloatRect getRect();
    // FloatRect FR(float x, float y, float w, float h);
    virtual void update(float time) = 0;
};
#endif // ENTITY_H
