#include "impact.h"

using namespace sf;

Impact::Impact(Image &image, float X, float Y, int W, int H, std::string Name) : Entity(image, X,
                                                                                        Y, W, H, Name){
    //Задаем спрайту один прямоугольник для
    //вывода. IntRect – для приведения типов
    sprite.setTextureRect(IntRect(0, 0, w, h));
    impactLimitTime = 300;  // задаем время "жизни" взрыва
    impactTimer = 0;
}

void Impact::update(float& time, Map& map)
{
    if (life)
    {
        sprite.setPosition(x, y); //задается позицию взрыва
        impactTimer += time;
        if (impactTimer >= impactLimitTime)
            life = false;
    }

}
