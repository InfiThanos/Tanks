#ifndef PLAYER_H
#define PLAYER_H
#include "entity.h"

using namespace sf;

class Player : public Entity { // класс Игрока
public:
    int playerScore;
    int gunDirect;     //направление орудия танка (куда полетит снаряд)
    bool rechargeGun;  //готовность к стрельбе (орудие перезаряжено)
    int ammo;    //боеклмплект (количество снарядов)

    Player(Image &image, float X, float Y, int W, int H, std::string Name);

    void control();
    void checkCollisionWithMap(float Dx, float Dy);
    void update(float time);
};


#endif // PLAYER_H
