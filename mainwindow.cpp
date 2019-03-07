#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include<QDebug>
#include<QString>

#include "game.h"
#include "unit.h"


//=============================================================
//-------------------------------------------------------------
//-------------------------------------------------------------
//------------- methods of class  GameTimeCounter---------------

GameTimeCounter::GameTimeCounter()
{
    GameTime.sec=0;
    GameTime.min=0;
    GameTime.hour=0;
}

GameTimeCounter::~GameTimeCounter()
{

}

TimeData GameTimeCounter::getGameTime()
{

    return GameTime;
}

bool GameTimeCounter::CountUp()
{
    GameTime.sec++;
    if(GameTime.sec==60){
        GameTime.min++;
        GameTime.sec=0;
    }
    if(GameTime.min==60){
        GameTime.hour++;
        GameTime.min=0;
    }
    if(GameTime.hour==24)
        GameTime.hour=0;
    return true;
}

bool GameTimeCounter::CountDown()
{

    return true;
}

bool GameTimeCounter::ResetCount()
{
    GameTime.sec=0;
    GameTime.min=0;
    GameTime.hour=0;

    return true;
}

//==============================================================



//==============================================================
//-------------------------------------------------------------
//-------------------------------------------------------------
//------------- methods of class  MainWindow ------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    PST=game_stop;

    TimeBase=200;
    LevelTimeStep=20;
    GamePause=5;
    delay_cnt=0;

    gameFild.border_x_min=0;
    gameFild.border_x_max=50;
    gameFild.border_y_min=0;
    gameFild.border_y_max=50;

    GameController=new Game(gameFild,6,0);


//    pole=GameController->getGameFild();


    ui->setupUi(this);

    timer=new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(_tic()));
    timer->start(TimeBase);


}

MainWindow::~MainWindow()
{
    delete GameController;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{

        if (pe->key()==Qt::Key_M){
            PST=game_stop;
        }


        if (PST==game_stop||PST==game_over){
            switch (pe->key())  {
                case Qt::Key_E:
                    PST=game_exit;
                    break;
                case Qt::Key_N:
                    PST=game_new;
                    break;
                case Qt::Key_C:
                    PST=game_on;
                    break;
                default:
                    break;
            }
        }

        if (PST==game_on){
            switch ( pe->key()){
            case Qt::Key_Left:
                mvf=Left;
                qDebug()<<"Pres Key_Left button";
                break;
            case Qt::Key_Right:
                mvf=Right;
                qDebug()<<"Pres Key_Right button";
                break;
            case Qt::Key_Up:
                mvf=Up;
                qDebug()<<"Pres Key_Up button";
                break;
            case Qt::Key_Down:
                mvf=Down;
                qDebug()<<"Pres Key_Down button";
                break;
            default:
                break;
            }
        }

}


void    MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    Point pen;
    int width, height;

    int i;
    int scr_border_x_min,scr_border_x_max,scr_border_y_min,scr_border_y_max;
    qreal hStep,vStep;

    QString str_BUF1,str_BUF2,str_BUF3;

    width=this->width();
    height=this->height();

    X_max=width;
    Y_max=height;

    scr_border_x_min=(X_max-9*X_max/10);
    scr_border_x_max=(X_max-1*X_max/10);
    scr_border_y_min=(Y_max-9*Y_max/10);
    scr_border_y_max=(Y_max-1*Y_max/10);

    hStep=static_cast<qreal>(scr_border_x_max-scr_border_x_min)/static_cast<qreal>(gameFild.border_x_max-gameFild.border_x_min);
    vStep=static_cast<qreal>(scr_border_y_max-scr_border_y_min)/static_cast<qreal>(gameFild.border_y_max-gameFild.border_y_min);

    //---------- Make game fild ----------------------
    painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
    painter.drawLine(QPointF(scr_border_x_min,scr_border_y_min),QPointF(scr_border_x_max+hStep,scr_border_y_min));
    painter.drawLine(QPointF(scr_border_x_max+hStep,scr_border_y_min),QPointF(scr_border_x_max+hStep,scr_border_y_max+vStep));
    painter.drawLine(QPointF(scr_border_x_max+hStep,scr_border_y_max+vStep),QPointF(scr_border_x_min,scr_border_y_max+vStep));
    painter.drawLine(QPointF(scr_border_x_min,scr_border_y_max+vStep),QPointF(scr_border_x_min,scr_border_y_min));
    painter.drawText(scr_border_x_min,scr_border_y_min-5, " Game SNAKE  ");


    //  =====================================================

        if (PST==game_stop)
        {
            //            // -- menu border
            painter.drawLine(QPointF(scr_border_x_max/2-25,scr_border_y_max/2-10),QPointF(scr_border_x_max/2+65,scr_border_y_max/2-10));
            painter.drawLine(QPointF(scr_border_x_max/2+65,scr_border_y_max/2-10),QPointF(scr_border_x_max/2+65,scr_border_y_max/2+45));
            painter.drawLine(QPointF(scr_border_x_max/2+65,scr_border_y_max/2+45),QPointF(scr_border_x_max/2-25,scr_border_y_max/2+45));
            painter.drawLine(QPointF(scr_border_x_max/2-25,scr_border_y_max/2+45),QPointF(scr_border_x_max/2-25,scr_border_y_max/2-10));
//            // -- menu text
            painter.drawText(scr_border_x_max/2-20,scr_border_y_max/2,"       MENU:");
            painter.drawText(scr_border_x_max/2-20,scr_border_y_max/2+10,"NEW GAME.....'n'");
            painter.drawText(scr_border_x_max/2-20,scr_border_y_max/2+20,"MENU/PAUSE..'m'");
            painter.drawText(scr_border_x_max/2-20,scr_border_y_max/2+30,"CONTINUE.....'c'");
            painter.drawText(scr_border_x_max/2-20,scr_border_y_max/2+40,"EXIT......'e'");
        }


        if (PST==game_on)
        {
            painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
            GameController->getRabbitPlace(pen);
            painter.drawRect(static_cast<int>(scr_border_x_min+pen._x*hStep),
                             static_cast<int>(scr_border_y_min+pen._y*vStep),
                             static_cast<int>(hStep),
                             static_cast<int>(vStep));



            for(i=0;i<GameController->getSnakeLen();i++ ){
                GameController->getSnakeBodyPartsCords(i,pen);
                painter.drawRect(static_cast<int>(scr_border_x_min+pen._x*hStep),
                                 static_cast<int>(scr_border_y_min+pen._y*vStep),
                                 static_cast<int>(hStep),
                                 static_cast<int>(vStep));
            }

//            //====  information ====
              str_BUF1=QString::number(GameController->getGameScore());
              painter.drawText(scr_border_x_min,scr_border_y_max+15,"Score-");
              painter.drawText(scr_border_x_min+35,scr_border_y_max+15,str_BUF1);

              str_BUF2=QString::number(GameController->getGameLevel());
              painter.drawText(scr_border_x_min,scr_border_y_max+25,"Level-");
              painter.drawText(scr_border_x_min+35,scr_border_y_max+25,str_BUF2);

              str_BUF3= QString::number((GTC.getGameTime().hour))+
                      QString(":")+
                      QString::number(GTC.getGameTime().min)+
                      QString(":")+
                      QString::number(GTC.getGameTime().sec);
              painter.drawText(scr_border_x_min+100,scr_border_y_max+15,"Time-");
              painter.drawText(scr_border_x_min+135,scr_border_y_max+15,str_BUF3);
       }
        if (PST==game_new_level)
              painter.drawText(scr_border_x_max/2-30,scr_border_y_max/2-20,"N E X T    L E V E L !!!!!");

        if (PST==game_over)
             painter.drawText(scr_border_x_max/2-30,scr_border_y_max/2-20,"G A M E   O V E R !!!!!");
}

