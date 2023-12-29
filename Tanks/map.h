#ifndef MAP_H
#define MAP_H
#include <SFML/Graphics.hpp>
#include <list>

class Map
{
private:
    int heightMap; //размер карты высота
    int widthMap;  //размер карты ширина
    int maxStars;      //максимальное количество звезд на поле
    int maxRepairs;    //максимальное количество ремкомплектов на поле
    int maxAmmunition; //максимальное количество боекомплектов на поле
    std::string tileMap;
    sf::Texture texture;
    sf::Sprite sprite;
    int level;

public:
    Map(sf::Image image);
    void createLevel();
    int getLevel();
    void draw(sf::RenderWindow& window);
    std::string getTileMap();
    void setTileMap(std::string TileMap);
    int getWidthMap();
    int getHeightMap();
    sf::Sprite getSprite();
    void randomMapGenerate();
};

#endif // MAP_H
