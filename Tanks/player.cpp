#include "player.h"

////////////////////////////КЛАСС ИГРОКА////////////////////////////

Player::Player(Image &image, float X, float Y, int W, int H, std::string Name) : Entity(image, X,     // Конструктор
                                                                                        Y, W, H, Name) {
    playerScore = 0;
    state = down;
    gunDirect = down;
    rechargeGun = true;
    ammo = 10;
    if (name == "Player1"){
      //Задаем спрайту один прямоугольник для
      //вывода одного игрока. IntRect – для приведения типов
      sprite.setTextureRect(IntRect(0, 0, w, h));
      //Задаем начальное количество снарядов (необходимо будет пополнять)
    }
}
void Player::control(){
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
//Метод проверки столкновений с элементами карты
void Player::checkCollisionWithMap(float Dx, float Dy) {
    for (int i = y / 32; i < (y + h) / 32; i++) //проходимся по элементам карты
        for (int j = x / 32; j < (x + w) / 32; j++)
        {
            if (TileMap[i][j] == '0' || TileMap[i][j] == 'x') //если элемент тайлик стены
            {
                if (Dy > 0) { y = i * 32 - h; dy = 0; } //по Y
                if (Dy < 0) { y = i * 32 + 32; dy = 0; } //столкновение с верхними краями
                if (Dx > 0) { x = j * 32 - w; dx = 0; } //с правым краем карты
                if (Dx < 0) { x = j * 32 + 32; dx = 0; } // с левым краем карты
            }
            if (TileMap[i][j] == 'a') {
                ammo = 10; // пополнили боекомплект
                TileMap[i][j] = ' ';
            }
            if (TileMap[i][j] == 'r') {
                health = 100; // восстановили здоровье
                TileMap[i][j] = ' ';
            }
            if (TileMap[i][j] == 's') {
                playerScore += 100; // очки в игре
                TileMap[i][j] = ' ';
            }
        }
}
void Player::update(float time) //метод "оживления/обновления" объекта класса.
{
    if (life) {//проверяем, жив ли герой
        control();//функция управления персонажем
        switch (state)//делаются различные действия в зависимости от состояния
        {
            case right:{//состояние идти вправо
                dx = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 2) CurrentFrame -= 2;
                sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 3*52, 52, 52));
                break;
            }
            case left:{//состояние идти влево
                dx = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 2) CurrentFrame -= 2;
                sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 52, 52, 52));
                break;
            }
            case up:{//идти вверх
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 2) CurrentFrame -= 2;
                sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 0, 52, 52));
                break;
            }
            case down:{//идти вниз
                dy = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 2) CurrentFrame -= 2;
                sprite.setTextureRect(IntRect(52 * int(CurrentFrame), 2*52, 52, 52));
                break;
            }
            case stay:{//стоим
                dx = 0;
                dy = 0;
                break;
            }
        }
        x += dx*time; //движение по “X”
        checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
        y += dy*time; //движение по “Y”
        checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
        speed = 0; //обнуляем скорость, чтобы персонаж остановился.
        sprite.setPosition(x, y); //спрайт в позиции (x, y).
        if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
        state = stay;
        dx = 0;
        dy = 0;
    }
}
