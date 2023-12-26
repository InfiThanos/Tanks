#ifndef MAP_H
#define MAP_H
#include <SFML\Graphics.hpp>

const int HEIGHT_MAP = 23;   //размер карты высота
const int WIDTH_MAP = 40;    //размер карты ширина
const int maxStars = 5;      //максимальное количество звезд на поле
const int maxRepairs = 2;    //максимальное количество ремкомплектов на поле
const int maxAmmunition = 2; //максимальное количество боекомплектов на поле

extern std::string TileMap[HEIGHT_MAP * WIDTH_MAP];

void randomMapGenerate(); //рандомно расставляем разные вещи на карте

#endif // MAP_H
