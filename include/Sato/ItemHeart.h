#ifndef ITEMHEART_H
#define ITEMHEART_H

#include "Sato/ItemObj.h"

class J3DEfctHeart;

class ItemHeart : public ItemObj
{
public:
    ItemHeart(u32);                             // 0x8028717c

    int setHitItem(const ItemObj &);             // 0x80287344
    void initMoveEffect(ItemObj *);             // 0x802874f0

    // Vtable overrides
    virtual ~ItemHeart() {}                             // 08
    virtual void reset();                               // 10
    virtual const char *getBmdFileName();               // 28
    virtual void createModel(JKRSolidHeap *, u32, u32); // 40
    virtual void doUniv();                              // 50
    virtual void initEquip();                           // 54
    virtual void initRelease();                         // 58
    virtual void initSpRelease();                       // 5C
    virtual void initDisappear();                       // 60
    virtual void initFall();                            // 64
    virtual void doEquip();                             // 78
    virtual void doRelease();                           // 7C
    virtual void doDisappear();                         // 84

    // Inlines
    bool hasHeartsRemaining() const { return !mNoHeartsRemaining; }

    f32 mRot;
    Mtx mRotMtx;
    s16 mRemainingNum; // 2e4
    s16 mLostNum; // 2e6;
    J3DEfctHeart *mEffects[2]; // 2e8
    s16 mEffectIDs[2]; 

    bool mNoHeartsRemaining; // 2f4

}; // 2f8
#endif // ITEMHEART_H