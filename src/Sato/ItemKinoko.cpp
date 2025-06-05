#include "Kaneshige/RaceMgr.h"
#include "Sato/ItemKinoko.h"
#include "Sato/ItemSuccession.h"

namespace // NOTE: this probably belongs in a header, this is likely the cause of all float slack stuff
{
    inline void QuaternionReset(Quaternion *pDest)
    {
        const Quaternion q = {0.0f, 0.0f, 0.0f, 1.0f};
        *pDest = q;
    }
}

namespace
{
    JGeometry::TVec3<s16> sBuildRotAngle(0, 32836, 0);
    f32 sScaleEquip = 1.0f;
    f32 sHeightOffset = 22.0f;
    f32 sGScaleEquip = 1.0f;
    f32 sGHeightOffset = 22.0f;
}

#include "JSystem/JAudio/JASFakeMatch2.h"

ItemKinoko::ItemKinoko(u32 kind) : ItemObj(kind)
{
    reset();

    mJuggle = new ItemJuggle(this);
    mBuild = new ItemBuild(this);
}

void ItemKinoko::reset()
{
    resetObject();
    mKartReaction = 6;
    _28 = 0.5f;
    mHeightOffset = sHeightOffset;
    _2c = 0.5f;
    mLerpRatio = 0.3f;
    _2b0 = 0.0f;
    _12c |= 8;
}

const char *ItemKinoko::getBmdFileName()
{
    static const char *cKinokoBmdName = "/Item/item_kinoko.bmd";
    return cKinokoBmdName;
}

void ItemKinoko::createModel(JKRSolidHeap *, u32, u32) {}

void ItemKinoko::doUniv()
{
    if (!tstAnyObjectHitting())
    {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sScaleEquip);
            break;
        case 4:
        case 6:
            doFallScaling();
            setScaleCol(mEquipScale);
            break;
        case 7:
            doForceDisappearScaleDown();
            break;
        }
    }
    (this->*mStateFunc)();
}

void ItemKinoko::initEquip()
{
    mEquipFlags = 0;
    mVel.zero();
    mPos.zero();

    mMulti->ParamInit(sBuildRotAngle);
    _2b0 = F_PI * (2.0f * ItemObj::getItemRnd()->get_ufloat_1() - 1.0f);

    QuaternionReset(&_34);
}

void ItemKinoko::doEquip() { executeEquipHandFunc(); }

void ItemKinoko::initRelease() { setStateDisappear(true); }

void ItemKinoko::doRelease() {}

void ItemKinoko::initDisappear()
{
    _12c &= ~1;
    mState = 10;
}

void ItemKinoko::doDisappear() {}

void ItemKinoko::initFall()
{
    if (RCMGetManager()->isRaceModeTA())
        setStateForceDisappear();
    else
        ItemObj::initFall();
}

void ItemKinoko::executeEquipHandFunc() { mMulti->MoveEquip(); }

ItemGoldenKinoko::ItemGoldenKinoko(u32 kind) : ItemKinoko(kind)
{
    ItemKinoko::reset(); // This already gets called in the ctor of ItemKinoko :)
}

void ItemGoldenKinoko::reset()
{
    ItemKinoko::reset();
    mHeightOffset = sGHeightOffset;
    _12c |= 0x200;
}

const char *ItemGoldenKinoko::getBmdFileName()
{
    static const char *cKinokoBmdName = "/Item/item_kinoko_gold.bmd";
    return cKinokoBmdName;
}

void ItemGoldenKinoko::loadAnmData(J3DModelData *model)
{
    if (model)
        return;
}

void ItemGoldenKinoko::createModel(JKRSolidHeap *heap, u32 p2, u32 p3) { mModel.createDifferedModel(heap, p2, p3 | 0x200, false); }

void ItemGoldenKinoko::doUniv()
{
    if (!tstAnyObjectHitting())
    {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sGScaleEquip);
            break;
        case 2:
            mEquipScale = sGScaleEquip;
            executeEquipScaleUp(0, getThunderScale());
            break;
        case 4:
        case 6:
            doFallScaling();
            setScaleCol(mEquipScale);
            break;
        case 7:
            doForceDisappearScaleDown();
            break;
        }
    }
    (this->*mStateFunc)();
}

void ItemGoldenKinoko::initRelease()
{
    _2a8 = 0;
    mTransferFlags &= ~0x20;
}

void ItemGoldenKinoko::doRelease() { ItemKinoko::doEquip(); } // Interesting..

void ItemGoldenKinoko::initDisappear() { ItemKinoko::initDisappear(); }

void ItemGoldenKinoko::update() { mModel.update(0); }

ItemKinokoSuc::ItemKinokoSuc(u32 kind) : ItemObjSuc(kind) {}

void ItemKinokoSuc::reset()
{
    ItemObjSuc::reset();
    mKartReaction = 0xf;
    _1fc = 3;
    mSucKind = 5;
    _2f8 = 0.15f;
}
