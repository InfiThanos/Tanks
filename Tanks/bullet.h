#ifndef BULLET_H
#define BULLET_H
#include "entity.h"
#include "map.h"

using namespace sf;

class Bullet : public Entity
{
public:
    Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir);
    void update(float& time, Map& map);
};

#endif // BULLET_H
