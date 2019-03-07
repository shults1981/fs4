// ---------#include <stdlib.h>
#include "unit.h"
#include <stddef.h>
#include <iostream>
//-------------------------------------
//-----------------methods of class PointArr

PointArr::PointArr()
{

    arrLen=1;
    pArr=new Point;
    if (pArr){
        pArr->_x=0;
        pArr->_y=0;
        pArr->_d=0;
    }
}

PointArr::PointArr(int Len)
{

    if (Len==0){
        arrLen=0;
        pArr=NULL;
    /*	pArr=new Point;
        if (pArr){
            pArr->_x=0;
            pArr->_y=0;
            pArr->_d=0;
        }*/
    }
    else {
        arrLen=Len;
        pArr=new Point [arrLen];
        if (pArr){
            for(int i=0;i<arrLen;i++){
                pArr[i]._x=0;
                pArr[i]._y=0;
                pArr[i]._d=0;
            }
        }
    }
}

PointArr::PointArr(const PointArr &PointSRC)
{
    Point tVar;
    arrLen=PointSRC.arrLen;
    pArr=new Point[arrLen];
    if (pArr){
        for (int i=0;i<arrLen;i++){
            if(PointSRC.getElement(i,tVar)){
                pArr[i]._x=tVar._x;
                pArr[i]._y=tVar._y;
                pArr[i]._d=tVar._d;
            }
        }
    }
}

PointArr & PointArr::operator= (const PointArr & PointSRC)
{
    Point *tVar1,*tVar2;
    Point tVar3;
    if (this==&PointSRC)
        return *this;
    else{
        tVar1=new Point[PointSRC.getLen()];
        if (tVar1){
            for(int i=0; i<PointSRC.getLen();i++){
                if(PointSRC.getElement(i,tVar3)){
                    tVar1[i]._x=tVar3._x;
                    tVar1[i]._y=tVar3._y;
                    tVar1[i]._d=tVar3._d;
                }
            }
        }

        tVar2=tVar1;
        tVar1=this->pArr;
        this->pArr=tVar2;
        this->arrLen=PointSRC.getLen();

        delete []tVar1;
        return *this;
    }
}


PointArr::~PointArr()
{

    if (pArr)
        delete []pArr;
}


int PointArr::setElement(const Point & PointSRC, int index) const
{
    if (!pArr && !arrLen)
        return -1;

    if ((index>=0)&&(index<arrLen)){
        pArr[index]._x=PointSRC._x;
        pArr[index]._y=PointSRC._y;
        pArr[index]._d=PointSRC._d;
        return 1;
    }
    else
        return 0;
}

int  PointArr::getElement(int index, Point & pointDEST) const
{
/*
    if ((index==0)&&(arrLen==0)){
            pointDEST._x=pArr->_x;
            pointDEST._y=pArr->_y;
            pointDEST._d=pArr->_d;
            return 1;
    }
*/
    if (!pArr && !arrLen)
        return -1;

    if ((index>=0)&&(index<arrLen)){
            pointDEST._x=pArr[index]._x;
            pointDEST._y=pArr[index]._y;
            pointDEST._d=pArr[index]._d;
        return 1;
    }
    else
        return 0;
}

int PointArr::delElement(int index)
{
    Point *tVar1,*tVar2;
    if ((index>=0)&&(index<arrLen)){
        tVar1=new Point[arrLen-1];
            if (tVar1){
                for(int i=0;i<index;i++){
                    tVar1[i]._x=pArr[i]._x;
                    tVar1[i]._y=pArr[i]._y;
                    tVar1[i]._d=pArr[i]._d;
                }
                for (int i=index;i<arrLen-1;i++){
                    tVar1[i]._x=pArr[i+1]._x;
                    tVar1[i]._y=pArr[i+1]._y;
                    tVar1[i]._d=pArr[i+1]._d;
                }
                tVar2=pArr;
                pArr=tVar1;
                arrLen-=1;
                delete []tVar2;
            }
            else
                return 0;
    }
    else
        return 0;

}


int PointArr::getLen() const
{
    return arrLen;
}


