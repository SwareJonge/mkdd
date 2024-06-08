#ifndef ITEMSUCCESSION_H
#define ITEMSUCCESSION_H

#include "Sato/ItemObj.h"

// TODO: virtual and static functions

class ItemMultiBase
{
public:
    // Inline/Unused
    void getOffsetScale();
    void setDivestedStartPos(const JGeometry::TVec3f &);

    virtual void ParamInit(const JGeometry::TVec3<s16> &) = 0;
    virtual void DoEquipScaleUp(f32) = 0;
    virtual void MoveEquip() = 0;
    virtual void InitMoveDivested() = 0;
    virtual void MoveDivested() = 0;
};

class ItemJuggle : public ItemMultiBase
{
public:
    void DoEquipScaleUp(f32);
    void ParamInit(const JGeometry::TVec3<s16> &);
    void InitMoveDivested();
    void MoveDivested();
    void MoveEquip();
    void doHandJuggleEquip(const JGeometry::TVec3f &, Mtx);
    void doMoveJuggleEquip(const JGeometry::TVec3f &, Mtx, JGeometry::TVec3f, f32);

    static f32 sJuggleRotStartOffset;
    static f32 sJuggleRotRatio;
    static f32 sJuggleRotRatioSub;
};

class ItemBuild : public ItemMultiBase
{
public:
    void ParamInit(const JGeometry::TVec3<s16> &);
    void DoEquipScaleUp(f32);
    void MoveEquip();
    void setFirstPos(JGeometry::TVec3f *, f32);
    void setVel(f32);
    void setPos(JGeometry::TVec3f *, f32);
    void setHeight(f32);
    void setRotate(Mtx);
    void setFirstRotate(Mtx);
    void InitMoveDivested();
    void MoveDivested();
};

class ItemObjSuc : public ItemObj
{
public:
    ItemObjSuc(u32 p1) : ItemObj(p1) {
        setSucMultiType(cMultiType_0);
    }

    void reset();
    void initEquip();
    void doEquip();
    void initJuggleEquip();
    void doJuggleEquip();
    void setInitBuildAppend();
    void initBuildEquip();
    void doBuildEquip();
    void selectSucChild();
    void selectJuggleSucChild();
    void initDivested();
    void doDivested();
    void setSucMultiType(cMultiType);
    void initForceDisappear();
    void doForceDisappear();

    static f32 sJuggleXLength;
    static f32 sJuggleRndAngle;
    static f32 sJuggleRndAngle2;
    static f32 sJuggleRotAxisRatio;
    static f32 sJuggleGravity;
    static f32 sJuggleGravityUpRatio;
    static f32 sJuggleAnmOverFrame;
};

#endif // ITEMSUCCESSION_H
