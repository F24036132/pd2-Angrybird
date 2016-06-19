#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Angry Bird");
    // Enable the event Filter
    qApp->installEventFilter(this);
    // Disable the ScrollBars
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Set background
    ui->graphicsView->setBackgroundBrush(QPixmap(":/image/background_2.png").scaled(960, 540));

    initialValue();
    QSound::play(":/sound/Angry_Birds_Theme_Song.wav");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    createWorld();
    setLittleWidget();

    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    timer.start(100/6);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    QMouseEvent *mouse;
    QWheelEvent *wheel;
    QPoint birdPos;
    static QPoint start,end;
    //distance between bird & cursor
    static int b_c_x,b_c_y;

    if(event->type()==QEvent::MouseButtonPress || event->type()==QEvent::MouseMove || event->type()==QEvent::MouseButtonRelease)
        mouse = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonPress)
    {
        //check select_bird button x
        buttonX = mouse->windowPos().x();

        //if there's a bird
        if(birdborn){
            birdPos = birdie->getPosition();
            if((birdPos.x()<=mouse->x()+addScale)&&(mouse->x()<=(birdPos.x()+addScale+60))&&(birdPos.y()<=mouse->y()+addScale)&&(mouse->y()<=(birdPos.y()+addScale+60)))
            {
                birdie->setAwake(false);
                press = true;
                b_c_x = mouse->x()-birdPos.x();
                b_c_y = mouse->y()-birdPos.y();
                start = mouse->pos();
            }
        }
        if(birdskill){
            QSound::play(":/sound/Activate_Sound_Effect.wav");
            switch (birdType) {
            case 0:
                break;
            case 1:
                birdie->setLinearVelocity(b2Vec2(20,-20));
                break;
            case 2:
                copybird1 = new Bird(birdie->getVPosition().x,birdie->getVPosition().y+2,0.2f,&timer,QPixmap(":/image/angry-bird-yellow-icon.png").scaled(height()/12.0,height()/12.0),world,scene);
                itemList.push_back(copybird1);
                copybird1->setLinearVelocity(b2Vec2(birdie->getLinearVelocity().x,birdie->getLinearVelocity().y+2));
                copybird1->countTime = true;
                copybird2 = new Bird(birdie->getVPosition().x,birdie->getVPosition().y-2,0.2f,&timer,QPixmap(":/image/angry-bird-yellow-icon.png").scaled(height()/12.0,height()/12.0),world,scene);
                itemList.push_back(copybird2);
                copybird2->setLinearVelocity(b2Vec2(birdie->getLinearVelocity().x,birdie->getLinearVelocity().y-2));
                copybird2->countTime = true;
                break;
            case 3:
                if(!dead[0] && birdie->getVPosition().x == pig[0]->getVPosition().x){
                    pig[0]->setLinearVelocity(b2Vec2(50,0));
                }
                else if(!dead[1] && birdie->getVPosition().x == pig[1]->getVPosition().x){
                    pig[1]->setLinearVelocity(b2Vec2(50,0));
                }
                else if(!dead[2] && birdie->getVPosition().x == pig[2]->getVPosition().x){
                    pig[2]->setLinearVelocity(b2Vec2(50,0));
                }
                break;
            default:
                break;
            }
            birdskill = false;
        }
    }
    if(event->type() == QEvent::MouseMove)
    {
        if(press && birdborn)
            birdie->setPosition(mouse->x(),mouse->y(),b_c_x,b_c_y,addScale);
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(birdborn)
        {
            QSound::play(":/sound/Trampoline_Sound_Effect.wav");
            birdie->countTime = true;
            press = false;
            birdie->setAwake(true);
            end = mouse->pos();
            birdie->setLinearVelocity(b2Vec2((start.x()-end.x())/6,(end.y()-start.y())/3));
            birdborn = false;
            birdskill = true;
        }
    }
    if(object == ui->graphicsView->viewport() && event->type() == QEvent::Wheel)
    {
        wheel = static_cast<QWheelEvent*>(event);
        if(wheel->delta()>0)
            viewSizechange(true);
        else
            viewSizechange(false);
        return true;
    }
    return false;
}

