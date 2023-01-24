#ifndef PASSWORD_H
#define PASSWORD_H

#include "types.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Kaneshige/Course/Course.h"

class Password {
    public:
    Password() {
        mMake = false;
    }
    u8 convCrsNo(ECourseID);
    // TODO: Make header with just the enums
    //bool make();

    char getPassAtIndex(int n) {
        JUT_ASSERT(48, mMake);
        JUT_MINMAX_ASSERT(49, 0, n, 16);
        return mPass[n];
    }

    private:
    u8 _0 [0xC]; // might be an array of unions, since sometimes a member is u16 and sometimes u8
    char mPass[17]; // last byte is a hardcoded terminator
    bool mMake;
};

#endif