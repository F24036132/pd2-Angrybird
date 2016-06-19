#ifndef BARRIER_H
#define BARRIER_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define WOOD_DENSITY 5.0f
#define WOOD_FRICTION 0.3f
#define WOOD_RESTITUTION 0.2f

class Barrier : public GameItem
{
public:
    Barrier(float x, float y, float w, float h, float angle, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    b2Vec2 getLinearVelocity();
};

#endif // BARRIER_H
