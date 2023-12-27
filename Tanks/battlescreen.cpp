#include "battlescreen.h"
#include "map.h"
#include "player.h"
#include "bullet.h"
#include "entity.h"
#include "enemy.h"
#include "impact.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <windows.h>
#include <sstream>

using namespace sf;

void BattleScreen::play()
{
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(VideoMode(1280, 736, desktop.bitsPerPixel), "Battle Tank");

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

    Map map(map_image);

    Image heroImage;
    heroImage.loadFromFile("images/player.png"); //загружаем изображения игрока

    Player p(heroImage, 100, 100, 52, 52, "Player"); //создаем объект класса игрока

    Clock clock;  // время для синхронизации движения объектов
    Clock gameTimeClock; //переменная игрового времени, будем здесь хранить время игры
    Clock generateClock; //время для задержек генерации вещей и бонусов
    int gameTime = 0; //объявили игровое время, инициализировали.

    Image EasyEnemyImage;
    EasyEnemyImage.loadFromFile("images/EasyEnemy.png"); //загружаем изображения врага
    Image MediumEnemyImage;
    MediumEnemyImage.loadFromFile("images/MediumEnemy.png"); //загружаем изображения врага
    Image HardEnemyImage;
    HardEnemyImage.loadFromFile("images/HardEnemy.png"); //загружаем изображения врага
    std::list<Entity*> Enemies; //список врагов

    Image BulletImage;//изображение для пули
    BulletImage.loadFromFile("images/bullet.png"); //загрузили изображения пули

    std::list<Entity*> Bullets; //список пуль игрока

    Image ImpactImage;//изображение для взрыва
    ImpactImage.loadFromFile("images/impact.png"); //загрузили изображения взрывов

    std::list<Entity*> Impacts; //спысок взрывов на поле

    while (window.isOpen() && p.getLife() && map.getLevel() != 4)
    {
        int enemiesCount = createEnemy(Enemies, EasyEnemyImage, map, "EasyEnemy");
        enemiesCount += createEnemy(Enemies, MediumEnemyImage, map, "MediumEnemy");
        enemiesCount += createEnemy(Enemies, HardEnemyImage, map, "HardEnemy"); //текущее количество врагов в игре
        int impactsCount;

        while (window.isOpen() && p.getLife() && enemiesCount)
        {
            float time = clock.getElapsedTime().asMicroseconds();
            if (p.getLife()) gameTime = gameTimeClock.getElapsedTime().asSeconds(); //игровое время в
            //секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
            //оно не обновляет логику игры
            clock.restart();
            time = time / 800;
            int generateTimer = generateClock.getElapsedTime().asMilliseconds();
            if (generateTimer > 3000) {  // время в миллисекундах
                map.randomMapGenerate(); //генерация разных вещей на карте
                generateClock.restart();
            }

            readEvent(window, p, Bullets, BulletImage);

            createImpact(Impacts, ImpactImage, map);
            p.update(time, map); //оживляем объект “p” класса “Player”
            updateEnemy(time, p, Enemies, enemiesCount, Bullets, BulletImage, Impacts, ImpactImage, map);
            updateImpact(Impacts, impactsCount);

            checkColision(p, Enemies, Bullets, Impacts, ImpactImage);

            draw(window, map, p, Enemies, Bullets, Impacts);
        }

        p.setCoordinates(100, 100);
        if ((map.getLevel() < 3) && p.getLife())
        {
            map.createLevel();
            message("Next", window, map, p);
            window.display();
            Sleep(800);
        }
        else break;
    }

    if (p.getLife())
    {
        message("Win", window, map, p);
        window.display();
    }
    else
    {
        message("Lose", window, map, p);
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
}

void BattleScreen::readEvent(sf::RenderWindow& window, Player& player, std::list<Entity*>& Bullets, Image& BulletImage)
{
    // Читаем события
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed) //стреляем по нажатию клавиши "Пробел"
            if (event.key.code == sf::Keyboard::Space && player.getRechargeGun()) {
                // имя пули игрока будет отличаться от вражеских
                float x, y;
                player.getCoordinates(x, y);
                Bullets.push_back(new Bullet(BulletImage, x, y, 16, 16, "HeroBullet", player.getGunDirect()));
                player.setAmmo();
                player.setRechargeGun();
            }
        if (event.type == sf::Event::KeyReleased)
            if (event.key.code == sf::Keyboard::Space && player.getAmmo() > 0) player.setRechargeGun();
    }
}

