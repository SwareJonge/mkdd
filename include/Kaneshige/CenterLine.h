#ifndef CENTERLINE_H
#define CENTERLINE_H

class CLPoint
{
public:
    CLPoint();
    void setPrevPoint(CLPoint *);
    void setNextPoint(CLPoint *);
    int getPrevPointNumber();
    int getNextPointNumber();
    int getBrosPointNumber();
    int getBrosPoint(int);

private:
    // TODO
};

#endif