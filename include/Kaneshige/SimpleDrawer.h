#ifndef SIMPLEDRAWER_H
#define SIMPLEDRAWER_H

#include "Kaneshige/ExModel.h"

class SimpleDrawer
{
public:
    SimpleDrawer();                         // 0x801ba064
    void drawInit(ExModel *model);          // 0x801ba07c
    void drawInit(J3DModelData *modelData); // 0x801ba14c
    bool loadPreDrawSetting();              // 0x801ba1b4

    ~SimpleDrawer() {}
    J3DMaterial *getDrawingMaterial() { return mDrawingMaterial; }
    void enableDrawMaterial() { mIsMatDrawOn = true; }
    bool isPhaseCheckHidding() { return mPhase == 0; }

    static SimpleDrawer *getDrawingDrawer() { return sDrawingDrawer; }

    static bool sEnableCheckHidding;     // 0x80414650
    static SimpleDrawer *sDrawingDrawer; // 0x804163e8

    u32 mPhase;                    // 0x0
    bool mKeepDrawMat;             // 0x4
    J3DMaterial *mDrawingMaterial; // 0x8
    bool mIsMatDrawOn;             // 0xc
};

#endif // SIMPLEDRAWER_H