int BattleScreen::createEnemy(std::list<Entity*>& Enemies, Image& EnemyImage, Map& map, std::string Name)
{
    int EnemiesCount = 0;
    std::string TileMap = map.getTileMap();
    int l_map = map.getWidthMap();
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка

    for (int i = 0; i < map.getHeightMap(); i++)
        for (int j = 0; j < map.getWidthMap(); j++){
            if (TileMap[i*l_map+j] == 'T' && Name == "EasyEnemy")
            {
                Enemies.push_back(new Enemy(EnemyImage, j * 32, i * 32, 52, 52, Name));
                EnemiesCount += 1; //увеличили счётчик врагов
                TileMap[i*l_map+j] = '_'; //на карте, где был враг ставим пустое место
                it = Enemies.end();
                it--;
                //указываем врагу случайное направление движения
                (*it)->setDirection(rand() % (4));
            }
            if (TileMap[i*l_map+j] == 'M' && Name == "MediumEnemy")
            {
                Enemies.push_back(new Enemy(EnemyImage, j * 32, i * 32, 52, 52, Name));
                EnemiesCount += 1; //увеличили счётчик врагов
                TileMap[i*l_map+j] = '_'; //на карте, где был враг ставим пустое место
                it = Enemies.end();
                it--;
                //указываем врагу случайное направление движения
                (*it)->setDirection(rand() % (4));
            }
            if (TileMap[i*l_map+j] == 'H' && Name == "HardEnemy")
            {
                Enemies.push_back(new Enemy(EnemyImage, j * 32, i * 32, 52, 52, Name));
                EnemiesCount += 1; //увеличили счётчик врагов
                TileMap[i*l_map+j] = '_'; //на карте, где был враг ставим пустое место
                it = Enemies.end();
                it--;
                //указываем врагу случайное направление движения
                (*it)->setDirection(rand() % (4));
            }
        }
    map.setTileMap(TileMap);
    return EnemiesCount;
}

void BattleScreen::createImpact(std::list<Entity*>& Impacts, Image& ImpactImage, Map& map)
{
    std::string TileMap = map.getTileMap();
    int l_map = map.getWidthMap();
    int h_map = map.getHeightMap();

    for (int i = 0; i < h_map; i++)
        for (int j = 0; j < l_map; j++)
        {
            if (TileMap[i*l_map+j] == 'X')
            {
                TileMap[i*l_map+j] = '_';
                //взрыв кирпичной стены
                Impacts.push_back(new Impact(ImpactImage, j * 32 - 10, i * 32 - 10, 52, 52, "EasyImpact"));
            }
        }
    map.setTileMap(TileMap);
}

void BattleScreen::updateEnemy(float& time, Player& player, std::list<Entity*>& Enemies,
                               int &enemiesCount, std::list<Entity*>& Bullets, Image& BulletImage,
                               std::list<Entity*>& Impacts, Image& ImpactImage, Map& map)
{
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
    //оживляем врагов
    for (it = Enemies.begin(); it != Enemies.end(); it++)
    {
        (*it)->update(time, map); //запускаем метод update()
        // враги стреляют по времени
        if ((*it)->getShootReady())
        { //если враг готов стрелять, выпускаем из него пулю
            (*it)->setShootReady();
            float x, y;
            (*it)->getCoordinates(x, y);
            Bullets.push_back(new Bullet(BulletImage, x, y, 12, 12, "Bullet", (*it)->getDirection()));
        }
    }

    updateBullet(time, player, Bullets, Impacts, ImpactImage, map);

    //Проверяем список на наличие мертвых врагов и удаляем их
    for (it = Enemies.begin(); it != Enemies.end(); )//говорим что проходимся от начала до конца
    {// если этот объект мертв, то удаляем его
        if ((*it)->getLife() == false)
        {
            it = Enemies.erase(it);
            enemiesCount--;
        }
        else it++;//и идем курсором (итератором) к след объекту.
    }
}

void BattleScreen::updateBullet(float& time, Player& player, std::list<Entity*>& Bullets, std::list<Entity*>& Impacts, Image& ImpactImage, Map& map)
{
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка

    //оживляем пули и смотрим пересечение пуль врагов с игроком
    for (it = Bullets.begin(); it != Bullets.end(); it++)
    {
        (*it)->update(time, map); //запускаем метод update()
        if (player.getRect().intersects((*it)->getRect()) && (*it)->getName() == "Bullet") { //попадание в игрока
            player.setHealth(20); //живучесть игрока уменьшается
            (*it)->setLife();  //пуля исчезает
            float x, y;
            player.getCoordinates(x, y);
            Impacts.push_back(new Impact(ImpactImage, x, y, 52, 52, "EasyImpact")); //новый взрыв на месте игрока
        }
    }

    // оживляем взрывы
    for (it = Impacts.begin(); it != Impacts.end(); it++)
    {
        (*it)->update(time, map); //запускаем метод update()
    }

    //Проверяем список на наличие "мертвых" пуль и удаляем их
    for (it = Bullets.begin(); it != Bullets.end();)//говорим что проходимся от начала до конца
    {// если этот объект мертв, то удаляем его
        if ((*it)->getLife() == false) { it = Bullets.erase(it); }
        else it++;//и идем курсором (итератором) к след объекту.
    }
}

void BattleScreen::updateImpact(std::list<Entity*>& Impacts, int& impactsCount)
{
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
    //Проверяем список на наличие прошедших взрывов и удаляем их
    impactsCount = 0;
    for (it = Impacts.begin(); it != Impacts.end(); )//говорим что проходимся от начала до конца
    {// если этот объект мертв, то удаляем его
        if ((*it)->getLife() == false) {
            it = Impacts.erase(it);
        }
        else it++;//и идем курсором (итератором) к след объекту.
        impactsCount++;
    }
}

