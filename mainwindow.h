#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
# include <QPaintEvent>
#include <QTime>
#include<QTimer>
#include <QGraphicsScene>

#include "game.h"
#include "unit.h"
#include"menu.h"

//======================================================
typedef struct _timedata_
    {
        unsigned int sec;
        unsigned int min;
        unsigned int hour;
    } TimeData;

//======================================================
class GameTimeCounter
{
private:
    TimeData GameTime;


public:
    GameTimeCounter();
    ~GameTimeCounter();

    TimeData getGameTime();
    bool	CountUp();
    bool	CountDown();
    bool	ResetCount();
};

//=======================================================


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:

    Fild gameFild;
    Game *GameController;
    MoveDirection mvf;
    GameTimeCounter GTC;

    qint32 timerSource;
    qint32 TimeBase;
    qint32 LevelTimeStep;
    qint32 GamePause;
    qint32 delay_cnt;
    qint32 X_max,Y_max;

    GameStatus PST;
    QTimer *timer;
    QGraphicsScene *scene;

//    Fild pole;


    Ui::MainWindow *ui;
    menu *m;

    void    keyPressEvent (QKeyEvent* pe);
    void    paintEvent(QPaintEvent *event);
    bool    Main_Loop();

private slots:
    void _tic();
    void on_Button_Up_clicked();
    void on_Button_Down_clicked();
    void on_Button_Left_clicked();
    void on_Button_Right_clicked();
    void game_menu(int st);

    void on_menu_clicked();
};

//==============================================================

#endif // MAINWINDOW_H








