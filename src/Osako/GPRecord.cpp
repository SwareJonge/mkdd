#include "Dolphin/string.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Osako/GPRecord.h"

GPRecord::GPRecord() {
    _0 = 0;
    _1 = 0;
    _4 = 0;
    _6 = 0;
}

void GPRecord::setName(const char * name) {
    strncpy(mName, name, sizeof(mName));
}

void GPRecord::set(u8 p1, u8 p2, u8 p3, u8 level, u8 p5, u8 p6, bool p7, const char * name, RaceTime raceTime) {
    _0 = p1;
    _1 = p2;
    _2 = p3;
    mLevel = level;
    _4 = p5;
    _5 = p6;
    if(p7)
        _6 |= 2;
    else
        _6 &= -3;

    setName(name);
    mTime = raceTime;
    _6 |= 1;
}

bool GPRecord::less(const GPRecord &rRec) {
    bool ret = false;
    if ((_6 & 1) == 0)
        ret = true;
    else {
        JUT_VALIDATE(82, rRec.mLevel == mLevel);
        if (rRec._4 < _4)
            ret = true;
        else { // can this be cleaned up somehow?
            if (rRec._4 == _4) {
                if (rRec._5 > _5)
                    return true;
                if ((rRec._5 == _5) && (rRec.mTime.isLittle(mTime)))
                    return true;
            }
            ret = false;
        }
    }
    return ret;
}