void BattleScreen::checkColision(Player& player,std::list<Entity*>& Enemies, std::list<Entity*> Bullets, std::list<Entity*>& Impacts, Image& ImpactImage)
{
    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
    std::list<Entity*>::iterator it1; //итератор чтобы проходить по элементам списка

    //Смотрим пересечение игрока с врагами
    for (it = Enemies.begin(); it != Enemies.end(); it++) //бежим по списку врагов
    {
        if (player.getRect().intersects((*it)->getRect()) && (*it)->getName() == "EasyEnemy") {
            player.setLife();
            player.setHealth(player.getHealth());
            break;
        }
        //Смотрим пересечение врагов между собой и при столкновении друг с другом
        //меняем направление движения на противоположное
        for (it1 = Enemies.begin(); it1 != Enemies.end(); it1++)
            if (it != it1)
                if ((*it1)->getRect().intersects((*it)->getRect()))
                    switch ((*it1)->getDirection())
                    {
                    case 0:{ (*it1)->setDirection(1); break; }
                    case 1:{ (*it1)->setDirection(0); break; }
                    case 2:{ (*it1)->setDirection(3); break; }
                    case 3:{ (*it1)->setDirection(2); break; }
                    }
        //Смотрим пересечение врагов с пулями игрока
        for (it1 = Bullets.begin(); it1 != Bullets.end(); it1++)
        {
            if ((*it1)->getRect().intersects((*it)->getRect()) && (*it1)->getName() == "HeroBullet") {
                (*it)->setHealth(35);  // Уменьшаем здоровье врага
                float x, y;
                (*it)->getCoordinates(x, y);
                Impacts.push_back(new Impact(ImpactImage, x, y, 52, 52, "EasyImpact"));
                (*it1)->setLife();
                break;  // Одна пуля - одно попадание, выходим из цикла
            }
        }
    }
}

void BattleScreen::draw(sf::RenderWindow& window, Map& map, Player& player, std::list<Entity*>& Enemies, std::list<Entity*>& Bullets, std::list<Entity*>& Impacts)
{
    window.clear();

    map.draw(window);

    std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка

    window.draw(player.getSprite()); //рисуем спрайт объекта “p” класса “Player”

    //рисуем врагов
    for (it = Enemies.begin(); it != Enemies.end(); it++)
    {
        if ((*it)->getLife()) //если враги живы
            window.draw((*it)->getSprite()); //рисуем врагов
    }

    //рисуем пули
    for (it = Bullets.begin(); it != Bullets.end(); it++)
    {
        if ((*it)->getLife()) //если пули живы
            window.draw((*it)->getSprite()); //рисуем пули
    }

    //рисуем взрывы
    for (it = Impacts.begin(); it != Impacts.end(); it++)
    {
        if ((*it)->getLife()) //если взрывы живы
            window.draw((*it)->getSprite()); //рисуем объекты
    }

    message("Info", window, map, player);

    window.display();
}

void BattleScreen::message(std::string Topic, sf::RenderWindow& window, Map map, Player player)
{
    Font font;
    font.loadFromFile("pixelfont.ttf"); //передаем нашему шрифту файл шрифта
    if (Topic == "Info")
    {
        Text text = Text("", font, 20); //создаем объект текст
        text.setColor(Color::White); //покрасили текст
        text.setStyle(Text::Bold); //жирный текст
                text.setString("Health: " + std::to_string(player.getHealth()) +
                               "    Ammo: " + std::to_string(player.getAmmo()) +
                               "\nScore: " + std::to_string(player.getPlayerScore()));
                text.setPosition(40, 40); //задаем позицию текста
                window.draw(text); //рисуем этот текст
        return;
    }

    if (Topic == "Win")
    {
        window.clear();
        Text text("", font, 40);
        text.setColor(Color::Yellow);
        text.setStyle(Text::Bold);
        text.setString("You are win!");
        text.setPosition(500, 300);
        window.draw(text);
        text.setString("Your score: " + std::to_string(player.getPlayerScore()));
        text.setPosition(450, 350);
        window.draw(text);
        return;
    }
    else if (Topic == "Lose")
    {
        window.clear();
        Text text("", font, 40);
        text.setColor(Color::Red);
        text.setStyle(Text::Bold);
        text.setString("You are lose!");
        text.setPosition(500, 300);
        window.draw(text);
        text.setString("Your score: " + std::to_string(player.getPlayerScore()));
        text.setPosition(450, 350);
        window.draw(text);
        return;
    }

    if (Topic == "Next")
    {
        window.clear();
        Text text("", font, 40);
        text.setColor(Color::Yellow);
        text.setStyle(Text::Bold);
        text.setString("NEXT LEVEL");
        text.setPosition(500, 300);
        window.draw(text);
        text.setString("lvl " + std::to_string(map.getLevel() - 1)
                               + " >>> " + "lvl " + std::to_string(map.getLevel()));
        text.setPosition(490, 350);
        window.draw(text);
    }
}
