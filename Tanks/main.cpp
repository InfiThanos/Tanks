#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h" //подключили код с картой
#include "player.h"
#include "entity.h"
#include "enemy.h"
#include "bullet.h"
#include "impact.h"
#include <list>

using namespace sf;

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1280, 736, desktop.bitsPerPixel), "Battle Tank");

 //   sf::SoundBuffer buffer;  // звуковое сопровождение
 //   buffer.loadFromFile("SFX/Intro.wav");// тут загружаем в буфер звуковой файл
 //   sf::Sound sound;
 //   sound.setBuffer(buffer);
 //   sound.play();

    Font font; //шрифт
    font.loadFromFile("pixelfont.ttf"); //передаем нашему шрифту файл шрифта
    Text text("", font, 20); //создаем объект текст
    text.setColor(Color::White); //покрасили текст
    text.setStyle(Text::Bold); //жирный текст
    Image map_image; //объект изображения для карты
    map_image.loadFromFile("images/map_test32.png"); //загружаем файл для карты
    Texture map;//текстура карты
    map.loadFromImage(map_image); //заряжаем текстуру картинкой
    Sprite s_map; //создаём спрайт для карты
    s_map.setTexture(map); //заливаем текстуру спрайтом
    Clock clock;  // время для синхронизации движения объектов
    Clock gameTimeClock; //переменная игрового времени, будем здесь хранить время игры
    Clock generateClock; //время для задержек генерации вещей и бонусов
    int gameTime = 0; //объявили игровое время, инициализировали.
    Image heroImage;
    heroImage.loadFromFile("images/player.png"); //загружаем изображения игрока
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); //загружаем изображения врага
    Image BulletImage;//изображение для пули
    BulletImage.loadFromFile("images/bullet.png"); //загрузили изображения пули
    Image ImpactImage;//изображение для взрыва
    ImpactImage.loadFromFile("images/impact.png"); //загрузили изображения взрывов
    Player p(heroImage, 100, 100, 52, 52, "Player1"); //создаем объект класса игрока
    std::list<Entity*> enemies; //список врагов
    std::list<Entity*> Bullets; //список пуль игрока
    std::list<Entity*> Impacts; //спысок взрывов на поле
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
    std::list<Entity*>::iterator it1; //итератор чтобы проходить по элементам списка
    int impactsCount;

    int enemiesCount = 0; //текущее количество врагов в игре

    //Заполняем список объектами врагами из карты
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++){
            if (TileMap[i][j] == 'T'){
                enemies.push_back(new Enemy(easyEnemyImage, j * 32, i * 32, 52, 52, "EasyEnemy"));
                enemiesCount += 1; //увеличили счётчик врагов
                TileMap[i][j] = ' '; //на карте, где был враг ставим пустое место
                it = enemies.end();
                it--;
                //указываем врагу случайное направление движения
                (*it)->direction = rand() % (4);
            }
        }

    // Начинаем игру
    while (window.isOpen() && p.life && (enemiesCount > 0 || impactsCount > 0))
    {
        float time = clock.getElapsedTime().asMicroseconds();
        if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); //игровое время в
        //секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
        //оно не обновляет логику игры
        clock.restart();
        time = time / 800;
        int generateTimer = generateClock.getElapsedTime().asMilliseconds();
        if (generateTimer > 3000) {  // время в миллисекундах
            randomMapGenerate(); //генерация разных вещей на карте
            generateClock.restart();
        }

        // Читаем события
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) //стреляем по нажатию клавиши "Пробел"
                if (event.key.code == sf::Keyboard::Space && p.rechargeGun) {
                    // имя пули игрока будет отличаться от вражеских
                    Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "HeroBullet", p.gunDirect));
                    p.ammo--;
                    p.rechargeGun = false;
                }
            if (event.type == sf::Event::KeyReleased)
                if (event.key.code == sf::Keyboard::Space && p.ammo > 0) p.rechargeGun = true;
        }
        p.update(time); //оживляем объект “p” класса “Player”
        //оживляем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
             // враги стреляют по времени
            if ((*it)->shootReady) { //если враг готов стрелять, выпускаем из него пулю
                (*it)->shootReady = false;
                Bullets.push_back(new Bullet(BulletImage, (*it)->x, (*it)->y, 12, 12, "Bullet", (*it)->direction));
            }
        }
        //оживляем пули и смотрим пересечение пуль врагов с игроком
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
            if (p.getRect().intersects((*it)->getRect()) && (*it)->name == "Bullet") { //попадание в игрока
                p.health -= 20; //живучесть игрока уменьшается
                (*it)->life = false;  //пуля исчезает
                Impacts.push_back(new Impact(ImpactImage, p.x, p.y, 52, 52, "EasyImpact")); //новый взрыв на месте игрока
            }
        }
        // оживляем взрывы
        for (it = Impacts.begin(); it != Impacts.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
        }
        //Проверяем список на наличие "мертвых" пуль и удаляем их
        for (it = Bullets.begin(); it != Bullets.end();)//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)->life == false) { it = Bullets.erase(it); }
            else it++;//и идем курсором (итератором) к след объекту.
        }
        //Проверяем список на наличие мертвых врагов и удаляем их
        for (it = enemies.begin(); it != enemies.end(); )//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)->life == false) {
                it = enemies.erase(it); enemiesCount--;
            }
            else it++;//и идем курсором (итератором) к след объекту.
        }
        //Проверяем список на наличие прошедших взрывов и удаляем их
        impactsCount = 0;
        for (it = Impacts.begin(); it != Impacts.end(); )//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)->life == false) {
                it = Impacts.erase(it);
            }
            else it++;//и идем курсором (итератором) к след объекту.
            impactsCount++;
        }
        //Смотрим пересечение игрока с врагами
        for (it = enemies.begin(); it != enemies.end(); it++) //бежим по списку врагов
        {
            if (p.getRect().intersects((*it)->getRect()) && (*it)->name == "EasyEnemy") {
                p.life = false;
                p.health = 0;
                break;
            }
            //Смотрим пересечение врагов между собой и при столкновении друг с другом
            //меняем направление движения на противоположное
            for (it1 = enemies.begin(); it1 != enemies.end(); it1++)
                if (it != it1)
                    if ((*it1)->getRect().intersects((*it)->getRect()))
                        switch ((*it1)->direction)
                        {
                            case 0:{ (*it1)->direction = 1; break; }
                            case 1:{ (*it1)->direction = 0; break; }
                            case 2:{ (*it1)->direction = 3; break; }
                            case 3:{ (*it1)->direction = 2; break; }
                        }
            //Смотрим пересечение врагов с пулями игрока
            for (it1 = Bullets.begin(); it1 != Bullets.end(); it1++)
            {
                if ((*it1)->getRect().intersects((*it)->getRect()) && (*it1)->name == "HeroBullet") {
                    (*it)->health -= 35;  // Уменьшаем здоровье врага
                    Impacts.push_back(new Impact(ImpactImage, (*it)->x, (*it)->y, 52, 52, "EasyImpact"));
                    (*it1)->life = false;
                    break;  // Одна пуля - одно попадание, выходим из цикла
                }
            }
        }  //for (it = enemies.begin()
        window.clear();  //очищаем игровое окно

