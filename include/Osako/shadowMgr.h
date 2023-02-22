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
    void setMirror(bool) {

    }
    void setShadowColor(JUtility::TColor &) {

    }
    void setShadowDepth(u8) {

    }
    static ShadowManager* mspShadowManager;

private:
    u8 _00[100];
};

#endif // SHADOWMGR_H