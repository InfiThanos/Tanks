#include "enemy.h"

using namespace sf;

// Конструктор
Enemy::Enemy(Image &image, float X, float Y, int W, int H, std::string Name) : Entity(image, X,
                                                                                      Y, W, H, Name){
    if (Name == "EasyEnemy"){
        // Задаем спрайту один прямоугольник для
        // вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (4); // Направление движения врага задаём случайным образом
        speed = 0.12; // Даем скорость, этот объект всегда двигается
        shootLimitTime = rand() % (3000) + 1000;  // Враг может делать выстрел в интервале от 1 до 4 сек
        shootTimer = 0;
        shootReady = false;  // Выстрел можно будет делать по окончании случайного времени shootTime
    }
    if (Name == "MediumEnemy"){
        // Задаем спрайту один прямоугольник для
        // вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (4); // Направление движения врага задаём случайным образом
        speed = 0.1; // Даем скорость, этот объект всегда двигается
        shootLimitTime = rand() % (3000) + 1000;  // Враг может делать выстрел в интервале от 1 до 4 сек
        shootTimer = 0;
        shootReady = false;  // Выстрел можно будет делать по окончании случайного времени shootTime
    }
    if (Name == "HardEnemy"){
        // Задаем спрайту один прямоугольник для
        // вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (4); // Направление движения врага задаём случайным образом
        speed = 0.08; // Даем скорость, этот объект всегда двигается
        shootLimitTime = rand() % (3000) + 1000;  // Враг может делать выстрел в интервале от 1 до 4 сек
        shootTimer = 0;
        shootReady = false;  // Выстрел можно будет делать по окончании случайного времени shootTime
    }
}
void Enemy::checkCollisionWithMap(float Dx, float Dy, Map& map) // Ф-ция проверки столкновений с картой
{
    std::string TileMap = map.getTileMap();
    int l_map = map.getWidthMap();

    for (int i = y / 32; i < (y + h) / 32; i++) // Проходимся по элементам карты
        for (int j = x / 32; j<(x + w) / 32; j++)
        {
            if (TileMap[i*l_map+j] == '0' || TileMap[i*l_map+j] == 'x') // Если элемент - тайлик стены
            {
                if (Dy > 0) {
                    y = i * 32 - h; dy = -speed;
                    direction = rand() % (4); // Направление движения врага
                } // Столкновение с нижними краями
                if (Dy < 0) {
                    y = i * 32 + 32; dy = speed;
                    direction = rand() % (4); // Направление движения врага
                } // С верхними краями
                if (Dx > 0) {
                    x = j * 32 - w; dx = -speed;
                    direction = rand() % (4); // Направление движения врага
                } // С правым краем карты
                if (Dx < 0) {
                    x = j * 32 + 32; dx = speed;
                    direction = rand() % (4); //Направление движения врага
                } // С левым краем карты
            }
        }
}
void Enemy::update(float& time, Map& map)
{
    if (name == "EasyEnemy"){ // Для персонажа с таким именем логика будет такой
        if (life) { // Проверяем, жив ли враг
            switch (direction)// Делаются различные действия в зависимости от состояния
            {
                case right:{ // Состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 3*52, 52, 52));
                    dy = 0;
                    break;
                }
                case left:{ // Идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 52, 52, 52));
                    dy = 0;
                    break;
                }
                case up:{ // Тдти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 0, 52, 52));
                    dx = 0;
                    break;
                }
                case down:{ // Идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 2*52, 52, 52));
                    dx = 0;
                    break;
                }
            }
            x += dx*time; // Движение по “X”
            checkCollisionWithMap(dx, 0, map); // Обрабатываем столкновение по Х
            y += dy*time; // Движение по “Y”
            checkCollisionWithMap(0, dy, map); // Обрабатываем столкновение по Y
            sprite.setPosition(x, y); // Спрайт в позиции (x, y).
            if (health <= 0){ life = false; } // Если жизней меньше 0, либо равно 0, то умираем
            if ((rand() % (10000)) == 10) {  // Случайно меняем направление движения врага
                direction = rand() % (4); }
        }
    }
    if (name == "MediumEnemy"){ // Для персонажа с таким именем логика будет такой
        if (life) { // Проверяем, жив ли враг
            switch (direction) // Делаются различные действия в зависимости от состояния
            {
                case right:{ // Состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 3*52, 52, 52));
                    dy = 0;
                    break;
                }
                case left:{ // Идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 52, 52, 52));
                    dy = 0;
                    break;
                }
                case up:{ // Идти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 0, 52, 52));
                    dx = 0;
                    break;
                }
                case down:{ // Идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 2*52, 52, 52));
                    dx = 0;
                    break;
                }
            }
            x += dx*time; // Движение по “X”
            checkCollisionWithMap(dx, 0, map); // Обрабатываем столкновение по Х
            y += dy*time; // Движение по “Y”
            checkCollisionWithMap(0, dy, map); // Обрабатываем столкновение по Y
            sprite.setPosition(x, y); // Спрайт в позиции (x, y).
            if (health <= 0){ life = false; } // Усли жизней меньше 0, либо равно 0, то умираем
            if ((rand() % (10000)) == 10) { // Случайно меняем направление движения врага
                direction = rand() % (4); }
        }
    }
    if (name == "HardEnemy"){ // Для персонажа с таким именем логика будет такой
        if (life) { // Проверяем, жив ли враг
            switch (direction) // Делаются различные действия в зависимости от состояния
            {
                case right:{ // Состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 3*52, 52, 52));
                    dy = 0;
                    break;
                }
                case left:{ // Идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 52, 52, 52));
                    dy = 0;
                    break;
                }
                case up:{ // Идти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 0, 52, 52));
                    dx = 0;
                    break;
                }
                case down:{ // Идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 2) CurrentFrame -= 2;
                    sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 2*52, 52, 52));
                    dx = 0;
                    break;
                }
            }
            x += dx*time; // Движение по “X”
            checkCollisionWithMap(dx, 0, map); // Обрабатываем столкновение по Х
            y += dy*time; //движение по “Y”
            checkCollisionWithMap(0, dy, map);// Обрабатываем столкновение по Y
            sprite.setPosition(x, y); // Спрайт в позиции (x, y).
            if (health <= 0){ life = false; } // Усли жизней меньше 0, либо равно 0, то умираем
            if ((rand() % (10000)) == 10) { // Случайно меняем направление движения врага
                direction = rand() % (4); }
        }
    }
    // Логика для всех врагов
    // Выдерживаем случайную паузу перед следующим выстрелом
    shootTimer += time;
    if (shootTimer > shootLimitTime) {
        shootReady = true;
        shootTimer = 0;
        shootLimitTime = rand() % (3000) + 1000; // Не менее 1 сек и не более 4 сек
    }
}
