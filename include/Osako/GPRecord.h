#ifndef GPRECORD_H
#define GPRECORD_H

#include "Kaneshige/RaceTime.h"
#include "kartEnums.h"
#include "types.h"

struct GPRecord
{
    GPRecord();
    void setName(const char *name);
    void set(u8 charId1, u8 charId2, u8 kartId, u8 level, u8 rank, u8 pts, bool coop, const char *name, RaceTime raceTime);
    bool less(const GPRecord &rRec);

    // fabricated
    void reset()
    {
        mFlags &= ~0x1;
        mTotalTime.reset();
    }

    bool isValid() const { // fabricated
        return (mFlags & 1) && mKartID < cKartIDMax && mCharIDs[0] > cCharIDNone && mCharIDs[0] < cCharIDMax && mCharIDs[1] > cCharIDNone && mCharIDs[1] < cCharIDMax;
    }

    u8 mCharIDs[2];      // 00
    u8 mKartID;          // 02
    u8 mLevel;           // 03
    u8 mRank;            // 04
    u8 mPoints;          // 05
    u8 mFlags;           // 06: binary flags, 1 = cleared, 2 = co-op
    u8 _7;               // set to 0x9c with debug menu?
    char mName[4];       // 08
    RaceTime mTotalTime; // 0C
};

#endif
