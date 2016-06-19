#include "bird.h"

Bird::Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
    g_pixmap.setPixmap(pixmap);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSizeF(radius*2,radius*2-1.8);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.bullet = true;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    g_body = world->CreateBody(&bodydef);
    b2CircleShape bodyshape;
    bodyshape.m_radius = radius*3;
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;
    g_body->SetAngularDamping(3);
    g_body->CreateFixture(&fixturedef);
    g_body->SetSleepingAllowed(true);

    // Bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));
    connect(timer, SIGNAL(timeout()), this,SLOT(birdEliminate()));
    time = 0;
    scene->addItem(&g_pixmap);
}

void Bird::setLinearVelocity(b2Vec2 velocity)
{
    g_body->SetLinearVelocity(velocity);
}

b2Vec2 Bird::getLinearVelocity()
{
    return g_body->GetLinearVelocity();
}

void Bird::setPosition(int x, int y, int a, int b, float scale)
{
    b2Vec2 pos;
    if(x-scale<40) x=40+scale;
    if(x-scale>220) x=220+scale;
    if(y+scale<350) y=350-scale;
    if(y+scale>450) y=450-scale;
    pos.x = (x-a)*g_worldsize.width()/g_windowsize.width() + g_size.width()/2;
    pos.y = (1.0f - (y-b)/g_windowsize.height())*g_worldsize.height() - g_size.height()/2;
    g_body->SetTransform(pos,g_body->GetAngle());
}

QPoint Bird::getPosition()
{
    b2Vec2 pos;
    QPoint transPos;

    pos = g_body->GetPosition();
    transPos.setX(((pos.x-g_size.width()/2) * g_windowsize.width())/g_worldsize.width());
    transPos.setY((1.0f - (pos.y+g_size.height()/2)/g_worldsize.height()) * g_windowsize.height());
    return transPos;
}

b2Vec2 Bird::getVPosition()
{
    return g_body->GetPosition();
}

void Bird::birdEliminate()
{
    if(countTime)
        ++time;
    if(g_body->GetPosition().x>32 || g_body->GetPosition().x<0)
        delete this;
    else if(g_body->GetPosition().y<0)
        delete this;
    else if(time == 360)
        delete this;
}
