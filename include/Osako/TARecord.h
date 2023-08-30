#ifndef TARECORD_H
#define TARECORD_H

#include "Kaneshige/RaceTime.h"
#include "types.h"

struct TARecord {
    TARecord() {
        reset();
    }
    // fabricated
    void reset()
    {
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
                             ((mCharIDs[0] != 0) && mCharIDs[0] < 0x15) &&
                             ((mCharIDs[1] != 0) && mCharIDs[1] < 0x15)); }
    RaceTime getRaceTime() const { return mTime; }

    // perhaps these are signed
    u8 mCharIDs[2];
    u8 mKartID;
    u8 mCourseID;
    char mName[4];
    RaceTime mTime;
}; // size 0xC

#endif