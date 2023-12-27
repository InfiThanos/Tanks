#ifndef IMPACT
#define IMPACT
#include "entity.h"

using namespace sf;

class Impact : public Entity
{
public:
    float impactLimitTime; //время отображения взрыва
    float impactTimer; //счетчик времени взрыва

    Impact(Image &image, float X, float Y, int W, int H, std::string Name);

    void update(float& time, Map& map);
};

#endif // IMPACT

