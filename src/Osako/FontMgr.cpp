#include "Osako/FontMgr.h"
#include "Osako/ResMgr.h"

namespace FontMgr {
    JUTResFont *mspResFont;
    JUTResFont *mspDebugFont;
    void create(JKRHeap *heap)
    {
        mspResFont = new (heap, 0) JUTResFont((const ResFONT *)ResMgr::getPtr(ResMgr::mcArcSystem, "Font/kartfont.bfn"), nullptr);
        mspDebugFont = new (heap, 0) JUTResFont(&JUTResFONT_Ascfont_fix12, nullptr);
    }
}


