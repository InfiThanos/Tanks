#ifndef MAP_H
#define MAP_H
#include <SFML\Graphics.hpp>

const int HEIGHT_MAP = 20; //размер карты высота
const int WIDTH_MAP = 25;  //размер карты ширина

extern std::string TileMap[HEIGHT_MAP];

void randomMapGenerate(); //рандомно расставляем камни

#endif // MAP_H
