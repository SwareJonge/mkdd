#ifndef SHADOWMGR_H
#define SHADOWMGR_H

#include "JSystem/JUtility/TColor.h"
#include "types.h"

class ShadowManager {
public:
    inline static ShadowManager *ptr();
    inline void setMirror(bool);
    inline void setShadowColor(JUtility::TColor &);
    inline void setShadowDepth(u8);
};

#endif // SHADOWMGR_H