#ifndef SHADOWMGR_H
#define SHADOWMGR_H

#include "JSystem/JUtility/TColor.h"
#include "types.h"

class ShadowManager {
public:
    static ShadowManager *ptr();
    void setMirror(bool);
    void setShadowColor(JUtility::TColor &);
    void setShadowDepth(u8);
};

#endif // SHADOWMGR_H