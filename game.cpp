#include "game.h"

#include "game.h"
#include <stdlib.h>
#include <time.h>

#include <iostream>



Game::Game(const Fild game_fild, int num_next_level_jump, int kill_self_flag ):snake(),rabbit()
{
    GST=game_stop;
    GameFild=game_fild;
    Kill_Self_Flag=kill_self_flag;
    rabbitStatus=unit_is_dead;
    snakeStatus=unit_is_dead;
    GameScore=0;
    GameLevel=1;
    NumNextLevelJump=num_next_level_jump;

    srand(time(0));


}

Game::~Game() { }


int Game::newGameUnitsSet()
{
    if((GST==game_new)){
        Point bufVar;
        bufVar._x=(GameFild.border_x_max-GameFild.border_x_min)/2;
        bufVar._y=(GameFild.border_y_max-GameFild.border_y_min)/2;
        bufVar._d=rand()%4+1;
        move_flag=static_cast<MoveDirection>(bufVar._d);
        snake.addNewElementInUnitBody(bufVar);
        if (snakeStatus==unit_is_dead){
            GameLevel=1;
        }
        snakeStatus=unit_is_alive;
        rabbitStatus=unit_is_dead;
        RabbitFactory();
        GameScore=0;
        return 1;
    }
    else
        return 0;
}

int Game::setGameStatus(GameStatus gst)
{

    if ((gst==game_exit) && (GST==game_over)){
        GST=game_over;
        ResetUnits();
        GST=game_exit;
    }

    if ((gst==game_over) && (GST==game_stop))	{
        GST=game_over;
        ResetUnits();
    }

    if ((gst==game_new) && ((GST==game_over)||(GST==game_stop))){
        GST=game_over;
        ResetUnits();
        GST=game_new;
        newGameUnitsSet();
    }

    if ((gst==game_stop) && ((GST==game_on)||(GST==game_new_level))){
        GST=game_stop;
    }

    if (gst==game_new_level){
        gst=GST;
    }

    if((gst==game_on) && ((GST==game_new)||(GST==game_stop))){
        GST=game_on;
    }

    return 1;
 }

GameStatus Game::getGameStatus(){ return GST; }


int Game::getSnakeLen()
{
    return snake.getBodyLen();
}

int Game::getRabbitLen()
{
    return rabbit.getBodyLen();
}

void Game::RabbitFactory()
{
    if (!rabbitStatus){
        Point bufVar;
        bufVar._x=GameFild.border_x_min+rand()%(GameFild.border_x_max-GameFild.border_x_min-1)+1;
        bufVar._y=GameFild.border_y_min+rand()%(GameFild.border_y_max-GameFild.border_y_min-1)+1;
        bufVar._d=0;
        rabbit.setBodyElement(0,bufVar);
        rabbitStatus=unit_is_alive;
    }

}


int Game::getRabbitPlace(Point &PointDEST)
{
    return rabbit.getBodyCords(0,PointDEST);
}

int  Game::getSnakeBodyPartsCords(int BodyPartIndex, Point &PointDEST)
{
    if ((BodyPartIndex>=0)&&(BodyPartIndex<(snake.getBodyLen()))){
        snake.getBodyCords(BodyPartIndex,PointDEST);
        return 1;
    }
    else
        return 0;
}


