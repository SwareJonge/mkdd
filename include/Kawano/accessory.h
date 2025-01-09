#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "types.h"

class AccessoryMgr {
public:
    void update();
    void setCurrentViewNo(u32 viewNo);
    void simpleDraw(u32 viewNo);

    static AccessoryMgr *ptr();
    static AccessoryMgr *mspAccessoryMgr;
};

#endif
