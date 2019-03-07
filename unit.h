#ifndef UNIT_H
#define UNIT_H


//
struct Point
{
    int _x; //cord x
    int _y; //cord y
    int _d; //moving direction flag of the point
};


//------------------Class PointArr----------------------------------------

class PointArr
{
private:
    Point *pArr;
    int arrLen;
    PointArr();

public:

    PointArr(int Len);
    PointArr(const PointArr &PointSRC);
    ~PointArr();
    PointArr & operator=(const PointArr &PointSRC);
    int setElement (const Point &, int index) const;
    int addElementInBack(const Point &PointSRC);
    int delElementFromBack();
    int addElementInBegin(const Point &PointSRC);
    int delElementFromBegin();
    int getElement(int index,Point &PointDEST) const;
    int delElement(int index);
    int getLen() const;
    int insertElementAfterIndex(int index,Point &PointSRC);

};

//------------------ Class Unit  --------------------------------

class BaseUnitIFace
{
public:
    virtual int addNewElementInUnitBody(const Point &PointSRC)=0;
    virtual	int addNewElementInBodyTPA(const Point &PointSRC)=0;
    virtual int setBodyElement(int BodyIndex, const Point &PointSRC)=0;
    virtual	int delElementFromBackOfUnitBody()=0;
    virtual	int delElementFromBeginOfUnitBody()=0;
    virtual	int delElementFromBodyTPA()=0;
    virtual	int getBodyLen()=0;
    virtual	int getBodyTPANum()=0;
    virtual	int getBodyCords(int BodyPartIndex,Point &PointDEST)=0;
    virtual	int getBodyTPA(int TPAIndex,Point &PointDEST)=0;
    virtual int ClearBodyAndTPA()=0;
};




class Unit:public BaseUnitIFace
{
protected:
    PointArr *BodyCords;// coordinates of body parts
    PointArr  *BodyTPA; // coordinates of turning body points

private:
    Unit();
    Unit(Unit&);

public:
    Unit(int UnitLen);
    ~Unit();
    int addNewElementInUnitBody(const Point &PointSRC);
    int addNewElementInBodyTPA(const Point &PointSRC);
    int setBodyElement(int BodyIndex, const Point &PointSRC);
    int delElementFromBackOfUnitBody();
    int delElementFromBeginOfUnitBody();
    int delElementFromBodyTPA();
    int getBodyLen();
    int getBodyTPANum();
    int getBodyCords(int BodyPartIndex,Point &PointDEST);
    int getBodyTPA(int TPAIndex,Point &PointDEST);
    int ClearBodyAndTPA();

};


//-----------------Class Snake --------------------------


class Snake:public Unit
{
private:
    Snake(Snake &snake);

public:
    Snake();
    ~Snake();
    int addNewElementInUnitBody(const Point &PointSRC);
    int delElementFromBeginOfUnitBody ();
};


class Rabbit:public Unit
{
private:
    Rabbit(Rabbit &rabbit);
public:
    Rabbit();
    ~Rabbit();
    int addNewElementInUnitBody(const Point &PointSRC);
    int addNewElementInBodyTPA(const Point &PointSRC);
    int setBodyElement(int BodyIndex, const Point &PointSRC);
    int delElementFromBackOfUnitBody();
    int delElementFromBeginOfUnitBody();
    int delElementFromBodyTPA();
    int getBodyLen();
    int getBodyTPANum();
    int getBodyCords(int BodyPartIndex,Point &PointDEST);
    int getBodyTPA(int TPAIndex,Point &PointDEST);
};

#endif // UNIT_H