int Game::SnakeMoveToOneStep()
{
    int i,j,k,turn_flag,self_closure_flag;
    unsigned int kill_self=0,border_crash=0;
    Point tempPoint1,tempPoint2,tempPoint3;

    if (GST==game_on){

        snake.getBodyCords(0,tempPoint1);
        border_crash=!((tempPoint1._x>GameFild.border_x_min)&&(tempPoint1._x<GameFild.border_x_max)&&(tempPoint1._y>GameFild.border_y_min) &&(tempPoint1._y<GameFild.border_y_max));

        if (Kill_Self_Flag){
            for (i=1;i<snake.getBodyLen();i++){
                snake.getBodyCords(i,tempPoint2);
                if ((tempPoint1._x==tempPoint2._x)&&(tempPoint1._y==tempPoint2._y))
                    kill_self=1;
            }
        }


        snake.getBodyCords(0,tempPoint1);
        if (tempPoint1._d!=(int)move_flag){
            tempPoint1._d=(int)move_flag;
            snake.setBodyElement(0,tempPoint1);
            if((snake.getBodyLen()>1) /* && (snake.getBodyTPANum()<(snake.getBodyLen()))*/ )
                snake.addNewElementInBodyTPA(tempPoint1);
        }
        else{
            self_closure_flag=0;
            i=snake.getBodyTPANum()-1;
            while (i>=0 && (!self_closure_flag)){
                snake.getBodyTPA(i,tempPoint2);
                if ((tempPoint1._x==tempPoint2._x) && (tempPoint1._y==tempPoint2._y)){
                    snake.addNewElementInBodyTPA(tempPoint1);
                    self_closure_flag=1;
                }
                i-=1;
            }
        }

        if (k=snake.getBodyTPANum())
            k-=1;

        for (i=0;i<snake.getBodyLen();i++){
            turn_flag=0;
            snake.getBodyCords(i,tempPoint2);
            if(snake.getBodyTPANum()){
                j=k;
                while ((j>=0) && (!turn_flag)) {
                    snake.getBodyTPA(j,tempPoint3);
                    if((tempPoint2._x==tempPoint3._x) && (tempPoint2._y==tempPoint3._y)){
                        switch(tempPoint3._d)
                        {
                        case 1:
                            tempPoint2._x-=1;
                            break;
                        case 2:
                            tempPoint2._x+=1;
                            break;
                        case 3:
                            tempPoint2._y-=1;
                            break;
                        case 4:
                            tempPoint2._y+=1;
                            break;
                        default:break;
                        }
                        tempPoint2._d=tempPoint3._d;
                        if((i==snake.getBodyLen()-1) && (j==0)){
                            snake.delElementFromBodyTPA();
                        }
                        turn_flag=1;
                        k-=1;
                    }
                    j-=1;
                }
            }
            if (!turn_flag){
                switch(tempPoint2._d)
                {
                case 1:
                    tempPoint2._x-=1;
                    break;
                case 2:
                    tempPoint2._x+=1;
                    break;
                case 3:
                    tempPoint2._y-=1;
                    break;
                case 4:
                    tempPoint2._y+=1;
                    break;
                default:
                    break;
                }
            }
            snake.setBodyElement(i,tempPoint2);
        }
//-------------body control and manage-------------
        snake.getBodyCords(0,tempPoint1);
        rabbit.getBodyCords(0,tempPoint2);
        if ((tempPoint1._x==tempPoint2._x)&&(tempPoint1._y==tempPoint2._y)){
            snake.addNewElementInUnitBody(tempPoint2);
            GameScore+=1;
            rabbitStatus=unit_is_dead;
            RabbitFactory();
        }

//-------------------------------------------------
/*


*/
        if (GameScore>=NumNextLevelJump){
            GST=game_new_level;
            GameLevel++;
        }

        if (border_crash || kill_self ) {
            GST=game_over;
            snakeStatus=unit_is_dead;
        }
        else {	}
    }
    return 1;
}

int Game::SnakeControl(MoveDirection md)
{
    Point tempPoint1;

    snake.getBodyCords(0,tempPoint1);

    switch(md)
    {
    case Left:
        if (snake.getBodyLen()==1){
            move_flag=Left;
        }
        else
            if((snake.getBodyLen()>1)&&(tempPoint1._d!=2))
                move_flag=Left;
        break;

    case Right:
        if (snake.getBodyLen()==1){
            move_flag=Right;
        }
        else
            if((snake.getBodyLen()>1)&&(tempPoint1._d!=1))
                move_flag=Right;
        break;

    case Up:
        if (snake.getBodyLen()==1)
            move_flag=Up;
        else
            if((snake.getBodyLen()>1)&&(tempPoint1._d!=4))
                move_flag=Up;
        break;

    case Down:
        if (snake.getBodyLen()==1)
            move_flag=Down;
        else
            if((snake.getBodyLen()>1)&&(tempPoint1._d!=3))
                move_flag=Down;
        break;

    default: break;


    }
    return 1;

}


Fild Game::getGameFild()
{
    return GameFild;
}


int Game::ResetUnits()
{

    if (GST==game_over){
        snake.ClearBodyAndTPA();
        rabbit.ClearBodyAndTPA();
    }

    return 1;
}

int Game::getGameScore()
{
    return GameScore;

}

int Game::getGameLevel()
{
    return GameLevel;
}

//int Game::DBG_f1()
//{
//	return snake.getBodyTPANum();
//}
