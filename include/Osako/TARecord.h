#ifndef TARECORD_H
#define TARECORD_H

#include "Kaneshige/RaceTime.h"
#include "types.h"

struct TARecord {
    TARecord() {
        mKartID = 0xff;
        mTime.reset();
    }
    TARecord(u8, u8, u8, u8);
    void setName(const char *);
    void setStatus(u8, u8, u8, u8);
    void setRaceTime(RaceTime);
    bool less(const TARecord &);

    // probably Inlined(auto)
    void set(u8, u8, u8, u8, const char *, RaceTime);

    // Inline
    bool isValid() { return ((mKartID < 0x15) && 
    ((mCharID1 != 0) && mCharID1 < 0x15) && 
    ((mCharID2 != 0) && mCharID2 < 0x15)); }
    RaceTime getRaceTime() const { return mTime; }

    // perhaps these are signed
    u8 mCharID1;
    u8 mCharID2;
    u8 mKartID;
    u8 mCourseID;
    char mName[4];
    RaceTime mTime;
}; // size 0xC

#endif