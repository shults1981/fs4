#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include<QDebug>
#include<QString>

#include "game.h"
#include "unit.h"
#include "menu.h"


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
    TimeBase=250;
    LevelTimeStep=15;
    GamePause=5;
    delay_cnt=0;

    gameFild.border_x_min=0;
    gameFild.border_x_max=50;
    gameFild.border_y_min=0;
    gameFild.border_y_max=50;

    //
    sx=0;
    sx_=0;
    sy=0;
    sy_=0;
    sz=0;
    sz_=0;

    flag_vrt=false;
    flag_hor=false;

    //counters
    cnt1=0;
    cnt2=0;


    m=new menu(this);
    s=QApplication::primaryScreen();

    GameController=new Game(gameFild,6,0);
    timer=new QTimer(this);
    scene=new QGraphicsScene();

    tmr1=new QTimer(this);
    pRs=new QRotationSensor(this);




    ui->setupUi(this);
    m->setMinimumHeight(this->height());
    m->setMinimumWidth(this->width());
    m->setMaximumHeight(this->height());
    m->setMaximumWidth(this->width());
    m->setWindowFlag(Qt::CustomizeWindowHint);
    m->setModal(true);
    ui->menu->setEnabled(false);
    s->setOrientationUpdateMask(
               Qt::PortraitOrientation
               | Qt::LandscapeOrientation
               | Qt::InvertedPortraitOrientation
               | Qt::InvertedLandscapeOrientation );
    scrOrient=s->orientation();



  //  ui->menu->setVisible(false);

    ui->_level->setText("0");
    ui->_score->setText("0");
    ui->_time->setText("00:00:00");




    // Start timers and sensor
    timer->start(TimeBase);
    tmr1->start(100);
    pRs->start();




    connect(timer, SIGNAL(timeout()), this, SLOT(_tic()));
    connect(m,SIGNAL(enterData(int)),this,SLOT(game_menu(int)));
    connect(tmr1,SIGNAL(timeout()),this,SLOT(RotUpdateData()));
    connect(s,SIGNAL(orientationChanged(Qt::ScreenOrientation)),
            this,
            SLOT(ScrOrientationChanged(Qt::ScreenOrientation)));

}

MainWindow::~MainWindow()
{
    timer->stop();
    tmr1->stop();
    pRs->stop();


    delete GameController;

    delete ui;
}


void    MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


    Point pen;
    int width, height;

    int i;
    int scr_border_x_min,scr_border_x_max,scr_border_y_min,scr_border_y_max;
    qreal hStep,vStep;

    QString str_BUF1,str_BUF2,str_BUF3;
    QPolygonF border;
    QRect r;

    width=ui->MoveFild->width()-4;
    height=ui->MoveFild->height()-4;

    X_max=width;
    Y_max=height;

    scr_border_x_min=(X_max-9*X_max/10);
    scr_border_x_max=(X_max-1*X_max/10);
    scr_border_y_min=(Y_max-9*Y_max/10);
    scr_border_y_max=(Y_max-1*Y_max/10);

    hStep=static_cast<qreal>(scr_border_x_max-scr_border_x_min)/static_cast<qreal>(gameFild.border_x_max-gameFild.border_x_min);
    vStep=static_cast<qreal>(scr_border_y_max-scr_border_y_min)/static_cast<qreal>(gameFild.border_y_max-gameFild.border_y_min);

    scene->setSceneRect(0,0,width,height);


    //---------- Make game fild ----------------------

    scene->clear();
    border<<QPointF (scr_border_x_min,scr_border_y_min);
    border<<QPointF(scr_border_x_max+hStep,scr_border_y_min);
    border<<QPointF(scr_border_x_max+hStep,scr_border_y_max+vStep);
    border<<QPointF(scr_border_x_min,scr_border_y_max+vStep);
    border<<QPointF(scr_border_x_min,scr_border_y_min);
    scene->addPolygon(border);

    //  =====================================================

        if (PST==game_on)
        {

            GameController->getRabbitPlace(pen);
            r=QRect(static_cast<int>(scr_border_x_min+pen._x*hStep),
                                                 static_cast<int>(scr_border_y_min+pen._y*vStep),
                                                 static_cast<int>(hStep),
                                                 static_cast<int>(vStep));
            scene->addRect(r,QPen(Qt::black),QBrush(Qt::yellow));




            for(i=0;i<GameController->getSnakeLen();i++ ){
                GameController->getSnakeBodyPartsCords(i,pen);
                r=QRect (static_cast<int>(scr_border_x_min+pen._x*hStep),
                         static_cast<int>(scr_border_y_min+pen._y*vStep),
                         static_cast<int>(hStep),
                         static_cast<int>(vStep));
                scene->addRect(r,QPen(Qt::black),QBrush(Qt::green));


            }

//            //====  information ====
              str_BUF1=QString::number(GameController->getGameScore());
              ui->_score->setText(str_BUF1);


              str_BUF2=QString::number(GameController->getGameLevel());
              ui->_level->setText(str_BUF2);

              str_BUF3= QString::number((GTC.getGameTime().hour))+
                      QString(":")+
                      QString::number(GTC.getGameTime().min)+
                      QString(":")+
                      QString::number(GTC.getGameTime().sec);
              ui->_time->setText(str_BUF3);


       }

        if (PST==game_on)
            scene->addText(" Game  ");
        if (PST==game_new_level)
                scene->addText("N E X T    L E V E L !!!!!");
        if (PST==game_over)
                scene->addText("G A M E   O V E R !!!!!");

        ui->MoveFild->setScene(scene);

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
            ui->menu->setEnabled(false);
            //ui->menu->setVisible(false);
            m->show();
            break;
        case game_on:

            if(GameController->getGameStatus()!=game_over){
                GameController->setGameStatus(game_on);
                    ui->menu->setEnabled(true);
                    //ui->menu->setVisible(true);
            }
            if(GameController->getGameStatus()==game_over)
                PST=game_over;

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

