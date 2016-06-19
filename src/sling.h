#ifndef SLING_H
#define SLING_H

#include <gameitem.h>
#include <QGraphicsScene>

class Sling : public GameItem
{
public:
    Sling(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

#endif // SLING_H
