#include "Sato/ItemHeart.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/J3DEfctHeart.h"
#include "Sato/ObjUtility.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

namespace
{
    f32 sScale = 1.0f;
    f32 sBoundsRadius = 30.0f;
};

ItemHeart::ItemHeart(u32 kind) : ItemObj(kind)
{
    reset();
}

void ItemHeart::reset()
{
    resetObject();
    mKartReaction = 15;
    _12c |= 8;
    _12c |= 0x200;

    if (mBounds)
        mBounds->setRadius(sBoundsRadius);

    mNoHeartsRemaining = false;
    mRot = 0.0f;
    PSMTXIdentity(mRotMtx);
    mLostNum = 2;
    mRemainingNum = 2;

    for (u8 i = 0; i < 2; i++)
    {
        mEffects[i] = nullptr;
        mEffectIDs[i] = -1;
    }
}

const char *ItemHeart::getBmdFileName() { return "/Item/item_heart.bmd"; }

void ItemHeart::createModel(JKRSolidHeap *heap, u32, u32) {}

void ItemHeart::doUniv()
{
    if (!tstAnyObjectHitting())
    {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sScale);
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

int ItemHeart::setHitItem(const ItemObj &rObj)
{
    bool ret = false;
    u8 driverIndexes[2] = {0, 0};
    u8 tandemNum = ItemObjMgr::getNowTandemDriverNum(mOwnerNum);

    driverIndexes[0] = tandemNum;
    if (tandemNum == 0)
    {
        driverIndexes[1] = 1;
    }

    mRemainingNum--;
    J3DEfctHeart *effect = mEffects[mRemainingNum];
    for (u8 i = 0; i < 2; i++)
    {
        const u8 driver_index = driverIndexes[i];
        if (GetItemObjMgr()->getKartEquipItem(mOwnerNum, driver_index))
            continue;

        ret = GetItemObjMgr()->stockItemToKart(rObj.getKind(), mOwnerNum, driver_index, true, rObj.getColorID());

        if (ret)
        {
            mEffectIDs[mRemainingNum] = mRemainingNum;
            effect->initItemHit(rObj);
            break;
        }
    }

    if (!ret)
    {
#line 202
        JUT_MINMAX_ASSERT(0, mRemainingNum, 2);
        effect->mHeartLink = this;
        effect->setHitStartPos(rObj);
        // This block is probably inline(expand set_dc)
        effect->_dd = effect->_dc;
        effect->set_dc(3);
        effect->_de = 1;
        effect->_ec = 0;
        //
    }
    if (mRemainingNum <= 0)
    {
        mNoHeartsRemaining = true;
    }

    return ret;
}

void ItemHeart::initMoveEffect(ItemObj *pObj)
{
    s16 efctId = -1;

    for (int i = 1; i >= 0; i--)
    {
        if (mEffectIDs[i] >= 0)
        {
            efctId = mEffectIDs[i];
            mEffectIDs[i] = -1;
            break;
        }
    }

#line 234
    JUT_ASSERT(efctId != -1);
    mEffects[efctId]->setMoveEfct(pObj, this);
}

void ItemHeart::initEquip()
{
    mEquipFlags = 0;
    mpHandAnmMtx = getHandAnmMtx();
    mVel.zero();
    mPos.zero();
    JMath::gekko_ps_copy12(mRotMtx, _210); // NOTE: mRotMtx might be SMatrix34C, spot other inlnes to be sure
}

void ItemHeart::doEquip()
{
    rotationRad(mRot, 0.05f);
    f32 r = mRot;
    f32 s = sin(r);
    f32 c = cos(r);
    Mtx m;

    m[1][1] = 1.0f;
    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    m[2][2] = c;
    m[2][1] = 0.0f;
    m[1][2] = 0.0f;
    m[1][0] = 0.0f;
    m[0][1] = 0.0f;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    PSMTXConcat(mRotMtx, m, _210);
    doHandOneEquip(false, 0.0f);
}

void ItemHeart::initRelease()
{
    _12c |= 0x80;
    if (mShadow)
        mShadow->clrVisibleAll();

    GetItemObjMgr()->deleteHeartItem(mOwnerNum);
    GetItemObjMgr()->setHeartItem(mOwnerNum, this);

    for (u8 i = 0; i < 2; i++)
    {
        const int owner = mOwnerNum;
        mEffects[i] = (J3DEfctHeart *)GetJ3DEfctMgr()->setEffectKart(owner, 4, ObjUtility::getKartBodyMtx(owner));
        mEffects[i]->_f4 = i;
    }
}

void ItemHeart::initSpRelease() { setStateRelease(true); }

void ItemHeart::doRelease()
{
    if (mLostNum > 0)
        return;

    setStateDisappear(true);
}

void ItemHeart::initDisappear()
{
    _12c &= ~1;
    GetItemObjMgr()->setHeartItem(mOwnerNum, nullptr);
    mState = 10;

    for (int i = mLostNum; i > 0; i--)
    {
        J3DEfctHeart *effect = mEffects[i - 1];
        if (effect)
        {
            effect->_dd = effect->_dc;
            effect->set_dc(2);
            effect->_de = 1;
            effect->_ec = 0;
        }
    }
}

void ItemHeart::doDisappear() {}

void ItemHeart::initFall()
{
    setStateForceDisappear();
}