int PointArr::addElementInBack(const Point &PointSRC)
{
    Point *tVar1,*tVar2;

    arrLen+=1;

    tVar1=new Point[arrLen];
    if (tVar1){
        if (pArr!=NULL){
            for(int i=0;i<(arrLen-1);i++){
                tVar1[i]._x=pArr[i]._x;
                tVar1[i]._y=pArr[i]._y;
                tVar1[i]._d=pArr[i]._d;
            }
            tVar1[arrLen-1]._x=PointSRC._x;
            tVar1[arrLen-1]._y=PointSRC._y;
            tVar1[arrLen-1]._d=PointSRC._d;
        }
        else {
            tVar1->_x=PointSRC._x;
            tVar1->_y=PointSRC._y;
            tVar1->_d=PointSRC._d;
        }

        tVar2=pArr;
        pArr=tVar1;
        delete []tVar2;
        return 1;
    }
    else
        return 0;

}

int PointArr::delElementFromBack()
{
    Point *tVar1, *tVar2;
    arrLen-=1;

    if(arrLen!=0){
        tVar1=new Point[arrLen];
        if (tVar1){
            for(int i=0;i<(arrLen);i++){
                tVar1[i]._x=pArr[i]._x;
                tVar1[i]._y=pArr[i]._y;
                tVar1[i]._d=pArr[i]._d;
            }
            tVar2=pArr;
            pArr=tVar1;
        }
        else
           return 0;
    }

    else {
        tVar2=pArr;
        pArr=NULL;
    }

    delete []tVar2;
    return 1;
}

int PointArr::addElementInBegin(const Point &PointSRC)
{
    Point *tVar1,*tVar2;
    arrLen+=1;
    tVar1=new Point[arrLen];
    if (tVar1){
        if (pArr!=NULL){
            tVar1[0]._x=PointSRC._x;
            tVar1[0]._y=PointSRC._y;
            tVar1[0]._d=PointSRC._d;
            for(int i=1;i<arrLen;i++){
                tVar1[i]._x=pArr[i-1]._x;
                tVar1[i]._y=pArr[i-1]._y;
                tVar1[i]._d=pArr[i-1]._d;
            }
        }
        else {
            tVar1->_x=PointSRC._x;
            tVar1->_y=PointSRC._y;
            tVar1->_d=PointSRC._d;
        }

        tVar2=pArr;
        pArr=tVar1;
        delete []tVar2;
        return 1;
    }
    else
        return 0;

}

int PointArr::delElementFromBegin()
{
    Point *tVar1, *tVar2;
    arrLen-=1;

    if(arrLen!=0){
        tVar1=new Point[arrLen];
        if (tVar1){
        for(int i=0;i<(arrLen);i++){
                tVar1[i]._x=pArr[i+1]._x;
                tVar1[i]._y=pArr[i+1]._y;
                tVar1[i]._d=pArr[i+1]._d;
            }
            tVar2=pArr;
            pArr=tVar1;
        }
        else
           return 0;
    }

    else {
        tVar2=pArr;
        pArr=NULL;
    }

    delete []tVar2;
    return 1;

}




int PointArr::insertElementAfterIndex(int index, Point &PointSRC)
{
    Point *tVar1,*tVar2;

    if ((index>=0)&&(index<arrLen)){
        if (index==(arrLen-1))
            return this->addElementInBack(PointSRC);
        else {
            tVar1=new Point [arrLen+1];
            if (tVar1){
                for(int i=0;i<=index;i++){
                    tVar1[i]._x=pArr[i]._x;
                    tVar1[i]._y=pArr[i]._y;
                    tVar1[i]._d=pArr[i]._d;
                }
                tVar1[index+1]._x=PointSRC._x;
                tVar1[index+1]._y=PointSRC._y;
                tVar1[index+1]._d=PointSRC._d;
                for (int i= index+2;i<arrLen+1;i++){
                    tVar1[i]._x=pArr[i-1]._x;
                    tVar1[i]._y=pArr[i-1]._y;
                    tVar1[i]._d=pArr[i-1]._d;
                }
                tVar2=pArr;
                pArr=tVar1;
                arrLen+=1;
                delete []tVar2;

                return 1;
            }
            else
                return 0;
        }
    }
    else
        return 0;



}
//---------------------------------------

//----------------------methods of class Unit


