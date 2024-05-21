#ifndef JAUDIO_JASSEQREADER_H
#define JAUDIO_JASSEQREADER_H

#include "types.h"

class JASSeqReader
{
public:
    void init();
    void init(void *base);
    bool call(u32 offset);
    bool loopStart(u32);
    bool loopEnd();
    bool ret();
    int readMidiValue();

    void jump(u32 offset)
    {
        mReadPtr = (u8 *)mBase + offset;
    }

    void jump(void *newAddr)
    {
        mReadPtr = (u8 *)newAddr;
    }

    u32 get24(int param_0)
    {
        return (*(u32 *)((int)mBase + param_0 - 1)) & 0xffffff;
    }

    u32 *getBase() { return mBase; }
    void *getAddr(u32 offset) { return (u8 *)mBase + offset; }
    u8 getByte(u32 offset) { return *((u8 *)mBase + offset); }
    u16 get16(u32 offset) { return *(u16 *)((u8 *)mBase + offset); }
    u32 get32(u32 offset) { return *(u32 *)((u8 *)mBase + offset); }
    u8 *getCur() { return mReadPtr; }
    u32 readByte() { return *mReadPtr++; }
    u32 read16()
    {
        u16 *tmp = (u16 *)mReadPtr;
        mReadPtr += 2;
        return *tmp;
    }
    u32 read24()
    {
        mReadPtr--;
        u32 *tmp = (u32 *)mReadPtr;
        mReadPtr += 4;
        return *tmp & 0x00ffffff;
    }
    u16 getLoopCount() const { return _08 == 0 ? 0 : _2c[_08 - 1]; }

    u32 *mBase;   // 00
    u8 *mReadPtr; // 04
    u32 _08;      // 08
    u16 *_0c[8];  // 0c
    u16 _2c[8];   // 2c
};

#endif