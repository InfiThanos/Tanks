#include "enemy.h"

using namespace sf;

// конструктор
Enemy::Enemy(Image &image, float X, float Y, int W, int H, std::string Name) : Entity(image, X,
                                                                                      Y, W, H, Name){
    if (name == "EasyEnemy"){
        //Задаем спрайту один прямоугольник для
        //вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (4); //Направление движения врага задаём случайным образом
        speed = 0.1; //даем скорость, этот объект всегда двигается
        shootLimitTime = rand() % (3000) + 1000;  // враг может делать выстрел в интервале от 1 до 4 сек
        shootTimer = 0;
        shootReady = false;  // выстрел можно будет делать по окончании случайного времени shootTime
    }
}
void Enemy::checkCollisionWithMap(float Dx, float Dy) //ф-ция проверки столкновений с картой
{
    for (int i = y / 32; i < (y + h) / 32; i++) //проходимся по элементам карты
        for (int j = x / 32; j<(x + w) / 32; j++)
        {
            if (TileMap[i][j] == '0' || TileMap[i][j] == 'x') //если элемент - тайлик стены
            {
                if (Dy > 0) {
                    y = i * 32 - h; dy = -speed;
                    direction = rand() % (4); //Направление движения врага
                } //по Y
                if (Dy < 0) {
                    y = i * 32 + 32; dy = speed;
                    direction = rand() % (4); //Направление движения врага
                } //столкновение с верхними краями
                if (Dx > 0) {
                    x = j * 32 - w; dx = -speed;
                    direction = rand() % (4); //Направление движения врага
                } //с правым краем карты
                if (Dx < 0) {
                    x = j * 32 + 32; dx = speed;
                    direction = rand() % (4); //Направление движения врага
                } // с левым краем карты
            }
        }
}
void Enemy::update(float time)
{
    if (name == "EasyEnemy"){ //для персонажа с таким именем логика будет такой
        if (life) { //проверяем, жив ли враг
            switch (direction)//делаются различные действия в зависимости от состояния
            {
                case right:{ //состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 3*52, 52, 52));
                    dy = 0;
                    break;
                }
                case left:{ //состояние идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 52, 52, 52));
                    dy = 0;
                    break;
                }
                case up:{//идти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 0, 52, 52));
                    dx = 0;
                    break;
                }
                case down:{ //идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 2*52, 52, 52));
                    dx = 0;
                    break;
                }
            }
            x += dx*time; //движение по “X”
            checkCollisionWithMap(dx, 0); //обрабатываем столкновение по Х
            y += dy*time; //движение по “Y”
            checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
            sprite.setPosition(x, y); //спрайт в позиции (x, y).
            if (health <= 0){ life = false; } //если жизней меньше 0, либо равно 0, то умираем
            if ((rand() % (10000)) == 10) { //случайно меняем направление движения врага
                direction = rand() % (4); }
        }
    }
    //выдерживаем случайную паузу перед следующим выстрелом
    shootTimer += time;
    if (shootTimer > shootLimitTime) {
        shootReady = true;
        shootTimer = 0;
        shootLimitTime = rand() % (3000) + 1000; //(не менее 1 сек и не более 4 сек)
    }
}
