#ifndef TARECORD_H
#define TARECORD_H

#include "Kaneshige/RaceTime.h"
#include "types.h"
#include "kartEnums.h"

class TARecord
{
public:
    TARecord() { reset(); }
    // fabricated
    void reset()
    {
        mKartID = 0xff;
        mTime.reset();
    }

    // i also have trouble believing this is right
    TARecord(u8 kartID) : mKartID(kartID) {}

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
    ECourseID getCourseID() const { return (ECourseID)mCourseID; }

private:
    // perhaps these are signed
    u8 mCharIDs[2];
    u8 mKartID;
    u8 mCourseID;
    char mName[4];
    RaceTime mTime;
}; // size 0xC

#endif