void MainWindow::viewSizechange(bool zoom)
{
    if(zoom && scaleFactor>=100 && scaleFactor <150)
    {
        scaleFactor += 5;
        addScale += 70.0/11.0;
        QMatrix old_matrix;
        old_matrix = ui->graphicsView->matrix();
        ui->graphicsView->resetMatrix();
        ui->graphicsView->translate(old_matrix.dx(),old_matrix.dy());
        ui->graphicsView->centerOn(0,540);
        ui->graphicsView->scale(scaleFactor/100.0,scaleFactor/100.0);
    }
    else if(!zoom && scaleFactor>100)
    {
        scaleFactor -= 5;
        addScale -= 70.0/11.0;
        QMatrix old_matrix;
        old_matrix = ui->graphicsView->matrix();
        ui->graphicsView->resetMatrix();
        ui->graphicsView->translate(old_matrix.dx(),old_matrix.dy());
        ui->graphicsView->centerOn(0,540);
        ui->graphicsView->scale(scaleFactor/100.0,scaleFactor/100.0);
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::tick()
{
    world->Step(1.0/60.0,6,2);
    scene->update();
}

void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
}


void MainWindow::createWorld()
{
    // Setting the QGraphicsScene
    scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    // Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));
    // Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());

    // Create ground (You can edit here)
    itemList.push_back(new Land(16,1.0,32,2,QPixmap(""),world,scene));

    // Create sling
    itemList.push_back(new Sling(4,2,1,6,QPixmap(":/image/sling.png").scaled(60,90),world,scene));

    // Create rock
    itemList.push_back(new Rock(18,2,2,5.8,QPixmap(":/image/rock.png").scaled(180,160),world,scene));

    // Create barrier
    barrier[0] = new Barrier(26.7f,5.0f,0.5f,1.2f,2,&timer,QPixmap(":/image/wood_2.png").scaled(30,78),world,scene);
    barrier[1] = new Barrier(29.3f,5.0f,0.5f,1.2f,2,&timer,QPixmap(":/image/wood_2.png").scaled(30,78),world,scene);
    barrier[2] = new Barrier(28.0f,7.0f,0.5f,1.9f,0.5,&timer,QPixmap(":/image/wood_2.png").scaled(30,100),world,scene);
    barrier[3] = new Barrier(26.7f,9.0f,0.5f,1.2f,2,&timer,QPixmap(":/image/wood_2.png").scaled(30,78),world,scene);
    barrier[4] = new Barrier(29.3f,9.0f,0.5f,1.2f,2,&timer,QPixmap(":/image/wood_2.png").scaled(30,78),world,scene);
    barrier[5] = new Barrier(28.0f,11.0f,0.5f,1.9f,0.5,&timer,QPixmap(":/image/wood_2.png").scaled(30,100),world,scene);
    barrier[6] = new Barrier(26.7f,13.0f,0.5f,1.2f,2,&timer,QPixmap(":/image/wood_2.png").scaled(30,78),world,scene);
    barrier[7] = new Barrier(29.3f,13.0f,0.5f,1.2f,2,&timer,QPixmap(":/image/wood_2.png").scaled(30,78),world,scene);
    barrier[8] = new Barrier(28.0f,15.0f,0.5f,1.9f,0.5,&timer,QPixmap(":/image/wood_2.png").scaled(30,100),world,scene);
    for(int i=0;i<9;++i) {itemList.push_back(barrier[i]);}

    // Creat pig
    pig[0] = new Pig(28.0f,4.0f,0.2f,&timer,QPixmap(":/image/pig.png").scaled(height()/12.0,height()/12.0),world,scene);
    pig[1] = new Pig(28.0f,7.0f,0.2f,&timer,QPixmap(":/image/pig.png").scaled(height()/12.0,height()/12.0),world,scene);
    pig[2] = new Pig(28.0f,11.0f,0.2f,&timer,QPixmap(":/image/pig.png").scaled(height()/12.0,height()/12.0),world,scene);
    for(int i=0;i<3;++i) {itemList.push_back(pig[i]);}
}

void MainWindow::setLittleWidget()
{
    //score table
    scoretable = new QLabel(this);
    scoretable->setGeometry(500,60,400,40);
    scoretable->setFont(QFont("Hobo Std",24));
    scoretable->setStyleSheet("color:black");
    scoretable->setAlignment(Qt::AlignRight);
    scoretable->setText("Score: " + QString::number(score));
    scoretable->show();

    //select bird button
    redbutton = new QPushButton(this);
    redbutton->setGeometry(50,20,50,50);
    redbutton->setStyleSheet("border-image:url(:/image/angry-bird-icon.png)");
    connect(redbutton,SIGNAL(clicked(bool)),this,SLOT(createBird()));
    scene->addWidget(redbutton);
    redbutton->show();

    bluebutton = new QPushButton(this);
    bluebutton->setGeometry(105,20,50,50);
    bluebutton->setStyleSheet("border-image:url(:/image/angry-bird-blue-icon.png)");
    connect(bluebutton,SIGNAL(clicked(bool)),this,SLOT(createBird()));
    scene->addWidget(bluebutton);
    bluebutton->show();

    yellowbutton = new QPushButton(this);
    yellowbutton->setGeometry(160,20,50,50);
    yellowbutton->setStyleSheet("border-image:url(:/image/angry-bird-yellow-icon.png)");
    connect(yellowbutton,SIGNAL(clicked(bool)),this,SLOT(createBird()));
    scene->addWidget(yellowbutton);
    yellowbutton->show();

    whitebutton = new QPushButton(this);
    whitebutton->setGeometry(215,20,50,50);
    whitebutton->setStyleSheet("border-image:url(:/image/angry-bird-white-icon.png)");
    connect(whitebutton,SIGNAL(clicked(bool)),this,SLOT(createBird()));
    scene->addWidget(whitebutton);
    whitebutton->show();

    //restart button
    restart = new QPushButton(this);
    restart->setGeometry(780,15,40,40);
    restart->setStyleSheet("border-image:url(:/image/Restart.ico)");
    connect(restart,SIGNAL(clicked(bool)),this,SLOT(setRestart()));
    scene->addWidget(restart);
    restart->show();

    //exit button
    exit = new QPushButton(this);
    exit->setGeometry(860,15,40,40);
    exit->setStyleSheet("border-image:url(:/image/emergency-exit.png)");
    connect(exit,SIGNAL(clicked(bool)),this,SLOT(exitGame()));
    scene->addWidget(exit);
    exit->show();

    //help button
    help = new QPushButton(this);
    help->setGeometry(700,20,40,40);
    help->setStyleSheet("border-image:url(:/image/info.png)");
    connect(help,SIGNAL(clicked(bool)),this,SLOT(showHelp()));
    scene->addWidget(help);
    help->show();
}

void MainWindow::createBird()
{
    QSound::play(":/sound/Bird_Select_Sound_Effect.wav");
    int x = buttonX;
    if(x>=50 && x<=100)
    {
        birdie = new Bird(4.0f,9.0f,0.2f,&timer,QPixmap(":/image/angry-bird-icon.png").scaled(height()/12.0,height()/12.0),world,scene);
        itemList.push_back(birdie);
        birdType = 0;
        birdborn = true;
        redbutton->hide();
    }
    else if(x>=105 && x<=155)
    {
        birdie = new Bird(4.0f,9.0f,0.2f,&timer,QPixmap(":/image/angry-bird-blue-icon.png").scaled(height()/12.0,height()/12.0),world,scene);
        itemList.push_back(birdie);
        birdType = 1;
        birdborn = true;
        bluebutton->hide();
    }
    else if(x>=160 && x<=210)
    {
        birdie = new Bird(4.0f,9.0f,0.2f,&timer,QPixmap(":/image/angry-bird-yellow-icon.png").scaled(height()/12.0,height()/12.0),world,scene);
        itemList.push_back(birdie);
        birdType = 2;
        birdborn = true;
        yellowbutton->hide();
    }
    else if(x>=215 && x<=265)
    {
        birdie = new Bird(4.0f,9.0f,0.2f,&timer,QPixmap(":/image/angry-bird-white-icon.png").scaled(height()/12.0,height()/12.0),world,scene);
        itemList.push_back(birdie);
        birdType = 3;
        birdborn = true;
        whitebutton->hide();
    }

    connect(&timer,SIGNAL(timeout()),this,SLOT(pigDestroy()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(barrierDestroy()));
}

void MainWindow::pigDestroy()
{
    if(!dead[0])
    {
        if(abs(pig[0]->getVPosition().x - 28.0f)>1 || abs(pig[0]->getVPosition().y - 2.0f)>3)
        {
            QSound::play(":/sound/Piglette_Collision_Sound_Effect.wav");
            score +=5000;
            dead[0] = true;
            delete pig[0];
        }
        else if(abs(birdie->getVPosition().x - pig[0]->getVPosition().x)<=0.8 && abs(birdie->getVPosition().y - pig[0]->getVPosition().y)<=0.8)
        {
            QSound::play(":/sound/Piglette_Collision_Sound_Effect.wav");
            score +=5000;
            dead[0] = true;
            delete pig[0];
        }
    }
    if(!dead[1])
    {
        if(abs(pig[1]->getVPosition().x - 28.0f)>1 || abs(pig[1]->getVPosition().y - 6.0f)>3)
        {
            QSound::play(":/sound/Piglette_Collision_Sound_Effect.wav");
            score +=5000;
            dead[1] = true;
            delete pig[1];
        }
        else if(abs(birdie->getVPosition().x-pig[1]->getVPosition().x)<=0.8 && abs(birdie->getVPosition().y-pig[1]->getVPosition().y)<=0.8)
        {
            QSound::play(":/sound/Piglette_Collision_Sound_Effect.wav");
            score +=5000;
            dead[1] = true;
            delete pig[1];
        }
    }
    if(!dead[2])
    {
        if(abs(pig[2]->getVPosition().x - 28.0f)>1 || abs(pig[2]->getVPosition().y - 10.0f)>3)
        {
            QSound::play(":/sound/Piglette_Collision_Sound_Effect.wav");
            score +=5000;
            dead[2] = true;
            delete pig[2];
        }
        else if(abs(birdie->getVPosition().x-pig[2]->getVPosition().x)<=0.8 && abs(birdie->getVPosition().y-pig[2]->getVPosition().y)<=0.8)
        {
            QSound::play(":/sound/Piglette_Collision_Sound_Effect.wav");
            score +=5000;
            dead[2] = true;
            delete pig[2];
        }
    }
    scoretable->setText("Score: " + QString::number(score));
}

void MainWindow::barrierDestroy()
{
    for(int i=0;i<9;++i){
        if(!collapse[i]){
            if(abs(barrier[i]->getLinearVelocity().x) + abs(barrier[i]->getLinearVelocity().y)>6){
                QSound::play(":/sound/Breaks_Sound.wav");
                score +=100;
                collapse[i] = true;
                delete barrier[i];
            }
        }
    }
    scoretable->setText("Score: " + QString::number(score));
}

void MainWindow::setRestart()
{
    for(int i=0;i<3;++i){
        if(!dead[i]) delete pig[i];
    }
    for(int i=0;i<9;++i){
        if(!collapse[i]) delete barrier[i];
    }
    deleter();
    createWorld();
    initialValue();
    redbutton->show();
    bluebutton->show();
    yellowbutton->show();
    whitebutton->show();
}

void MainWindow::initialValue()
{
    score = 0;
    press=false;
    birdborn=false;
    birdskill=false;
    dead[0]=false;
    dead[1]=false;
    dead[2]=false;
    collapse[8]=false;
    collapse[7]=false;
    collapse[6]=false;
    collapse[5]=false;
    collapse[4]=false;
    collapse[3]=false;
    collapse[2]=false;
    collapse[1]=false;
    collapse[0]=false;
}

void MainWindow::deleter(){
    itemList.clear();
}

void MainWindow::exitGame()
{
    emit quitGame();
    close();
}

void MainWindow::showHelp()
{
    explain.show();
}
