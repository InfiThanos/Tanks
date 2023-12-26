#include "map.h"

std::string TileMap[HEIGHT_MAP * WIDTH_MAP]  = {
    "0000000000000000000000000000000000000000",
    "0                                      0",
    "0                               s      0",
    "0              xx       xx             0",
    "0     xx       x0xxxxxxxx0      xx     0",
    "0     x0       x00000000x0      x0     0",
    "0     x0       x0       x0      x0     0",
    "0     x0                        x0     0",
    "0          s                           0",
    "0                                      0",
    "0xx   xxxxx                  xxxxx   xx0",
    "000   x0000                  x0000   x00",
    "0                                      0",
    "0                                T     0",
    "0     T        xx       xx             0",
    "0              x0xxxxxxxx0             0",
    "0   xx   xx    x0000000000    xxxxx    0",
    "0   x0   x0    x0       x0    x0000    0",
    "0   x0   x0    x0   s   x0             0",
    "0                                      0",
    "0                               T      0",
    "0                                      0",
    "0000000000000000000000000000000000000000",
};

void randomMapGenerate() { //рандомно расставляем камни

    int randomElementX = 0; //переменная для хранения случайного элемента по горизонтали
    int randomElementY = 0; //переменная для хранения случайного элемента по вертикали
    int countStar = 0;      //текущее количество звезд-бонусов на поле
    int countRepairs = 0;   //текущее количество ремкомплектов на поле
    int countAmmo = 0;      //текущее количество боекомплектов на поле

    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap[i][j] == 's') countStar++;
            if (TileMap[i][j] == 'a') countAmmo++;
            if (TileMap[i][j] == 'r') countRepairs++;
        }
    randomElementX = 1 + rand() % (WIDTH_MAP - 1); //псевдослучайное значение по “x” от 1 до
    //ширина карты-1. Ограничение введено чтобы не получать числа бордюра карты
    randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по “y”
    int o = rand() % 5;  //для случайного определения, какой предмет будем генерировать
    if (o > 1) {
        //если встретили пустое место на карте, и звезд на карте меньше положенного,
        if (TileMap[randomElementY][randomElementX] == ' ' && countStar < maxStars) {
            TileMap[randomElementY][randomElementX] = 's';    //то ставим туда звезду.
        }
    }
    if (o == 0) {
        if (TileMap[randomElementY][randomElementX] == ' ' && countAmmo < maxAmmunition) {
            TileMap[randomElementY][randomElementX] = 'a';    //ставим запасной боекомплект
        }
    }
    if (o == 1) {
        if (TileMap[randomElementY][randomElementX] == ' ' && countRepairs < maxRepairs) {
            TileMap[randomElementY][randomElementX] = 'r';    //ставим запасной ремкомплект
        }
    }
}
