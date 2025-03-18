#ifndef SHIMMERSCREEN_H
#define SHIMMERSCREEN_H

#include <JSystem/JUtility/JUTTexture.h>
#include <JSystem/JUtility/JUTAssert.h>

#include "types.h"

#define NUM_LEVELS 2

class ShimmerScreen
{
public:
    // Global
    ShimmerScreen(ResTIMG *timg);       // 0x801d2ff8
    void calc();                        // 0x801d31e0
    void capture();                     // 0x801d3244
    void draw();                        // 0x801d32cc
    void drawInit();                    // 0x801d3324
    void drawEnd();                     // 0x801d34a4
    void levelDraw(int);                // 0x801d34e0
    static u8 sIndSinedOfs;             // 0x804147a0
    static s16 sIndScaleExp;            // 0x804147a2
    static GXIndTexScale sIndTexScaleS; // 0x80416570
    static GXIndTexScale sIndTexScaleT; // 0x80416574
    static u8 sIndReplace;              // 0x80416578
    // Inline/Unused
    // void drawForDebug();
    // void sTestR;
    // void sTestG;
    // void sTestB;
    // void sTestA;
    // void sIsDrawDebug;
    // Inline
    bool isIndActive() const { return mIndImg != nullptr; } // 0x801d3494

    void setOffsetZ(int level, s32 z)
    {
#line 47
        JUT_MINMAX_ASSERT(0, level, NUM_LEVELS)
        JUT_MINMAX_ASSERT(0, z, 0x1000)
        if (!_0)
            mOffsetZ[level] = z;
    }

    void setBlendRate(int level, u8 rate)
    {
#line 56
        JUT_MINMAX_ASSERT(0, level, NUM_LEVELS)
        JUT_MINMAX_ASSERT(0, rate, 0x100); // silly
        mBlendRate[level] = rate;
    }

private:
    bool _0;
    s32 mOffsetZ[2];
    u8 mBlendRate[2];
    ResTIMG *mIndImg;
    JUTTexture mTex1;
    JUTTexture mTex2;
    JUTTexture mTex3;
    u16 mStartX;
    u16 mStartY;
    u16 mEndX;
    u16 mEndY;
    bool mFlush;
    f32 _e0[2][3];
    Mtx mIndMtx;
    s16 _128;
    s16 _12a;
}; // class ShimmerScreen

#endif // SHIMMERSCREEN_H
