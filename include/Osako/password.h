#ifndef PASSWORD_H
#define PASSWORD_H

#include "types.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/RaceInfo.h"

struct PWUnion
{
    // 0x0
    u32 totalTime : 19;
    // 0x2
    u32 unk1 : 1;
    u32 region : 2;
    u32 back : 5;
    u32 checkData : 5;
    // 0x4
    u32 flapTime : 18;
    // 0x6
    u16 kart : 5;
    u16 driver : 5;
    u16 course : 4;
    u16 rnd;
};

union PW
{
    PWUnion bits;
    u8 packed[0xa];
};

class Password {
public:
    Password() { mMake = false; }
    u8 convCrsNo(ECourseID);
    // TODO: Make header with just the enums

    u8 calcCheckData();
    void setCheckData();

    void crypt(u16);
    void decode();
    void encode();

    bool pack(EKartID kartId, ECharID driverId, ECharID backId, ECourseID crsID, RaceTime total, RaceTime flap);
    bool unpack(EKartID *kartId, ECharID *driverId, ECharID *backId, ECourseID *crsID, RaceTime *total, RaceTime *flap);
    bool make(EKartID kartId, ECharID driverId, ECharID backId, ECourseID crsID, RaceTime totalTime, RaceTime flapTime);

    char searchPasswordTable(char c);

    char getPassAtIndex(int n) {
#line 48
        JUT_ASSERT(mMake);
        JUT_MINMAX_ASSERT(0, n, 16);
        return mPass[n];
    }

private:
    PW mPWTable;
    char mPass[17]; // last byte is a hardcoded terminator
    bool mMake;
};

#endif