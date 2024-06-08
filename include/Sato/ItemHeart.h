#ifndef ITEMHEART_H
#define ITEMHEART_H

#include "Sato/ItemObj.h"

class ItemHeart : public ItemObj
{
public:
    ItemHeart(u32);                             // 0x8028717c
    void reset();                               // 0x802871cc
    const char *getBmdFileName();               // 0x80287288
    void createModel(JKRSolidHeap *, u32, u32); // 0x80287294
    void doUniv();                              // 0x80287298
    void setHitItem(const ItemObj &);           // 0x80287344
    void initMoveEffect(ItemObj *);             // 0x802874f0
    void initEquip();                           // 0x802875bc
    void doEquip();                             // 0x80287640
    void initRelease();                         // 0x8028770c
    void initSpRelease();                       // 0x802877c4
    void doRelease();                           // 0x802877e8
    void initDisappear();                       // 0x80287818
    void doDisappear();                         // 0x802878b0
    void initFall();                            // 0x802878b4
    // Inline
    virtual ~ItemHeart() {} // 0x802878d4

    bool hasHeartsRemaining() const { return !mNoHeartsRemaining; }

    u8 _2b4[0x2f4 - 0x2b0];
    bool mNoHeartsRemaining;

}; // 2f8
#endif // ITEMHEART_H