//        qDebug()<<"Prg. tic. PST-"<<PST;

        return true;
}

//==============================================================

void MainWindow::on_menu_clicked()
{
              PST=game_stop;
}

void MainWindow::on_Button_Up_clicked()
{
    if (PST==game_on){
            mvf=Up;
//            qDebug()<<"Pres Key_Up button";
    }
}

void MainWindow::on_Button_Down_clicked()
{
    if (PST==game_on){
           mvf=Down;
//           qDebug()<<"Pres Key_Down button";
      }
}

void MainWindow::on_Button_Left_clicked()
{
    if (PST==game_on){
            mvf=Left;
//            qDebug()<<"Pres Key_Left button";
    }
}

void MainWindow::on_Button_Right_clicked()
{
    if (PST==game_on){
             mvf=Right;
//            qDebug()<<"Pres Key_Right button";
    }
}

void MainWindow::game_menu(int st)
{
    if (PST==game_stop||PST==game_over){
        switch (st) {
        case 0:
//            qDebug()<<"set EXIT";
            PST=game_exit;
            break;
        case 1:
//            qDebug()<<"set NEW";
            PST=game_new;
            break;
        case 2:
//            qDebug()<<"set CONTINUE";
            PST=game_on;
            break;
        default:
            break;
        }
    }
}

void MainWindow::RotUpdateData()
{

    qreal _rx, _ry,_rz;

    // set value S[i-1]
    sx_=sx;
    sy_=sy;
    sz_=sz;

    // read value S[i]
    sx=_rx=pRs->reading()->x();
    sy=_ry=pRs->reading()->y();
    sz=_rz=pRs->reading()->z();

    // calc delta= S[i]-S[i-1]
    dx=sx-sx_;
    dy=sy-sy_;
    dz=sz-sz_;

    // output

    // ---


    if (scrOrient==Qt::PortraitOrientation){
        if(!flag_vrt){
            if (dx>0 && qAbs(dx)>5){
//              ("--UP--");
                flag_vrt=true;
                    if (PST==game_on){
                         mvf=Up;
                    }
            }
            if (dx<0 && qAbs(dx)>5){
//              ("---DOWN---");
                if (PST==game_on){
                     mvf=Down;
                }
                flag_vrt=true;
            }
        }
        else {
            cnt1++;
            if (cnt1>=4){
                cnt1=0;
                flag_vrt=false;
            }

        }

        if (!flag_hor) {
            if (dy>0 && qAbs(dy)>5){
//              ("--LEFT--");
                if (PST==game_on){
                     mvf=Left;
                }
                flag_hor=true;
            }
            if (dy<0 && qAbs(dy)>5){
//              ("---RIGHT---");
                if (PST==game_on){
                     mvf=Right;
                }
                flag_hor=true;
            }
        }
        else {
            cnt2++;
            if (cnt2>=4){
                cnt2=0;
                flag_hor=false;
            }

        }
    }

    if (scrOrient==Qt::LandscapeOrientation){
        if(!flag_vrt){
            if (dy>0 && qAbs(dy)>3){
 //              ("---DOWN---");
                flag_vrt=true;
                if (PST==game_on){
                     mvf=Down;
                }
            }
            if (dy<0 && qAbs(dy)>3){
//              ("--UP--");
                flag_vrt=true;
                if (PST==game_on){
                     mvf=Up;
                }
            }
        }
        else {
            cnt1++;
            if (cnt1>=4){
                cnt1=0;
                flag_vrt=false;
            }

        }

        if (!flag_hor) {
            if (dx>0 && qAbs(dx)>3){
//              ("--LEFT--");
                flag_hor=true;
                if (PST==game_on){
                     mvf=Left;
                }
            }
            if (dx<0 && qAbs(dx)>3){
//              ("---RIGHT---");
                if (PST==game_on){
                     mvf=Right;
                }
                flag_hor=true;
            }
        }
        else {
            cnt2++;
            if (cnt2>=4){
                cnt2=0;
                flag_hor=false;
            }

        }
    }

    if (scrOrient==Qt::InvertedLandscapeOrientation){
        if(!flag_vrt){
            if (dy>0 && qAbs(dy)>3){
//              ("--UP--");
                flag_vrt=true;
                if (PST==game_on){
                     mvf=Up;
                }
            }
            if (dy<0 && qAbs(dy)>3){
//              ("---DOWN---");
                flag_vrt=true;
                if (PST==game_on){
                     mvf=Down;
                }
            }
        }
        else {
            cnt1++;
            if (cnt1>=4){
                cnt1=0;
                flag_vrt=false;
            }

        }

        if (!flag_hor) {
            if (dx>0 && qAbs(dx)>3){
//              ("---RIGHT---");
                flag_hor=true;
                if (PST==game_on){
                     mvf=Right;
                }
            }
            if (dx<0 && qAbs(dx)>3){
 //          ("--LEFT--");
                flag_hor=true;
                if (PST==game_on){
                     mvf=Left;
                }
            }
        }
        else {
            cnt2++;
            if (cnt2>=4){
                cnt2=0;
                flag_hor=false;
            }

        }
    }

}

void MainWindow::ScrOrientationChanged(Qt::ScreenOrientation orientation)
{
    scrOrient= orientation;
}


