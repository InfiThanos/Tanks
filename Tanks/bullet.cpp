#include "bullet.h"

using namespace sf;

Bullet::Bullet(Image &image, float X, float Y, int W, int H, std::string Name, int dir) : Entity(image, X, Y, W, H, Name){
    x += 12;
    y += 12;
    direction = dir;
    speed = 0.8;
    //выше инициализация в конструкторе
}
void Bullet::update(float time)
{
    switch (direction)
    {
        case left: dx = -speed; dy = 0; break; // state = left
        case right: dx = speed; dy = 0; break; // state = right
        case up: dx = 0; dy = -speed; break; // state = up
        case down: dx = 0; dy = speed; break; // state = down
    }
    if (life){
        x += dx*time;//само движение пули по х
        y += dy*time;//по у
        if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров
        //она случайно не вылетела за предел карты и не было ошибки
        if (y <= 0) y = 20;
        if (x >= 1280) x = 1260;// задержка пули в правой стене, чтобы при проседании
        //кадров она случайно не вылетела за предел карты и не было ошибки
        if (y >= 736) y = 716;
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0') //если элемент тайлик стены, то
                    life = false;         //то пуля исчезает
                if (TileMap[i][j] == 'x') { //если тайлик кирпичной стены и пуля игрока
                    if (name == "HeroBullet") TileMap[i][j] = 'X';  // то этот кусок стены разрушается
                    life = false;           //пуля исчезает
                }
            }
            sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию пули
    }
}
