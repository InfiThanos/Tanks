#ifndef PLAYER_H
#define PLAYER_H
#include "entity.h"

using namespace sf;

class Player : public Entity { // класс Игрока
public:
    int playerScore;

//    Player(Image &image, float X, float Y, int W, int H, std::string Name) : Entity(image, X,
//                                                                                        Y, W, H, Name) {}

    Player(Image &image, float X, float Y, int W, int H, std::string Name);

    void control();
    void checkCollisionWithMap(float Dx, float Dy);
    void update(float time);
};


#endif // PLAYER_H
