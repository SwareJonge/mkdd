#ifndef SHADOWMGR_H
#define SHADOWMGR_H

#include "JSystem/JUtility/TColor.h"
#include "types.h"

class ShadowManager {
public:
    ShadowManager();
    void calc();
    void viewCalc(u32);

    static ShadowManager *ptr() {
        if(!mspShadowManager)
            mspShadowManager = new ShadowManager();
        return mspShadowManager;
    }
    void setMirror(bool mirror) {
        mIsMirror = mirror;
    }
    void setShadowColor(JUtility::TColor & color) {
        mShadowColor = color;
    }
    void setShadowDepth(u8 depth) {
        mShadowDepth = depth;
    }
    static ShadowManager* mspShadowManager;

private:
    u8 _00[0x58];
    JUtility::TColor mShadowColor;
    u8 _5c;
    u8 mShadowDepth;
    u8 _5e;
    u8 _5f;
    bool mIsMirror;
    u8 _61[3]; // i thought operator new always gets aligned by 4?
}; // Size: 0x64

#endif // SHADOWMGR_H