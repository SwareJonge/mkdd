#ifndef ITEMOBJMGR_H
#define ITEMOBJMGR_H

#include <types.h>

class ItemObjMgr {
public:
    ItemObjMgr();
    virtual ~ItemObjMgr();
    virtual void draw();
    virtual void callbackColGeoObj();

    static bool sTempSlotUseItem[18];
private:
    u8 _0x4[0x828 - 0x4];
};

#endif // !ITEMOBJMGR_H
