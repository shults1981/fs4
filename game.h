#ifndef GAME_H
#define GAME_H


#include "unit.h"


//=============== for DEBUG
#include <fstream>
//-------------------------


typedef enum _game_status{game_exit=0,game_menu,game_stop,game_on,game_over,game_new,game_new_level} GameStatus;
typedef enum _unit_status{unit_is_dead=0,unit_is_alive} UnitStatus;
typedef enum _move_direction{Left=1,Right,Up,Down} MoveDirection;


struct Fild
{
    int border_x_min;
    int border_x_max;
    int border_y_min;
    int border_y_max;
};


class Game
{
private:
    int GameLevel;
    int GameScore;
    int NumNextLevelJump;
    int Kill_Self_Flag;//0- without self kill contorl; 1-with self kill control
    Snake snake;
    Rabbit rabbit;
    GameStatus GST;
    Fild GameFild;
    UnitStatus rabbitStatus,snakeStatus;
    MoveDirection move_flag;

    Game();

private:
    int newGameUnitsSet();
    void RabbitFactory();
    int ResetUnits();


public:

    Game(const Fild gamefild,int num_next_level_jump,int kill_self_flag);
    ~Game();
    int setGameStatus(GameStatus gst);
    GameStatus getGameStatus();
    int getSnakeLen();
    int getRabbitLen();
    int getRabbitPlace(Point &PointDEST);
    int getSnakeBodyPartsCords(int BodyPartIndex, Point &PointDEST);
    int SnakeControl(MoveDirection md);
    int SnakeMoveToOneStep();
    Fild getGameFild();
    int getGameScore();
    int getGameLevel();


    //int DBG_f1();
};


#endif // GAME_H
