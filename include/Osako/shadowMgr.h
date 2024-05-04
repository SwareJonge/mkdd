#ifndef SHADOWMGR_H
#define SHADOWMGR_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JGadget/linklist.h"
#include "Osako/shadowModel.h"
#include "types.h"

class ShadowManager {
public:
    ShadowManager();

    void getShadowModelList(ShadowModel::ShadowKind);
    void regist(ShadowModel *);
    //void draw(u32, JGadget::TList<ShadowModel *, JGadget::TAllocator<ShadowModel *>> &);
    void calc();
    void viewCalc(u32);
    void setDstAlpha();

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
        mDepth[1] = depth;
    }
    void setDepthMode(u8 depth)
    {
        mDepthMode = depth;
    }

    int getDstAlpha() {
        if (mDepthMode == 1) {
            return 0xff - (mDepth[1] & 0xfc);
        }
        return 0xff;
    }

    static ShadowManager* mspShadowManager;

//private:
    u8 _00[0x58];
    JUtility::TColor mShadowColor;
    u8 mDepth[2];
    u8 mDepthMode;
    u8 _5f;
    bool mIsMirror;
    u8 _61[3]; // i thought operator new always gets aligned by 4?
}; // Size: 0x64

#endif // SHADOWMGR_H