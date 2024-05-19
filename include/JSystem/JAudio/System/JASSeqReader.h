#ifndef JAUDIO_JASSEQREADER_H
#define JAUDIO_JASSEQREADER_H

#include "types.h"

class JASSeqReader
{
public:
    void init();
    void init(void *);
    bool call(u32);
    bool loopStart(u32);
    bool loopEnd();
    bool ret();
    int readMidiValue();

    void jump(u32 param_1)
    {
        _04 = (u8 *)_00 + param_1;
    }

    void jump(void *param_1)
    {
        _04 = (u8 *)param_1;
    }

    u32 get24(int param_0)
    {
        return (*(u32 *)((int)_00 + param_0 - 1)) & 0xffffff;
    }

    u32 *getBase() { return _00; }
    void *getAddr(u32 param_0) { return (u8 *)_00 + param_0; }
    u8 getByte(u32 param_0) { return *((u8 *)_00 + param_0); }
    u16 get16(u32 param_0) { return *(u16 *)((u8 *)_00 + param_0); }
    u32 get32(u32 param_0) { return *(u32 *)((u8 *)_00 + param_0); }
    u8 *getCur() { return _04; }
    u32 readByte() { return *_04++; }
    u32 read16()
    {
        u16 *tmp = (u16 *)_04;
        _04 += 2;
        return *tmp;
    }
    u32 read24()
    {
        _04--;
        u32 *tmp = (u32 *)_04;
        _04 += 4;
        return *tmp & 0x00ffffff;
    }
    u16 getLoopCount() const { return _08 == 0 ? 0 : _2c[_08 - 1]; }

    u32 *_00;    // 00
    u8 *_04;     // 04
    u32 _08;     // 08
    u16 *_0c[8]; // 0c
    u16 _2c[8];  // 2c
};

#endif