#ifndef JAUDIO_JASWAVEINFO_H
#define JAUDIO_JASWAVEINFO_H

#include "types.h"

struct JASWaveInfo
{
    JASWaveInfo()
    {
        _01 = 0x3c;
        _20 = &one;
    }

    u8 _00;         // 00
    u8 _01;         // 01
    u8 _02;         // 02
    f32 _04;        // 04
    int _08;        // 08
    int _0c;        // 0c
    u32 _10;        // 10
    int _14;        // 14
    int _18;        // 18
    s16 _1c;        // 1c
    s16 _1e;        // 1e
    const u32 *_20; // 20

    static u32 one;
};

#endif