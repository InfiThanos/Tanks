#ifndef ENEMY_H
#define ENEMY_H
#include "entity.h"
#include "map.h"

using namespace sf;

class Enemy : public Entity
{
private:
    float shootLimitTime;
    float shootTimer;

public:
    Enemy(Image image, float X, float Y, int W, int H, std::string Name);
    void checkCollisionWithMap(float Dx, float Dy, Map& map);
    void update(float& time, Map& map);
};

#endif // ENEMY_H
