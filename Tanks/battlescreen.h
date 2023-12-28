#ifndef BATTLESCREEN_H
#define BATTLESCREEN_H
#include "iostream"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "entity.h"
#include <list>
#include <SFML/Graphics.hpp>

class BattleScreen
{
public:
    void play();
    void readEvent(sf::RenderWindow& window, Player& player, std::list<Entity*>& Bullets, Image& BulletImage);
    int createEnemy(std::list<Entity*>& Enemies, Image& EnemyImage, Map& map);
    void createImpact(std::list<Entity*>& Impacts, Image& ImpactImage, Map& map);
    void updateEnemy(float& time, Player& player, std::list<Entity*>& Enemies,
                     int& enemiesCount, std::list<Entity*>& Bullets, Image& BulletImage,
                     std::list<Entity*>& Impacts, Image& ImpactImage, Map& map);
    void updateBullet(float& time, Player& player, std::list<Entity*>& Bullets, std::list<Entity*>& Impacts, Image& ImpactImage, Map& map);
    void updateImpact(std::list<Entity*>& Impacts, int& impactsCount);
    void checkColision(Player& player,std::list<Entity*>& Enemies, std::list<Entity*> Bullets, std::list<Entity*>& Impacts, Image& ImpactImage);
    void draw(sf::RenderWindow& window, Map& map, Player& player, std::list<Entity*>& Enemies, std::list<Entity*>& Bullets, std::list<Entity*>& Impacts);
    void message(std::string Topic, sf::RenderWindow& window, Map map, Player player);
};

#endif // BATTLESCREEN_H
