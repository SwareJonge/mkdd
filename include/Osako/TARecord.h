#ifndef TARECORD_H
#define TARECORD_H

#include "Kaneshige/RaceTime.h"
#include "types.h"
#include "kartEnums.h"

class TARecord
{
public:
    TARecord() : mKartID(0xff) { }
    // fabricated
    void reset()
    {
        mKartID = 0xff;
        mTime.reset();
    }

    TARecord(u8 charId1, u8 charId2, u8 kartId, u8 courseId);
    void setName(const char *name);
    void setStatus(u8 charId1, u8 charId2, u8 kartId, u8 courseId);
    void setRaceTime(RaceTime time);
    bool less(const TARecord &rec);

    // probably Inlined(auto)
    void set(u8 charId1, u8 charId2, u8 kartId, u8 courseId, const char *name, RaceTime time);

    // Inline
    bool isValid() { return ((mKartID < 0x15) &&
                             ((mCharIDs[0] != 0) && mCharIDs[0] < 0x15) &&
                             ((mCharIDs[1] != 0) && mCharIDs[1] < 0x15)); }
    RaceTime getRaceTime() const { return mTime; }
    ECourseID getCourseID() const { return (ECourseID)mCourseID; }
    EKartID getKartID() const { return (EKartID)mKartID; }
    ECharID getCharID(int no) const { return (ECharID)mCharIDs[no]; }

private:
    u8 mCharIDs[2]; // 00
    u8 mKartID;     // 02
    u8 mCourseID;   // 03
    char mName[4];  // 04
    RaceTime mTime; // 08
};

#endif