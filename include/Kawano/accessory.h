#ifndef ACCESSORY_H
#define ACCESSORY_H

class AccessoryMgr {
    public:
    void update();

    static AccessoryMgr *ptr() { return mspAccessoryMgr; }
    static AccessoryMgr *mspAccessoryMgr;
};

#endif