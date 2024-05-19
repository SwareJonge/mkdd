#ifndef JAUDIO_JASREGISTERPARAM_H
#define JAUDIO_JASREGISTERPARAM_H

#include "types.h"

class JASRegisterParam
{
public:
    enum RegID
    {
        REG0,
        REG1,
        REG2,
        REG3,
    };

    JASRegisterParam();
    void init();
    void write(RegID, u32);
    u32 read(RegID);

    u16 _0[14];
};

#endif