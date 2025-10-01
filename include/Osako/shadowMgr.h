#ifndef SHADOWMGR_H
#define SHADOWMGR_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JGadget/std-list.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "Osako/shadowModel.h"
#include "types.h"

class ShadowManager : public JKRDisposer 
{
public:
    ShadowManager();

    typedef JGadget::TList<ShadowModel*> List;

    List &getShadowModelList(ShadowModel::ShadowKind);
    void regist(ShadowModel *);
    void draw(u32, List &modelList);
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
    void setShadowColor(JUTColor & color) {
        mShadowColor = color;
    }
    void setShadowDepth(u8 depth) {
        mDepth[1] = depth;
    }
    void setDepthMode(u8 depth)
    {
        mDepthMode = depth;
    }

    void drawShadowVolume(u32 viewNo, ShadowModel::ShadowKind shadowKind) {
        draw(viewNo, getShadowModelList(shadowKind));
    }

    int getDstAlpha() {
        if (mDepthMode == 1) {
            return 0xff - (mDepth[1] & 0xfc);
        }
        return 0xff;
    }

    static ShadowManager* mspShadowManager;

private:
    List _18;
    List _28;
    List _38;
    List _48;
    
    JUTColor mShadowColor;
    unsigned char mDepth[2];
    unsigned char mDepthMode;
    unsigned char _5f;
    unsigned char mIsMirror;
}; // Size: 0x64

#endif // SHADOWMGR_H