Unit::Unit(int UnitLen)
{

    BodyCords = new PointArr(UnitLen);
    BodyTPA= new PointArr(0);

}

Unit::~Unit()
{

    delete BodyCords;
    delete BodyTPA;
}

int Unit::addNewElementInUnitBody(const Point &PointSRC)
{
    return BodyCords->addElementInBack(PointSRC);
}

int Unit::addNewElementInBodyTPA(const Point &PointSRC)
{
    return BodyTPA->addElementInBack(PointSRC);
}

int Unit::setBodyElement(int BodyIndex,const Point &PointSRC)
{
    return BodyCords->setElement(PointSRC, BodyIndex);
}

int Unit::delElementFromBackOfUnitBody()
{
    return BodyCords->delElementFromBack();
}

int Unit::delElementFromBeginOfUnitBody()
{
    return BodyCords->delElementFromBegin();
}

int Unit::delElementFromBodyTPA()
{
    return BodyTPA->delElementFromBegin();
}

int Unit::getBodyLen()
{
    return BodyCords->getLen();
}

int Unit::getBodyTPANum()
{
    return BodyTPA->getLen();
}

int Unit::getBodyCords(int BodyPartIndex, Point &PointDEST)
{
    return BodyCords->getElement(BodyPartIndex,PointDEST);
}

int Unit::getBodyTPA(int TPAIndex, Point &PointDEST)
{
    return BodyTPA->getElement(TPAIndex,PointDEST);
}

int Unit::ClearBodyAndTPA()
{
    Point tempVar;
    int k;
    tempVar._x=0;
    tempVar._y=0;
    tempVar._d=0;
    if ((k=BodyCords->getLen())>1){
        for(int i=0;i<k-1;i++)
            BodyCords->delElementFromBack();
    }
    if ((k=BodyTPA->getLen())>0){
        for (int i=0;i<k;i++)
            BodyTPA->delElementFromBack();
    }
    BodyCords->setElement(tempVar,0);
    return 1;

}



//---------------------------------------------------------------------


//-------- methods of class Snake--------------------------------------

Snake::Snake():Unit(1) { }

Snake::~Snake() { }

int Snake::addNewElementInUnitBody(const Point &PointSRC)
{
    Point bufVar;

    if (!getBodyCords(getBodyLen()-1,bufVar))
        return 0;
    else {
        if ((!bufVar._x) && (!bufVar._y) && (!bufVar._d)){
            bufVar._x=PointSRC._x;
            bufVar._y=PointSRC._y;
            bufVar._d=PointSRC._d;
            return BodyCords->setElement(bufVar,0);
        }
        else {
            switch(bufVar._d){
            case 1:
                bufVar._x+=1;
                break;
            case 2:
                bufVar._x-=1;
                break;

            case 3:
                bufVar._y+=1;
                break;
            case 4:
                bufVar._y-=1;
                break;
            default: break;
            }
            return BodyCords->addElementInBack(bufVar);
        }
    }
}
int Snake::delElementFromBeginOfUnitBody()  { return 0;}

//-----------------------------------------------------------------------


//-------- methods of class Rabbit---------------------------------

Rabbit::Rabbit():Unit(1){   }

Rabbit::~Rabbit() { }

int Rabbit::addNewElementInUnitBody(const Point &PointSRC) { return 0; }

int Rabbit::addNewElementInBodyTPA(const Point &PointSRC) { return 0; }

int Rabbit::setBodyElement(int BodyIndex,const Point &PointSRC)
{
    return BodyCords->setElement(PointSRC,0);
}

int Rabbit::delElementFromBackOfUnitBody() { return 0; }

int Rabbit::delElementFromBeginOfUnitBody() {return 0; }

int Rabbit::delElementFromBodyTPA() {	return 0; }

int Rabbit::getBodyLen() { return BodyCords->getLen(); }

int Rabbit::getBodyTPANum() {	return 0; }

int Rabbit::getBodyCords(int BodyPartIndex, Point &PointDEST)
{
    return BodyCords->getElement(BodyPartIndex,PointDEST);
}

int Rabbit::getBodyTPA(int TPAIndex, Point &PointDEST) { return 0; }


//-----------------------------------------------------------------------
