/*
|*****************************************************************
|*****************************************************************
* Project                        :  <Funny Snake #3>
* Programm name                  :  funny_snake21.cpp
* Author                         :  Shults1981
* Data create                    :  07/03/2019
* Purpose                        :  classical game "SNAKE";
                                    testing library Qt
                                    for mobile app
|****************************************************************
|****************************************************************
*/


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
