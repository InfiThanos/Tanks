#ifndef BULLET_H
#define BULLET_H
#include "entity.h"

using namespace sf;

class Bullet : public Entity
{
public:
    int direction;
//    Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir) : Entity(image, X, Y, W, H, Name){}
    Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir);

    void update(float time);
};

#endif // BULLET_H
