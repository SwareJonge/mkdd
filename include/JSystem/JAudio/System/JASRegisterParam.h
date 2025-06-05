#ifndef JAUDIO_JASREGISTERPARAM_H
#define JAUDIO_JASREGISTERPARAM_H

#include "types.h"

class JASRegisterParam
{
public:
    enum RegID
    {
        REG3 = 3,
        REGID32 = 32,
        REGID33,
        REGID34,
        REGID35,
        REGID36,
        REGID37,
        REGID38,
        REGID39,
        REGID40,
        REGID41,

        NORMAL_REGID_MAX = 14,
    };

    JASRegisterParam();
    void init();
    void write(RegID, u32);
    u32 read(RegID);

    u16 mRegs[NORMAL_REGID_MAX];
};

#endif
