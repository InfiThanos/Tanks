#ifndef ENEMY_H
#define ENEMY_H
#include "entity.h"

using namespace sf;

class Enemy : public Entity
{
public:
    float shootLimitTime;
    float shootTimer;

    Enemy(Image &image, float X, float Y, int W, int H, std::string Name);

    void checkCollisionWithMap(float Dx, float Dy);
    void update(float time);
};

#endif // ENEMY_H
