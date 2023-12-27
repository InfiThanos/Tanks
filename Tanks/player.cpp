#include "player.h"

////////////////////////////КЛАСС ИГРОКА////////////////////////////

// Конструктор
Player::Player(Image &image, float X, float Y, int W, int H, std::string Name) : Entity(image, X,
                                                                                        Y, W, H, Name)
{
    playerScore = 0;
    state = down;
    gunDirect = down;
    rechargeGun = true;
    ammo = 10;
    if (name == "Player1")
    {
      // Задаем спрайту один прямоугольник для
      // вывода одного игрока. IntRect – для приведения типов
      sprite.setTextureRect(IntRect(0, 0, w, h));
      // Задаем начальное количество снарядов (необходимо будет пополнять)
    }
}

// Метод для управления игроком
void Player::control()
{
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        state = left;
        gunDirect = left;
        speed = 0.1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        state = right;
        gunDirect = right;
        speed = 0.1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        state = up;
        gunDirect = up;
        speed = 0.1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        state = down;
        gunDirect = down;
        speed = 0.1;
    }
}

// Метод проверки столкновений с элементами карты
void Player::checkCollisionWithMap(float Dx, float Dy, Map& map)
{
    std::string TileMap = map.getTileMap();
    int l_map = map.getWidthMap();

    for (int i = y / 32; i < (y + h) / 32; i++) // Проходимся по элементам карты
        for (int j = x / 32; j<(x + w) / 32; j++)
        {
            if (TileMap[i*l_map+j] == '0' || TileMap[i*l_map+j] == 'x') // Если элемент тайлик стены
            {
                if (Dy > 0) { y = i * 32 - h; dy = 0; }  // По Y
                if (Dy < 0) { y = i * 32 + 32; dy = 0; } // Столкновение с верхними краями
                if (Dx > 0) { x = j * 32 - w; dx = 0; }  // С правым краем карты
                if (Dx < 0) { x = j * 32 + 32; dx = 0; } // С левым краем карты
            }
            if (TileMap[i*l_map+j] == 'a') {
                ammo = 10; // Пополнили боекомплект
                TileMap[i*l_map+j] = '_';
            }
            if (TileMap[i*l_map+j] == 'r') {
                health = 100; // Восстановили здоровье
                TileMap[i*l_map+j] = '_';
            }
            if (TileMap[i*l_map+j] == 's') {
                playerScore += 100; // Очки в игре
                TileMap[i*l_map+j] = '_';
            }
        }
    map.setTileMap(TileMap);
}

// Метод "оживления/обновления" объекта класса
void Player::update(float& time, Map& map)
{
    if (life) { // Проверяем, жив ли герой
        control(); // Функция управления персонажем
        switch (state) // Делаются различные действия в зависимости от состояния
        {
        case right:{ // Состояние идти вправо
            dx = speed;
            CurrentFrame += 0.005*time;
            if (CurrentFrame > 2) CurrentFrame -= 2;
            sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 3*52, 52, 52));
            break;
        }
        case left:{ // Cостояние идти влево
            dx = -speed;
            CurrentFrame += 0.005*time;
            if (CurrentFrame > 2) CurrentFrame -= 2;
            sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 52, 52, 52));
            break;
        }
        case up:{ // Идти вверх
            dy = -speed;
            CurrentFrame += 0.005*time;
            if (CurrentFrame > 2) CurrentFrame -= 2;
            sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 0, 52, 52));
            break;
        }
        case down:{ // Идти вниз
            dy = speed;
            CurrentFrame += 0.005*time;
            if (CurrentFrame > 2) CurrentFrame -= 2;
            sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 2*52, 52, 52));
            break;
        }
        case stay:{ // Стоим
            dx = 0;
            dy = 0;
            break;
        }
        }
        x += dx*time; // Движение по “X”
        checkCollisionWithMap(dx, 0, map); // Обрабатываем столкновение по Х
        y += dy*time; // Движение по “Y”
        checkCollisionWithMap(0, dy, map); // Обрабатываем столкновение по Y
        speed = 0; // Обнуляем скорость, чтобы персонаж остановился.
        sprite.setPosition(x, y); // Спрайт в позиции (x, y).
        if (health <= 0){ life = false; } // Если жизней меньше 0, либо равно 0, то умираем
        state = stay;
        dx = 0;
        dy = 0;
    }
}

int Player::getGunDirect()
{
    return gunDirect;
}

int Player::getAmmo()
{
    return ammo;
}

void Player::setAmmo()
{
    ammo -= 1;
}

bool Player::getRechargeGun()
{
    return rechargeGun;
}

void Player::setRechargeGun()
{
    rechargeGun = !rechargeGun;
}

int Player::getPlayerScore()
{
    return playerScore;
}



