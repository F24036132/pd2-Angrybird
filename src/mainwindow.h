#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QSound>

#include <gameitem.h>
#include <land.h>
#include <sling.h>
#include <rock.h>
#include <barrier.h>
#include <pig.h>
#include <bird.h>
#include <explain.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *object, QEvent *event);
    void closeEvent(QCloseEvent *);
    //my functions
    void createWorld();
    void setLittleWidget();
    void viewSizechange(bool zoom);
    void initialValue();
    void deleter();

public slots:
    void pigDestroy();
    void barrierDestroy();
    void createBird();
    void setRestart();
    void exitGame();
    void showHelp();

signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    // For debug slot
    void QUITSLOT();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QTimer timer;
//my objects
    Barrier *barrier[9];
    Pig *pig[3];
    QLabel *scoretable;
    int score = 0;
    QPushButton *redbutton, *bluebutton, *yellowbutton, *whitebutton;
    QPushButton *restart, *exit, *help;
    int buttonX;
    bool birdborn;
    bool dead[3];
    bool collapse[9];
    Bird *birdie;
    Bird *copybird1,*copybird2;
    bool press;
    float scaleFactor = 100.0;
    float addScale = 0;
    int birdType;
    bool birdskill;
    Explain explain;
};

#endif // MAINWINDOW_H
