#ifndef JASOSCILLATOR_H
#define JASOSCILLATOR_H

#include "dolphin/types.h"

struct JASOscillator
{
    struct Point
    {
        s16 _0;
        s16 _2;
        s16 _4;
    };

    struct EffectParams
    {
        EffectParams()
        {
            mVolume = 1.0f;
            mPitch = 1.0f;
            mPan = 0.5f;
            mFxMix = 0.0f;
            mDolby = 0.0f;
            _14 = 1.0f;
            _18 = 1.0f;
        }

        f32 mVolume; // 00
        f32 mPitch;  // 04
        f32 mPan;    // 08
        f32 mFxMix;  // 0c
        f32 mDolby;  // 10
        f32 _14;     // 14
        f32 _18;     // 18
    };

    struct Data
    {
        u32 mTarget;          // 00
        f32 _04;              // 04
        const Point *mTable;  // 08
        const Point *mTable2; // 0c
        f32 mScale;           // 10
        f32 _14;              // 14
    };

    enum Target
    {
        TARGET_VOLUME, // 0
        TARGET_PITCH,  // 1
        TARGET_PAN,    // 2
        TARGET_FXMIX,  // 3
        TARGET_DOLBY,  // 4
        TARGET_5,      // 5
        TARGET_6,      // 6
    };

    JASOscillator();

    void initStart(const Data *);
    void incCounter(f32 param_0);
    f32 getValue() const;
    void release();
    void update();
    void updateCurrentValue(f32 param_0);

    void setDirectRelease(u16 release) { mDirectRelease = release; }
    void stop() { _1C = 0; }
    bool isValid() { return mData != NULL; }
    bool isStop() { return _1C == 0; }
    bool isRelease() { return _1C == 3 || _1C == 4; }
    u32 getTarget() { return mData->mTarget; }

    const Data *mData;  // 00
    f32 _04;            // 04
    f32 _08;            // 08
    f32 _0C;            // 0c
    f32 _10;            // 10
    u16 _14;            // 14
    u16 mDirectRelease; // 16
    u8 _18;             // 18
    u16 _1A;            // 1a
    int _1C;            // 1c

    static const f32 sCurveTableLinear[17];
    static const f32 sCurveTableSampleCell[17];
    static const f32 sCurveTableSqRoot[17];
    static const f32 sCurveTableSquare[17];
};

#endif /* JASOSCILLATOR_H */