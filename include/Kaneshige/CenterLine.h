#ifndef CENTERLINE_H
#define CENTERLINE_H

#include <JSystem/JGeometry.h>
#include <JSystem/JUtility/JUTDbg.h>

#include "Kaneshige/Course/CrsData.h"

class CLPoint
{
public:
    CLPoint();
    bool setPrevPoint(CLPoint *);
    bool setNextPoint(CLPoint *);
    int getPrevPointNumber();
    int getNextPointNumber();
    int getBrosPointNumber();
    CLPoint *getBrosPoint(int);

    // Inline
    CLPoint *getPrevPoint(int divNo)
    {
#line 84
        JUT_ASSERT_F(mNextPoints[divNo], "P:%5.3f,%5.3f,%5.3f:%d", mPointData->mPos.x, mPointData->mPos.y, mPointData->mPos.z, divNo);
        JUT_MINMAX_ASSERT(0, divNo, 4);
        return mPrevPoints[divNo];
    }
    CLPoint *getNextPoint(int divNo) 
    {
        JUT_ASSERT_F(mNextPoints[divNo], "N:%5.3f,%5.3f,%5.3f:%d", mPointData->mPos.x, mPointData->mPos.y, mPointData->mPos.z, divNo);
        JUT_MINMAX_ASSERT(0, divNo, 4);
        return mNextPoints[divNo];
    }
    CLPoint *getDivPoint() { return mDivPoint; }
private:
    u16 mKind;
    CrsData::SCLPoint *mPointData;
    CLPoint *mPrevPoints[4];
    CLPoint *mNextPoints[4];
    CLPoint *mDivPoint;
}; // Size: 0x2c

#endif