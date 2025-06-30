#ifndef ITEMSUCCESSION_H
#define ITEMSUCCESSION_H

#include "Sato/ItemObj.h"

// TODO: virtual and static functions

class ItemMultiBase
{
public:
    ItemMultiBase(ItemObj *link) { mObj = link; }
    ~ItemMultiBase() {}

    // Inline/Unused
    void getOffsetScale();
    void setDivestedStartPos(const JGeometry::TVec3f &);

    // Vtable 0
    virtual void ParamInit(const JGeometry::TVec3<s16> &) = 0;
    virtual void DoEquipScaleUp(f32) = 0;
    virtual void MoveEquip() = 0;
    virtual void InitMoveDivested() = 0;
    virtual void MoveDivested() = 0;

    ItemObj *mObj; // 4
};

class ItemJuggle : public ItemMultiBase
{
public:
    ItemJuggle(ItemObj *link) : ItemMultiBase(link) {}

    void doHandJuggleEquip(const JGeometry::TVec3f &, Mtx);
    void doMoveJuggleEquip(const JGeometry::TVec3f &, Mtx, JGeometry::TVec3f, f32);

    // Vtable 0
    virtual void ParamInit(const JGeometry::TVec3<s16> &); // 8
    virtual void DoEquipScaleUp(f32);                      // C
    virtual void MoveDivested();                           // 10
    virtual void InitMoveDivested();                       // 14
    virtual void MoveEquip();                              // 18

    static f32 sJuggleRotStartOffset;
    static f32 sJuggleRotRatio;
    static f32 sJuggleRotRatioSub;

    u8 _8[0x60 - 0x8];
};

class ItemBuild : public ItemMultiBase
{
public:
    ItemBuild(ItemObj *link) : ItemMultiBase(link) {}

    void setFirstPos(JGeometry::TVec3f *, f32);
    void setVel(f32);
    void setPos(JGeometry::TVec3f *, f32);
    void setHeight(f32);
    void setRotate(Mtx);
    void setFirstRotate(Mtx);

    // Vtable
    virtual void ParamInit(const JGeometry::TVec3<s16> &); // 8
    virtual void DoEquipScaleUp(f32);                      // C
    virtual void MoveDivested();                           // 10
    virtual void InitMoveDivested();                       // 14
    virtual void MoveEquip();                              // 18

    u8 _8[0x35 - 0x8];
    u8 _35;
    u8 _36[0x68 - 0x36];
};

class ItemObjSuc : public ItemObj
{
public:
    ItemObjSuc(u32 p1) : ItemObj(p1) {
        setSucMultiType(cMultiType_0);
    }

    void initJuggleEquip();
    void doJuggleEquip();
    void setInitBuildAppend();
    void initBuildEquip();
    void doBuildEquip();
    void selectJuggleSucChild();
    void setSucMultiType(cMultiType);

    virtual void reset();
    virtual void initEquip();
    virtual void doEquip();
    virtual void selectSucChild();    
    virtual void initDivested();
    virtual void doDivested();    
    virtual void initForceDisappear();
    virtual void doForceDisappear();

    static f32 sJuggleXLength;
    static f32 sJuggleRndAngle;
    static f32 sJuggleRndAngle2;
    static f32 sJuggleRotAxisRatio;
    static f32 sJuggleGravity;
    static f32 sJuggleGravityUpRatio;
    static f32 sJuggleAnmOverFrame;

    typedef void (ItemObjSuc::*EquipFunc)();

    EquipFunc mEquipInitFunc; // 2b0
    EquipFunc mEquipDoFunc;   // 2bc
    u8 _2c8[0x2dc - 0x2c8]; //
    f32 _2dc;               //
    f32 _2e0;               //
    u32 _2e4;               //
    u32 _2e8;               //
    u8 _2ec;                //
    f32 _2f0;               //
    u8 _2f4;                //
    f32 _2f8;               //
    u8 _2fc;                //
};

#endif // ITEMSUCCESSION_H