/////////////////////////////Рисуем карту/////////////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(128, 0, 32, 32));
                if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(32, 0, 32, 32));  //стена
                if (TileMap[i][j] == 'x') s_map.setTextureRect(IntRect(0, 0, 32, 32));  //стена кирпич
                if (TileMap[i][j] == 'X') {
                    TileMap[i][j] = ' ';
                    //взрыв кирпичной стены
                    Impacts.push_back(new Impact(ImpactImage, j * 32 - 10, i * 32 - 10, 52, 52, "EasyImpact"));
                }
                if (TileMap[i][j] == 'a') s_map.setTextureRect(IntRect(256, 0, 32, 32));  //оружие
                if (TileMap[i][j] == 'r') s_map.setTextureRect(IntRect(224, 0, 32, 32));  //ремонт
                if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(288, 0, 32, 32));  //звезда - бонус
                s_map.setPosition(j * 32, i * 32);
                window.draw(s_map);
            }
        window.draw(p.sprite); //рисуем спрайт объекта “p” класса “Player”
        //рисуем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            if ((*it)->life) //если враги живы
                window.draw((*it)->sprite); //рисуем врагов
        }
        //рисуем пули
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            if ((*it)->life) //если пули живы
                window.draw((*it)->sprite); //рисуем пули
        }
        //рисуем взрывы
        for (it = Impacts.begin(); it != Impacts.end(); it++)
        {
            if ((*it)->life) //если взрывы живы
                window.draw((*it)->sprite); //рисуем объекты
        }
        //объявили переменную здоровья, вооружения и очков
        std::ostringstream playerHealthString, gameScoreString, playerAmmoString;
        playerHealthString << p.health;  //формируем строку
        gameScoreString << p.playerScore; //формируем строку
        playerAmmoString << p.ammo; //формируем строку
        //задаем строку тексту
        text.setString("Health: " + playerHealthString.str() + "    Ammo: " + playerAmmoString.str()
                       + "\nScore: "
                       + gameScoreString.str());
        text.setPosition(40, 40); //задаем позицию текста
        window.draw(text); //рисуем этот текст
        window.display();  // перерисовываем игровое окно
    } // while игры

    Text text1("", font, 40);
    text1.setColor(Color::Yellow);
    text1.setStyle(Text::Bold);
    if (p.life) {
        text1.setString("You are win!");
        text1.setPosition(500, 300);
        window.draw(text1);
        window.display();
    }
    else {
        text1.setString("You are lose!");
        text1.setPosition(500, 300);
        window.draw(text1);
        window.display();
    }
    // Ожидаем закрытия окна или нажатия клавиши
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Space)
                    window.close();
        }
    }
    return 0;
}
