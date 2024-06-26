#ifndef ITEMSTAR_H
#define ITEMSTAR_H

#include "Sato/ItemObj.h"

 // TODO

class ItemStar : public ItemObj
{ // Autogenerated
public:
    ItemStar(u32);                              // 0x80255aa0
    void reset();                               // 0x80255af0
    const char *getBmdFileName();               // 0x80255b7c
    void createModel(JKRSolidHeap *, u32, u32); // 0x80255ba4
    void doUniv();                              // 0x80255bdc
    void initEquip();                           // 0x80255c88
    void doEquip();                             // 0x80255d0c
    void initRelease();                         // 0x80255dd8
    void doRelease();                           // 0x80255dfc
    void initDisappear();                       // 0x80255e00
    void doDisappear();                         // 0x80255e18
    void initFall();                            // 0x80255e1c
    void doFall();                              // 0x80255e44
    // Inline
    virtual ~ItemStar() {} // 0x80255ec8

    u8 _[0x2e4 - 0x2b0];
};
#endif // ITEMSTAR_H