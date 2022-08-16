#ifndef ITEMOBJ_H
#define ITEMOBJ_H

#include "types.h"

class ItemObj {
public:

int getOwnerNum() const {
    return mOwnerNum;
}

int getKartReaction() const {
    return mKartReaction;
}

int getDirectHitKartNo() const {
    return mDirectHitKartNo;
}

private:
u8 _0[0x120];
int mOwnerNum;
u8 _0x124[0x140 - 0x124];
int mKartReaction; // probably an enum
u8 _0x144[0x28c - 0x144];
int mDirectHitKartNo;

};

#endif