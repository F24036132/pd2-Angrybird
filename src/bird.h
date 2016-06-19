#ifndef BIRD_H
#define BIRD_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BIRD_DENSITY 20.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.3f

class Bird : public GameItem
{
public:
    Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    void setLinearVelocity(b2Vec2 velocity);
    b2Vec2 getLinearVelocity();
    void setPosition(int x, int y, int a, int b, float scale);
    QPoint getPosition();
    b2Vec2 getVPosition();

    bool countTime = false;

public slots:
    void birdEliminate();

private:
    int time;
};

#endif // BIRD_H
