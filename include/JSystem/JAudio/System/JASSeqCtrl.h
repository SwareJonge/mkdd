#ifndef JAUDIO_JASSEQCTRL_H
#define JAUDIO_JASSEQCTRL_H

#include "JSystem/JAudio/System/JASSeqReader.h"

#include "types.h"

class JASTrack;
class JASSeqParser;

class JASSeqCtrl
{
public:
    enum IntrType
    {
        INTRTYPE_VALUE_0,
        INTRTYPE_VALUE_1,
        INTRTYPE_VALUE_2,
        INTRTYPE_VALUE_3,
        INTRTYPE_VALUE_4,
        INTRTYPE_VALUE_5,
        INTRTYPE_VALUE_6,
    };

    JASSeqCtrl();
    void init();
    void start(void *, u32);
    int tickProc(JASTrack *);
    void interrupt(IntrType);
    void setIntrMask(u32);
    void clrIntrMask(u32);
    int retIntr();
    int findIntr();
    void checkIntr();
    void timerProcess();

    const JASSeqReader *getSeqReader() const { return &mReader; }
    void *getBase() { return mReader.getBase(); }
    void *getAddr(u32 param_0) { return mReader.getAddr(param_0); }
    u8 getByte(u32 param_0) { return mReader.getByte(param_0); }
    u16 get16(u32 param_0) { return mReader.get16(param_0); }
    u32 get24(int param_0) { return mReader.get24(param_0); }
    u32 get32(u32 param_0) { return mReader.get32(param_0); }
    void *getCur() { return mReader.getCur(); }
    u32 readByte() { return mReader.readByte(); }
    u32 read16() { return mReader.read16(); }
    u32 read24() { return mReader.read24(); }
    bool call(u32 param_0) { return mReader.call(param_0); }
    bool ret() { return mReader.ret(); }
    void jump(u32 param_0) { mReader.jump(param_0); }
    bool loopStart(u32 param_0) { return mReader.loopStart(param_0); }
    bool loopEnd() { return mReader.loopEnd(); }
    int readMidiValue() { return mReader.readMidiValue(); }
    void wait(s32 param_0) { _40 = param_0; }
    void clrIntr() { _44 = NULL; }
    void setIntrTable(u32 param_0) { _48 = param_0; }
    void setIntrTimer(u32 param_0, u32 param_1)
    {
        _50 = param_0;
        _54 = param_1;
        _58 = param_1;
    }
    void waitNoteFinish() { _51 = 1; }

    JASSeqReader mReader; // 00
    JASSeqParser *_3c;    // 3c
    s32 _40;              // 40
    void *_44;            // 44
    u32 _48;              // 48
    u16 _4c;              // 4c
    u16 _4e;              // 4e
    u8 _50;               // 50
    u8 _51;               // 51
    u16 _52;              // 52
    u32 _54;              // 54
    u32 _58;              // 58
    static JASSeqParser sDefaultParser;
};

#endif