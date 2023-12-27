#include "entity.h"

using namespace sf;

Entity::Entity(Image &image, float X, float Y, int W, int H, std::string Name){
    x = X; y = Y; //координата появления спрайта
    w = W; h = H;
    name = Name;
    moveTimer = 0;
    dx = 0; dy = 0;
    speed = 0;
    CurrentFrame = 0;
    health = 100;
    life = true; //инициализировали логическую переменную жизни, герой жив
    texture.loadFromImage(image); //заносим наше изображение в текстуру
    sprite.setTexture(texture);   //заливаем спрайт текстурой
}

FloatRect Entity::getRect(){     //метод получения прямоугольника. его коорд, размеры (шир,высот).
    FloatRect FR(x, y, w, h);    //переменная FR типа FloatRect
    return FR;
    //Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
    //в нашей игре это координаты текущего расположения тайла на карте
    //далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
    //эта ф-ция нужна для проверки пересечений
}

void Entity::getCoordinates(float& X, float& Y)
{
    X = x;
    Y = y;
}

void Entity::setCoordinates(float X, float Y)
{
    x = X;
    y = Y;
}

bool Entity::getLife()
{
    return life;
}

void Entity::setLife()
{
    life = !life;
}

Sprite Entity::getSprite()
{
    return sprite;
}

int Entity::getDirection()
{
    return direction;
}

void Entity::setDirection(int dir)
{
    direction = dir;
}

bool Entity::getShootReady()
{
    return shootReady;
}

void Entity::setShootReady()
{
    shootReady = !shootReady;
}

int Entity::getHealth()
{
    return health;
}

void Entity::setHealth(int Health)
{
    health -= Health;
}

std::string Entity::getName()
{
    return name;
}


