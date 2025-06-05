#ifndef JAUDIO_JASINST_H
#define JAUDIO_JASINST_H

#include "JSystem/JMath/JMath.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JAudio/System/JASSoundParams.h"
#include "JSystem/JAudio/System/JASOscillator.h"

enum JASInstTarget
{
    JASINST_Volume = 0,
    JASINST_Pitch = 1,
    JASINST_Pan = 2,
    JASINST_FxMix = 3,
    JASINST_Dolby = 4,
};

// fabricated
struct JASVelo
{
    int _0;
    u32 _4;
    f32 _8;
    f32 _c;
};

class JASInstParam : public JASSoundParams
{
public:
    JASInstParam() : JASSoundParams()
    {
        _14 = 0;
        _18 = 0;
        _1c = NULL;
        _20 = 0;
        _24 = 0;
        _26 = 0;
    }

    u32 _14;
    u32 _18;
    JASOscillator::Data **_1c;
    u32 _20;
    u8 _24;
    u16 _26;
};

class JASInst
{
public:
    JASInst() {}
    virtual ~JASInst() {}
    virtual bool getParam(int, int, JASInstParam *) const = 0;
    virtual u32 getType() const = 0;
};

// NOTE: none of these constructors have been tested
class JASInstEffect
{
public:
    JASInstEffect()  {}
    virtual void effect(int, int, JASSoundParams *params) const = 0;

    void setType(int type) { mType = type; } // fabricated

protected:
    int mType;
};

class JASBasicInst : public JASInst
{
public:
    static const int OSC_MAX = 2;
    static const int EFFECT_MAX = 4;

    class TKeymap
    {
    public:
        TKeymap() {
            mHighKey = -1;
            mVeloRegionCount = 0;
            mVelomap = NULL;
        }
        ~TKeymap();                                        // 0x800975f4
        void setVeloRegionCount(u32 count, JKRHeap *heap); // 0x80097648
        JASVelo *getVeloRegion(int index);                 // 0x800976f0
        JASVelo *getVeloRegion(int index) const;           // 0x80097784

        void setHighKey(int high) {
            mHighKey = high;
        }

        int mHighKey;         // 0
        u32 mVeloRegionCount; // 4
        JASVelo *mVelomap;    // 8
    };

    // Global
    JASBasicInst();                                             // 0x80096ffc
    virtual ~JASBasicInst();                                    // 0x800970b8
    virtual bool getParam(int, int, JASInstParam *param) const; // 0x80097184
    void setKeyRegionCount(u32, JKRHeap *heap);                 // 0x800972dc
    void setEffect(int index, JASInstEffect *effect);           // 0x800973c8
    void setOsc(int index, JASOscillator::Data *oscData);       // 0x80097494
    TKeymap *getKeyRegion(int index);                           // 0x80097560

    virtual u32 getType() const { return 'BSIC'; }

    void setVolume(f32 vol) { mVolume = vol; }
    void setPitch(f32 pitch) { mPitch = pitch; }

    // Inline/Unused
    void searchKeymap(int) const;
    void getEffect(int);
    void getOsc(int);
    TKeymap *getKeyRegion(int) const;
private:
    f32 mVolume;                        // 04
    f32 mPitch;                         // 08
    JASOscillator::Data *mOsc[OSC_MAX]; // 0C
    JASInstEffect *mEffect[EFFECT_MAX]; // 14
    TKeymap *mKeymap;                   // 24
    u32 mKeyRegionCount;                // 28
};

class JASInstRand : public JASInstEffect
{
public:
    JASInstRand() : JASInstEffect()
    {
        mType = 0;
        mFloor = 1.0f;
        mCeiling = 0.0f;
    }

    virtual void effect(int, int, JASSoundParams *params) const;

    // Fabricated
    f32 getY(f32 in, f32 rnd, f32 mult) const
    {
        return rnd - (mult * in);
    }

    // fabricated
    void setFloor(f32 floor) { mFloor = floor; }
    void setCeiling(f32 ceiling) { mCeiling = ceiling; }
    void set(f32 floor, f32 ceiling)
    {
        mFloor = floor;
        mCeiling = ceiling;
    }

private:
    f32 mFloor;
    f32 mCeiling;
};

class JASInstSense : public JASInstEffect
{
public:
    JASInstSense() : JASInstEffect()
    {
        mType = 0;
        mTrigger = 0;
        mCenterKey = 60;
        _0C = 1.0f;
        _10 = 1.0f;
    }
    void setParams(int trigger, int centerkey, f32, f32);

    virtual void effect(int, int, JASSoundParams *params) const;

private:
    u8 mTrigger;
    u8 mCenterKey;
    f32 _0C;
    f32 _10;
};

#endif
