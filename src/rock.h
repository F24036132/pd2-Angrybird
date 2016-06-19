#ifndef ROCK_H
#define ROCK_H

#include <gameitem.h>
#include <QGraphicsScene>

class Rock : public GameItem
{
public:
    Rock(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

#endif // ROCK_H
