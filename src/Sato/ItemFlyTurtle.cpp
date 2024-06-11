#include "Kaneshige/LightMgr.h"
#include "Sato/ItemFlyTurtle.h"

// WIP

namespace
{
    f32 sEquipScale = 1.0f;
    f32 sReleaseScale = 2.2f;
    f32 sScaleUpRatio = 1.05f;
    f32 sMoveVelDownRatio = 0.85f;
    f32 sMoveAddVel = 45.0f;
    f32 sSearchAddVel = 30.0f;
    f32 sTargetChkRadius = 3000.0f;
    f32 sTargetChkHeight = 1000.0f;
    f32 sStdHeight = 300.0f;
    f32 sWaitHeight = 200.0f;
    f32 sSearchHeight = 180.0f;
    f32 sAttackHeight = 120.0f;
    f32 sHeightYVelAddUp = 0.08f;
    f32 sHeightYVelAddDown = 0.07;
    f32 sAttackHeightYVelAddDown = 0.05f;
    f32 sSearchHeightYVelAddDown = 0.05f;
    f32 sPoseAngle = 20.0f;
    f32 sBoundRadius = 40.0f;
    u8 sMaxSearchFrame = 30;
    f32 sAttackAddVel = 20.0f;
    f32 sStraightLength = 200.0f;
    f32 sMaxVel = 200.0f;
    f32 sGndOffsetY = 25.0f;
    f32 sSearchOffsetLength = 350.0f;
    f32 sSearchNearLength = 250.0f;
    f32 sSearchNearVelDownRatio = 0.85f;
    f32 sFirstVelMag = 60.0f;
    f32 sLerpRatio = 0.15f;
    f32 sSearchMinLength = 20.0f;
    s16 sMaxAttackFrame = 120;
    f32 sSamePointRadiusMag = 3.0f;
}

J3DAnmTransform *ItemFlyTurtle::mAnmTrans[2] = {nullptr, nullptr}; // 0x80414f64
J3DMtxCalc *ItemFlyTurtle::mAnmCalc[2] = {nullptr, nullptr};       // 0x80414f6c

Vec ItemFlyTurtle::sHandOffsetPos = {20.0f, 5.0f, -30.0f};

ItemFlyTurtle::ItemFlyTurtle(u32 kind) : ItemObj(kind)
{
    mCLPointController = new CLPointControllerFlyTurtle();
    mBombEffect = new BombEffectObj();
    mLapChecker = new LapChecker();
    mNewBounds = nullptr;
    reset();
}

void ItemFlyTurtle::reset()
{
    _12c |= 0x2000;
    mLapChecker->reset();
}

const char *ItemFlyTurtle::getBmdFileName()
{
    static const char *cFlyTurtleBmdName = "/Item/item_koura_yellow.bmd";
    return cFlyTurtleBmdName;
}

const char *ItemFlyTurtle::getBckName(int bckID)
{
    switch (bckID)
    {
    case 0:
        return "/Item/item_k_yellow_wait.bck";
    case 1:
        return "/Item/item_k_yellow_fly.bck";
    }

    return nullptr;
}

void ItemFlyTurtle::loadAnmData(J3DModelData *modelData)
{
    if (modelData)
    {
        for (int i = 0; i < 2; i++)
        {
            const char *bckName = getBckName(i);
            J3DAnmObjTrans::setupTransAnmData(mAnmTrans +i, mAnmCalc + i, modelData, ResMgr::getPtr(ResMgr::mcArcMRAM,bckName));
        }
    }
}

void ItemFlyTurtle::createModel(JKRSolidHeap *, u32, u32) {}

void ItemFlyTurtle::createColModel(J3DModelData *) { createBoundSphere(sBoundRadius, 1.0f); }

void ItemFlyTurtle::update()
{
    mController.SetAnimation();
    mModel.update(0);
}

void ItemFlyTurtle::setCurrentViewNo(u32 viewNo)
{
    setCurrentViewNo(viewNo);
    LightObj *lightobj = LightMgr::getManager()->searchLight('SCN0' + viewNo);
    mModel.setEffectMtx(lightobj->getEffectMtx(), 1);
}

void ItemFlyTurtle::doUniv()
{
    if (mMoveState == 4)
    {
        if (mTransferFlags & 2 || mTransferFlags & 4)
            setStateDisappear(true);
    }

    mController.FrameProc();
    calcScale(sEquipScale, sReleaseScale, sScaleUpRatio);
    (this->*mStateFunc)();
}

void ItemFlyTurtle::initEquip()
{
    mEquipFlags = 0;
    mpHandAnmMtx = getHandAnmMtx();
    mVel.zero();
    mPos.zero();
}

void ItemFlyTurtle::doEquip() { doHandOneEquip(false, 0.0f); }

void ItemFlyTurtle::initRelease() {}

void ItemFlyTurtle::setInitReleaseParam() {}

void ItemFlyTurtle::doRelease() {}

void ItemFlyTurtle::initForceDisappear() {}

void ItemFlyTurtle::deleteEffect() {}

void ItemFlyTurtle::moveWait() {}

void ItemFlyTurtle::moveFlying() {}

void ItemFlyTurtle::moveSearchBack() {}

void ItemFlyTurtle::moveSearch() {}

void ItemFlyTurtle::moveAttack() {}

void ItemFlyTurtle::moveCommon() {}

void ItemFlyTurtle::moveRescue(CrsGround &) {}

void ItemFlyTurtle::find1StKartNo() {}

void ItemFlyTurtle::chkSearchTargetKartRadius() {}

void ItemFlyTurtle::chkIsBackTargetKartOffsetPos() {}

void ItemFlyTurtle::moveNormal(CrsGround &) {}

void ItemFlyTurtle::moveColWall(CrsGround &) {}

void ItemFlyTurtle::moveCannon(CrsGround &rGround) { moveNormal(rGround); }

void ItemFlyTurtle::initDisappear() {}

void ItemFlyTurtle::doDisappear() {}

void ItemFlyTurtle::initFall() { setStateForceDisappear(); }

void ItemFlyTurtle::calcNowHeight() {}

CLPointControllerFlyTurtle::CLPointControllerFlyTurtle() {}

int CLPointControllerFlyTurtle::findNextPointNo(CLPoint *) {}

void CLPointControllerFlyTurtle::recursiveTargetCalcBack(CLPoint *, int, int *, unsigned char *, bool *) {}
