#ifndef COURSE_AIPATH_H
#define COURSE_AIPATH_H

#include <dolphin/mtx.h>

class AIPoint
{
public:
    AIPoint() : mPos(nullptr) {}
    void setPosition(const Vec *pos) { mPos = pos; }

    const Vec *mPos;
};

class AIPath
{
public:
    AIPath() { init(0); }
    ~AIPath() {}

    void init(int id)
    {
        mId = id;
        if (id > 0)
            mPoints = new AIPoint[id];
        else
            mPoints = nullptr;
    }

    AIPoint *getPoint(int id)
    {
        if (id < mId)
            return &mPoints[id];
        return nullptr;
    }

    int mId;
    AIPoint *mPoints;
};

#endif