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

    u8 getGroupID() const { return mPointData->uniqueGroupID; }

    void getPosition(JGeometry::TVec3f *pos)
    {
        if (mPointData)
        {
            pos->set(mPointData->pos);
            return;
        }
        pos->zero();
    }

    // Inline
    CLPoint *getPrevPoint(int divNo)
    {
#line 84
        JUT_ASSERT_F(mPrevPoints[divNo], "P:%5.3f,%5.3f,%5.3f:%d", mPointData->pos.x, mPointData->pos.y, mPointData->pos.z, divNo);
        JUT_MINMAX_ASSERT(0, divNo, 4);
        return mPrevPoints[divNo];
    }

    CLPoint *getNextPoint(int divNo)
    {
        JUT_ASSERT_F(mNextPoints[divNo], "N:%5.3f,%5.3f,%5.3f:%d", mPointData->pos.x, mPointData->pos.y, mPointData->pos.z, divNo);
        JUT_MINMAX_ASSERT(0, divNo, 4);
        return mNextPoints[divNo];
    }
    CLPoint *getDivPoint() { return mDivPoint; }
    const CrsData::SCLPoint *getPointData() { return mPointData; }

    void setKind(u16 kind) { mKind = kind; }
    u16 getKind() const { return mKind; }
    void setPointData(const CrsData::SCLPoint *pointData) { mPointData = pointData; }

private:
    u16 mKind;
    const CrsData::SCLPoint *mPointData;
    CLPoint *mPrevPoints[4];
    CLPoint *mNextPoints[4];

public:
    CLPoint *mDivPoint;
}; // Size: 0x2c

class CLPointController
{
public:
    CLPointController() { reset(); }

    void reset();
    void init_ByKartIndex(int);
    void init_ByCLPointPtr(CLPoint *);
    bool chkArriveAtFwPoint(const JGeometry::TVec3f &, f32);
    bool chkIsOverFwPoint(const JGeometry::TVec3f &);
    bool doNextChkIsOverFwPoint(const JGeometry::TVec3f &);
    void getLengthFromCLDir(const JGeometry::TVec3f &);
    void changeNextPoint();
    void calcCLDir();
    void calcFirstCLbyAngle(const JGeometry::TVec3f &, int, f32);
    CLPoint *calcNextPoint(CLPoint *);
    void findRndPointNoExceptForShortCut(CLPoint *, stRandom *);
    void recursiveTargetCalc(CLPoint *, int, int *, u8 *, bool *);

    // Inline/Unused
    void doNextChkArriveAtFwPoint(const JGeometry::TVec3f &, f32);
    void chkLRforTargetDir(const JGeometry::TVec3f &, f32 *);

    // Vtable
    virtual int findNextPointNo(CLPoint *);

    CLPoint *getNext() const { return mNext; }
    CLPoint *getPrev() const { return mPrev; }

    bool tstReverse() const { return mIsReverse; }

    CLPoint *mNext; // 4
    CLPoint *mPrev; // 8
    JGeometry::TVec3f _c;
    JGeometry::TVec3f _18;
    bool mIsReverse; // 24
};

#endif
