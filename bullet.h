#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include <SDL.h>

class Bullet : public Object
{
public:
    Bullet();
    Bullet(double x, double y);

    void update(Uint32 dt);

    void destroy();

    double speed;

    bool collide;

    bool increased_damage;

    Direction direction;
};

#endif // BULLET_H
