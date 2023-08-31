#ifndef GPRECORD_H
#define GPRECORD_H

#include "Kaneshige/RaceTime.h"
#include "types.h"

struct GPRecord {
    GPRecord();
    void setName(const char *);
    void set(u8, u8, u8, u8, u8, u8, bool, const char *, RaceTime);
    bool less(const GPRecord & rRec);

    // fabricated
    void reset()
    {
        _6 &= ~0x1;
        mTime.reset();
    }

    // series of bytes probably consist of IDs
    u8 _mCharIDs[2]; 
    u8 _2;
    u8 mLevel;
    u8 _4;
    u8 _5;
    u8 _6; // some type of flag
    u8 _7; // probably padding
    char mName[4];
    RaceTime mTime;
}; // Size 0x10


#endif