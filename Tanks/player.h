#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "entity.h"
//#include "map.h"

using namespace sf;

class Player : public Entity // Класс Игрока
{
private:
    int playerScore;
    int gunDirect;     // Направление орудия танка (куда полетит снаряд)
    bool rechargeGun;  // Готовность к стрельбе (орудие перезаряжено)
    int ammo;          // Боеклмплект (количество снарядов)

public:
    Player(Image &image, float X, float Y, int W, int H, std::string Name);
    void control();
    void checkCollisionWithMap(float Dx, float Dy, Map& map);
    void update(float& time, Map& map);
    int getGunDirect();
    int getAmmo();
    void setAmmo();
    bool getRechargeGun();
    void setRechargeGun();
    int getPlayerScore();
};

#endif // PLAYER_H
