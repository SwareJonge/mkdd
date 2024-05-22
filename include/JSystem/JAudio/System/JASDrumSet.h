#ifndef JAUDIO_JASDRUMSET_H
#define JAUDIO_JASDRUMSET_H

#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JKernel/JKRHeap.h"

struct JASDrumSet : public JASInst
{
    class TPerc
    {
    public:
        TPerc();
        void setVolume(f32 vol) { mVolume = vol; }
        void setPitch(f32 pitch) { mPitch = pitch; }
        void setPan(f32 pan) { mPan = pan; }

        void setEffect(int index, JASInstEffect *effect);
        void setRelease(u32 release);
        JASVelo *getVeloRegion(int index);
        void setVeloRegionCount(u32 count, JKRHeap *heap);

    private:
        f32 mVolume;  // 00
        f32 mPitch;   // 04
        f32 mPan;     // 08
        u16 mRelease; // 0c
    public:
        u16 _e;              // 0e
        f32 _10;             // 10
        f32 _14;             // 14
        u8 _18[0x28 - 0x18]; // 18
    };

    JASDrumSet();
    virtual ~JASDrumSet();
    void newPercArray(u8 num, JKRHeap *heap);
    virtual bool getParam(int, int, JASInstParam *) const;
    void setPerc(int index, JASDrumSet::TPerc *perc);
    virtual u32 getType() const;

    u8 mBuffer[0x200];
    // TPerc **mPercArray;
    // u8 mPercNumMax;
};

#endif