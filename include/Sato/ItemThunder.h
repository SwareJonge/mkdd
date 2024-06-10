#ifndef ITEMTHUNDER_H
#define ITEMTHUNDER_H

#include "Sato/GeographyObj.h"
#include "Sato/ItemObj.h"

// TODO

class ItemThunder : public ItemObj
{ // Autogenerated
public:
    ItemThunder(unsigned long);                                     // 0x80278588
    void entrySuperVisor(GeoObjSupervisor *);                       // 0x80278744
    void reset();                                                   // 0x80278774
    const char *getBmdFileName();                                   // 0x80278840
    void createModel(JKRSolidHeap *, unsigned long, unsigned long); // 0x80278868
    void doUniv();                                                  // 0x8027889c
    void initEquip();                                               // 0x8027896c
    void doEquip();                                                 // 0x802789c0
    void initRelease();                                             // 0x802789e8
    void doRelease();                                               // 0x80278a1c
    void initDisappear();                                           // 0x80278a20
    void doDisappear();                                             // 0x80278b0c
    void initFall();                                                // 0x80278b5c
    void doFall();                                                  // 0x80278b84
    void initForceDisappear();                                      // 0x80278c08
    void checkKartColDisappear(int);                                // 0x80278c3c
    void setEffectThunder(int);                                     // 0x80278c50
    void deleteEffectThunder();                                     // 0x80278d14
    void doEffectThunder();                                         // 0x80278da4

    static Vec sHandOffsetPos;                // 0x804137d8
    static GeoObjSupervisor *sSuperVisorList; // 0x80413830
    // Inline
    virtual ~ItemThunder(); // 0x80278628

    u8 _[0x350 - 0x2b0];
}; // class ItemThunder
#endif // ITEMTHUNDER_H