#include <string.h>
#include "JSystem/JUtility/JUTDbg.h"
#include "Osako/GPRecord.h"

#define GP_CLEARED 1
#define GP_COOP 2

GPRecord::GPRecord() {
    mCharIDs[0] = 0;
    mCharIDs[1] = 0;
    mRank = 0;
    mFlags = 0;
}

void GPRecord::setName(const char * name) {
    strncpy(mName, name, sizeof(mName));
}

void GPRecord::set(u8 charId1, u8 charId2, u8 kartId, u8 level, u8 rank, u8 pts, bool coop, const char *name, RaceTime raceTime)
{
    mCharIDs[0] = charId1;
    mCharIDs[1] = charId2;
    mKartID = kartId;
    mLevel = level;
    mRank = rank;
    mPoints = pts;

    if(coop)
        mFlags |= GP_COOP;
    else
        mFlags &= ~GP_COOP;

    setName(name);
    mTotalTime = raceTime;
    mFlags |= GP_CLEARED;
}

bool GPRecord::less(const GPRecord &rRec) {
    if (FLAG_ON(mFlags, GP_CLEARED))
        return true;

#line 82
    JUT_ASSERT(rRec.mLevel == mLevel);
    if (rRec.mRank < mRank)
        return true;

    if (rRec.mRank == mRank)
    {
        if (rRec.mPoints > mPoints)
            return true;
        if ((rRec.mPoints == mPoints) && (rRec.mTotalTime.isLittle(mTotalTime)))
            return true;
    }
    return false;
}