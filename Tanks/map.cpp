#include "map.h"
#include <SFML/Graphics.hpp>
#include "string.h"
#include <iostream>

using namespace sf;

Map::Map(Image image) : heightMap (23), widthMap (40), maxStars (5),
    maxRepairs(2), maxAmmunition (2), level(1)
{
    tileMap = { "0000000000000000000000000000000000000000"
                "0______________________________________0"
                "0_______________________________s______0"
                "0______________xx_______xx_____________0"
                "0_____xx_______xxxxxxxxxxx______xx_____0"
                "0_____x0_______x000000000x______0x_____0"
                "0_____x0_______x0_______0x______0x_____0"
                "0_____x0________________________0x_____0"
                "0__________s___________________________0"
                "0______________________________________0"
                "0xx___xxxxx__________________xxxxx___xx0"
                "00x___x0000__________________x000x___x00"
                "0______________________________________0"
                "0________________________________T_____0"
                "0_____T________xx_______xx_____________0"
                "0______________x0xxxxxxx0x_____________0"
                "0___xx___xx____x000000000x____xxxxx____0"
                "0___x0___x0____x0_______0x____x0000____0"
                "0___x0___x0____x0___s___0x_____________0"
                "0______________________________________0"
                "0_______________________________M______0"
                "0______________________________________0"
                "0000000000000000000000000000000000000000"
              };
    texture.loadFromImage(image); //заносим наше изображение в текстуру
    sprite.setTexture(texture);   //заливаем спрайт текстурой
}

int Map::getLevel()
{
    return level;
}

void Map::createLevel()
{
    level++;
    if (level == 2)
    {
        tileMap = { "0000000000000000000000000000000000000000"
                    "0_______________________00_____________0"
                    "0______________________xx0_________M___0"
                    "0______________________xx0_____________0"
                    "0_____xx____________xxxxx0______xx_____0"
                    "0_____x0_________0xxxxxxx0______x0_____0"
                    "0_____x0_________000000000______x0_____0"
                    "0_____x000______________________x0_____0"
                    "0______________________________________0"
                    "0______________________________________0"
                    "0xx___xxxxx__________H_______xxxxx___xx0"
                    "000___x0000__________________x0000___000"
                    "0______________________________________0"
                    "0______________________________________0"
                    "0______________xxx_____xxx_____________0"
                    "0______________xx_______xx_____________0"
                    "0______________xxxxxxxxxxx_____________0"
                    "0____________________________xxxxx_____0"
                    "0___x0___x0__________________x0000_____0"
                    "0___________________M__________________0"
                    "0______________________________________0"
                    "0______________________________________0"
                    "0000000000000000000000000000000000000000"
                  };
    }
    else
    {
        tileMap = { "0000000000000000000000000000000000000000"
                    "0______________________________________0"
                    "0___________________________H__________0"
                    "0______________________________________0"
                    "0________x0___x0___x0___x0___x0___x0___0"
                    "0________x0____________________________0"
                    "0________x0____________________________0"
                    "0________x0____________________________0"
                    "0________x0____________________________0"
                    "0________x0____00000000000___xxxxx___xx0"
                    "0xxxxxxxxx0____00_______00_____________0"
                    "00000000000____xx_______xx_____________0"
                    "0______________xx___H___xx_____________0"
                    "0______________xx_______xx_____________0"
                    "0______________xx___r___xx_______H_____0"
                    "0______________00_______00_____________0"
                    "0______________00xxxxxxx00_____________0"
                    "0______________00xaxxxrx00____x________0"
                    "0__x0__x0___x0_xxxxxxxxxxx____x________0"
                    "0_____________________________x________0"
                    "0_____________________________x________0"
                    "0_____________________________x________0"
                    "0000000000000000000000000000000000000000"
                  };
    }
}

void Map::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < heightMap; i++)
        for (int j = 0; j < widthMap; j++)
        {
            if (tileMap[i*widthMap+j] == '_') sprite.setTextureRect(IntRect(128, 0, 32, 32));
            if (tileMap[i*widthMap+j] == '0') sprite.setTextureRect(IntRect(32, 0, 32, 32));  //стена
            if (tileMap[i*widthMap+j] == 'x') sprite.setTextureRect(IntRect(0, 0, 32, 32));  //стена кирпич

            if (tileMap[i*widthMap+j] == 'a') sprite.setTextureRect(IntRect(256, 0, 32, 32));  //оружие
            if (tileMap[i*widthMap+j] == 'r') sprite.setTextureRect(IntRect(224, 0, 32, 32));  //ремонт
            if (tileMap[i*widthMap+j] == 's') sprite.setTextureRect(IntRect(288, 0, 32, 32));  //звезда - бонус
            sprite.setPosition(j * 32, i * 32);
            window.draw(sprite);
        }

}

std::string Map::getTileMap()
{
    return tileMap;
}

void Map::setTileMap(std::string TileMap)
{
    tileMap = TileMap;
}

int Map::getWidthMap()
{
    return widthMap;
}

int Map::getHeightMap()
{
    return heightMap;
}

sf::Sprite Map::getSprite()
{
    return sprite;
}

void Map::randomMapGenerate()
{
    int randomElementX = 0; //переменная для хранения случайного элемента по горизонтали
    int randomElementY = 0; //переменная для хранения случайного элемента по вертикали
    int countStar = 0;      //текущее количество звезд-бонусов на поле
    int countRepairs = 0;   //текущее количество ремкомплектов на поле
    int countAmmo = 0;      //текущее количество боекомплектов на поле

    for (int i = 0; i < heightMap; i++)
        for (int j = 0; j < widthMap; j++)
        {
            if (tileMap[i*widthMap+j] == 's') countStar++;
            if (tileMap[i*widthMap+j] == 'a') countAmmo++;
            if (tileMap[i*widthMap+j] == 'r') countRepairs++;
        }

    randomElementX = 1 + rand() % (widthMap - 1); //псевдослучайное значение по “x” от 1 до
    //ширина карты-1. Ограничение введено чтобы не получать числа бордюра карты
    randomElementY = 1 + rand() % (heightMap - 1);//по “y”
    int o = rand() % 5;  //для случайного определения, какой предмет будем генерировать
    if (o > 1) {
        //если встретили пустое место на карте, и звезд на карте меньше положенного,
        if (tileMap[randomElementY*widthMap+randomElementX] == '_' && countStar < maxStars) {
            tileMap[randomElementY*widthMap+randomElementX] = 's';    //то ставим туда звезду.
        }
    }
    if (o == 0) {
        if (tileMap[randomElementY*widthMap+randomElementX] == '_' && countAmmo < maxAmmunition) {
            tileMap[randomElementY*widthMap+randomElementX] = 'a';    //ставим запасной боекомплект
        }
    }
    if (o == 1) {
        if (tileMap[randomElementY*widthMap+randomElementX] == '_' && countRepairs < maxRepairs) {
            tileMap[randomElementY*widthMap+randomElementX] = 'r';    //ставим запасной ремкомплект
        }
    }
}