void MainWindow::_tic()
{
   Main_Loop();
   this->repaint();
}

bool    MainWindow::Main_Loop()
{
    switch (PST){
        case game_exit:
            GameController->setGameStatus(game_over);
            GameController->setGameStatus(game_exit);
            timer->stop();
            this->close();
            break;
        case game_new:
            GameController->setGameStatus(game_stop);
            GameController->setGameStatus(game_over);
            mvf=static_cast<MoveDirection>(0);
            if ((delay_cnt)==1){
                GameController->setGameStatus(game_new);
                GameController->setGameStatus(game_on);
                PST=game_on;
                delay_cnt=0;
            }
            else
                delay_cnt++;
            timer->stop();
            timer->start(TimeBase );
            GTC.ResetCount();
            break;
        case game_stop:
            GameController->setGameStatus(game_stop);
            delay_cnt=0;
            break;
        case game_on:

            if(GameController->getGameStatus()!=game_over){
                GameController->setGameStatus(game_on);
            }
            if(GameController->getGameStatus()==game_over)
                PST=game_stop;

            break;
        case game_over:
            if ((delay_cnt++)>GamePause)
                PST=game_stop;
            break;

        default:
            break;
    }

        if (GameController->getGameStatus()==game_on)
        {
                GameController->SnakeControl(mvf);
                GameController->SnakeMoveToOneStep();
                GTC.CountUp();
        }

        if (GameController->getGameStatus()==game_new_level){
            PST=game_new_level;
            if ((delay_cnt++)>GamePause){

                GameController->setGameStatus(game_stop);
                GameController->setGameStatus(game_over);
                GameController->setGameStatus(game_new);
                GameController->setGameStatus(game_on);
                mvf=static_cast<MoveDirection>(0);
                delay_cnt=0;
                PST=game_on;
                timer->stop();
                timer->start(TimeBase-(GameController->getGameLevel()-1)*LevelTimeStep);

            }
        }

        qDebug()<<"Prg. tic. PST-"<<PST;

        return true;
}

//==============================================================





