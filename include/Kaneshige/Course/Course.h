#ifndef COURSE_H
#define COURSE_H

#include "types.h"

#include "Kaneshige/Course/CrsArea.h"

class Course
{
public:
    class Sector
    {
    public:
        int isMainSector() const;
        int getGeneration() const;
        Sector *getPrevSector(int);
        float calcUnitDist(JGeometry::TVec3<float> const &);

        bool isDiv() const
        {
            return bDivFlag;
        }

        bool isRevSearchEnable()
        {
            return mCheckPoint->mRevSearchEnable != 0;
        }

        void getBNormal(JGeometry::TVec3<f32> bNormal) {
            bNormal.set(_0x70[0], 0.0f, _0x70[1]);
            bNormal.normalize();
        }

        uint getShortcutID() const
        {
            return mCheckPoint->mShortcutID;
        }

        int getNextNum() const; /* {
             return mNextNum;
         }*/

        int getPrevNum() const
        {
            return mPrevNum;
        }

        Sector *getNextSector(int no); /*{
           bool valid = false;
           if(no >= 0 && no < 4)
               valid = true;

           if(!valid) {
               JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 105, "range over: %d <= no=%d < %d", 0, no, 4);
               OSPanic(__FILE__, 105, "Halt");
           }


           return mNextSectors[no];
       }*/

        Sector *getMainSector()
        {
            return mMainSector;
        }

        float getTotalPriorDist() const
        {
            return mTotalPriorDist;
        }

        float getSectorDist(void) const;

        bool isInvalid() const
        {
            return mCheckPoint->_0x19 != 0;
        }

    private:
        s16 mID;
        s16 mGeneration;
        bool bMainSector;
        bool bDivFlag;
        CrsArea::SCheckPoint *mCheckPoint;
        CrsArea::SCheckPoint *mCheckPoint2;
        JGeometry::TVec3<float> _0x10[4];
        JGeometry::TVec3<float> _0x40[4];
        float _0x70[2]; // this is TVec2<f32>
        float _0x78[2];
        float mSectorDist;
        s16 mPrevNum;
        s16 mNextNum;
        Sector *mPrevSectors[4];
        Sector *mNextSectors[4];
        Sector *mMainSector;
        float mTotalPriorDist;
    };

    Sector *getStartSector() const
    {
        return &mSectors[0];
    }

    float getTrackSectorDist() const
    {
        return mTrackSectorDist;
    }

    int getTotalSectorNumber() const
    {
        return mTotalSectors;
    }

    Sector *getSector(int no) const
    {
        bool valid = false;
        if (no >= 0 && no < mTotalSectors)
            valid = true;

        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 300, "range over: %d <= no=%d < %d", 0, no, mTotalSectors);
            OSPanic(__FILE__, 300, "Halt");
        }
        return &mSectors[no];
    }

    Sector *getMainSector(int gen) const
    {
        bool valid = false;
        if (gen >= 0 && gen < mTrackSectorNum)
            valid = true;
        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 306, "range over: %d <= gen=%d < %d", 0, gen, mTrackSectorNum);
            OSPanic(__FILE__, 306, "Halt");
        }
        return mMainSector[gen];
    }

    Sector *searchSector(short);

private:
    u8 _0[0x50c];
    s16 mTotalSectors;
    s16 mTrackSectorNum;
    Sector *mSectors;
    float mTrackSectorDist;
    Sector **mMainSector;
};

#endif