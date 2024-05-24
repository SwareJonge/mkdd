#include "JSystem/JAudio/System/JASSeqReader.h"
#include "JSystem/JUtility/JUTAssert.h"

void JASSeqReader::init()
{
    mBase = NULL;
    mReadPtr = NULL;
    mCommandNo = 0;
    for (int i = 0; i < 8; i++)
    {
        mCommandPtr[i] = NULL;
        mLoopCount[i] = 0;
    }
}

void JASSeqReader::init(void *base)
{
    mBase = base;
    mReadPtr = (u8*)mBase;
    mCommandNo = 0;
    for (int i = 0; i < 8; i++)
    {
        mCommandPtr[i] = NULL;
        mLoopCount[i] = 0;
    }
}

bool JASSeqReader::call(u32 offset)
{
    if (mCommandNo >= 8)
    {
#line 42
        JUT_WARNING_F2("%s", "Cannot exec call command");
        return false;
    }

    mCommandPtr[mCommandNo++] = mReadPtr;
    mReadPtr = ((u8 *)mBase + offset);
    return true;
}

bool JASSeqReader::loopStart(u32 loopCount)
{
    if (mCommandNo >= 8)
    {
#line 53
        JUT_WARNING_F2("%s", "Cannot exec loopStart command");
        return false;
    }

    mCommandPtr[mCommandNo] = mReadPtr;
    mLoopCount[mCommandNo++] = loopCount;
    return true;
}

bool JASSeqReader::loopEnd()
{
    if (mCommandNo == 0)
    {
#line 65
        JUT_WARNING_F2("%s", "cannot loopE for call-stack is NULL");
        return false;
    }

    u16 loopCount = mLoopCount[mCommandNo - 1];
    if (loopCount != 0)
        loopCount--;

    if (loopCount == 0)
    {
        mCommandNo--;
        return true;
    }

    mLoopCount[mCommandNo - 1] = loopCount;
    mReadPtr = (u8 *)mCommandPtr[mCommandNo - 1];
    return true;
}

bool JASSeqReader::ret()
{
    if (mCommandNo == 0)
        return false;

    mReadPtr = (u8 *)mCommandPtr[--mCommandNo];

    return true;
}

u32 JASSeqReader::readMidiValue()
{
    u32 byte = readByte();

    if (!(byte & 0x80))
        return byte;

    byte &= 0x7f;

    for (int i = 0; ; i++)
    {
        if (i > 2)
        {
#line 100
            JUT_WARNING_F2("%s", "readMidiValue: Too large value");
            return 0;
        }

        u8 newByte = readByte();
        byte = byte << 7;
        byte |= newByte & 0x7f;

        if (!(newByte & 0x80))
            break;
    }

    return byte;
}