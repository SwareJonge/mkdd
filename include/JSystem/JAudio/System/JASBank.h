#ifndef JASBANK_H
#define JASBANK_H

#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JKernel/JKRHeap.h"

class JASInstParam;
class JASWaveBank;

class JASBank
{
public:
    JASBank() { mWaveBank = NULL; }
    virtual ~JASBank(){};
    virtual JASInst *getInst(int) const = 0;
    virtual u32 getType() const = 0;

    JASWaveBank *getWaveBank() const { return mWaveBank; }
    void assignWaveBank(JASWaveBank *waveBank) { mWaveBank = waveBank; }

    static JASChannel *noteOn(const JASBank *pBank, int, u8, u8, u16, JASChannel::Callback channelCb, void *);
    static JASChannel *noteOnOsc(int, u8, u8, u16, JASChannel::Callback channelCb, void *);

private:
    JASWaveBank *mWaveBank; // 04
};

class JASBasicBank : public JASBank
{
public:
    JASBasicBank();
    void newInstTable(u8, JKRHeap *);
    bool getInstParam(int, int, int, JASInstParam *) const;
    void setInst(int prg_no, JASInst *inst);
    virtual JASInst *getInst(int) const;
    virtual ~JASBasicBank();
    virtual u32 getType() const;

private:
    u8 mInstTable[0x400]; // 08
    //JASInst **mInstTable; // 08
    //u8 mInstNumMax;       // 0c
};

#endif