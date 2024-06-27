#include "Kaneshige/LightMgr.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/ItemBomb.h"
#include "Sato/ItemFlyTurtle.h"
#include "Sato/JPEffectMgr.h"
#include "Yamamoto/kartCtrl.h"

// WIP
#include "mathHelper.h"


namespace
{
    u8 sMaxWaitFrame = 0;
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
    f32 sPoseAngle = 2.0f;
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

JGeometry::TVec3f ItemFlyTurtle::sHandOffsetPos(20.0f, 5.0f, -30.0f);

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
    resetObject();
    mKartReaction = 8;
    _28 = 0.5f;
    mHeightOffset = sGndOffsetY;
    _6c = 0.0f;
    _2c = 0.5f;
    mLerpRatio = sLerpRatio;
    mController.Reset();

    if(mController.IsAvailableTrans())
        mController.StopTrans();

    if(mNewBounds)
        mBounds = mNewBounds;
    
    if(mBounds) {
        mBounds->setRadius(sBoundRadius);
        mBounds->setScale(1.0f);
    }

    mMoveState = 0;
    mTargetKartNo = 0;
    mLockOnKartNo = -1;
    mSearchFrame = 0;
    mHeight = 0.0f;
    mAttackVel.zero();
    _2f4.zero();
    _300.zero();
    _30c.zero();
    _318.zero();
    _324 = 0.0f;
    _328 = 0;
    _329 = 0;
    mCLPointController->reset();
    mBombEffect->reset();
    mAttackFrame = 0;
    mEmitter = nullptr;
    _340 =  0.0f;
    _33c = false;
    _33d = 0;
    mNowHeight = 0.0f;
    m1stKartNo = 0;
    _34c = 0;
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
    const char *ret = nullptr;
    switch (bckID)
    {
    case 0:
        ret = "/Item/item_k_yellow_wait.bck";
        break;
    case 1:
        ret = "/Item/item_k_yellow_fly.bck";
        break;
    }
    return ret;

}

void ItemFlyTurtle::loadAnmData(J3DModelData *modelData)
{
    if (modelData)
    {
        for (int i = 0; i < 2; i++)
        {
            const char *bckName = getBckName(i);
            void *bck = ResMgr::getPtr(ResMgr::mcArcMRAM,bckName);
            J3DAnmObjTrans::setupTransAnmData(mAnmTrans + i, mAnmCalc + i, modelData, bck);
        }
    }
}

void ItemFlyTurtle::createModel(JKRSolidHeap *heap, u32 p2, u32 id) {
    mModel.createDifferedModel(heap, p2, id | 0x1000200, true);
    mController.InitRegistration(2, &mModel);
    for(u8 i = 0; i < 2; i++) {
        mController.RegisterTrans(i, mAnmTrans[i], mAnmCalc[i]);
        J3DFrameCtrl *ctrl = mController.getFrameCtrl(i);
        ctrl->setAttribute(2);
    }
}

void ItemFlyTurtle::createColModel(J3DModelData *) { createBoundSphere(sBoundRadius, 1.0f); }

void ItemFlyTurtle::update()
{
    mController.SetAnimation();
    mModel.update(0);
}

void ItemFlyTurtle::setCurrentViewNo(u32 viewNo)
{
    ItemObj::setCurrentViewNo(viewNo);
    LightObj *lightobj = LightMgr::getManager()->searchLight('SCN0' + viewNo);
    MtxPtr m = lightobj->getEffectMtx();
    mModel.setEffectMtx(m, 1);
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

void ItemFlyTurtle::initRelease() {
    find1StKartNo();
    mCLPointController->init_ByKartIndex(mOwnerNum);
    mMoveState = 0;

    if(mController.IsAvailableTrans()) {
        mController.SetAllWeight(1);
        mController.ChangeTransAnm(1, true);
    }

    mMaxVel = sMaxVel;
    setInitReleaseParam();
    ObjUtility::getKartZdir(mOwnerNum, &mVel);
    mVel.scale(sFirstVelMag);

    JGeometry::TVec3f kartVel;
    ObjUtility::getKartVel(mOwnerNum, &kartVel);
    kartVel.y = 0.0f;
    mVel.add(kartVel);

    JGeometry::TVec3f kartYDir;
    ObjUtility::getKartYdir(mOwnerNum, &kartYDir);
    kartYDir.scale(30.0f);
    mVel.y += kartYDir.y;

    Mtx &m = ObjUtility::getKartBodyMtx(mOwnerNum);
    stMtx2Qt(&_44, m);

    ObjUtility::getKartZdir(mOwnerNum, &_240);
    stVecNormalize(_240);

    _2f4 = mVel;
    stVecNormalize(_2f4);

    _340 = mPos.y;
    mLapChecker->start(RCMGetKartChecker(mOwnerNum)->getCheckSector());
}

void ItemFlyTurtle::setInitReleaseParam() {
    mEquipFlags = 0;
    _12c &= ~2;
    _12c &= ~1;
    initMoveColGround();
    mEmitter = GetJPAMgr()->createEmt("mk_kouraBlur_b", mPos);
}

void ItemFlyTurtle::doRelease() {
    mLapChecker->calc(mPos);
    calcNowHeight();
    switch (mMoveState) {
    case 0:
        moveWait();
        break;
    case 1:
        moveFlying();
        break;
    case 2:
        moveSearchBack();
        break;
    case 3:
        moveSearch();
        break;
    case 4:
        moveAttack();
        break;
    }

    if (mEmitter) {
        mEmitter->setGlobalTranslation(mPos);
    }
    
    mSoundMgr->setSe(0x4008b);
}

void ItemFlyTurtle::initForceDisappear() {
    deleteEffect();
    ItemObj::initForceDisappear();
}

void ItemFlyTurtle::deleteEffect() {
    if(mEmitter == nullptr)
        return;
    
    GetJPAMgr()->deleteEmt(mEmitter);
    mEmitter = nullptr;
}

void ItemFlyTurtle::moveWait() {
    JGeometry::TVec3f kartVel;
    ObjUtility::getKartVel(mOwnerNum, &kartVel);
    mPos.add(kartVel);

    JGeometry::TVec3f kartZDir;
    ObjUtility::getKartZdir(mOwnerNum, &kartZDir);
    mVel.x = mVel.z = 0.0f;
    mVel.y *= sMoveVelDownRatio;   
    _2f4 = kartZDir;
    _2f4.y = 0.03f * mVel.y;
    stVecNormalize(_2f4);

    moveCommon();
    if(mWaitFrame > sMaxWaitFrame) {
        mMoveState = 1;
        mVel.x = kartVel.x;
        mVel.z = kartVel.z;
    }
    _340 = mPos.y;
}

void ItemFlyTurtle::moveFlying() {
    JGeometry::TVec3f moveVel;
    JGeometry::TVec3f nextPos;
    mCLPointController->getNext()->getPosition(&nextPos);

    moveVel.sub(nextPos, mPos);
    moveVel.y = 0.0f;
    stVecNormalize(moveVel);

    moveVel.scale(sMoveAddVel);
    mVel.add(moveVel);
    mVel.scale(sMoveVelDownRatio);
    _33c = mCLPointController->doNextChkIsOverFwPoint(mPos);
    _2f4 = mVel;

    stVecNormalize(_2f4);
    moveCommon();
    find1StKartNo();
    chkSearchTargetKartRadius();
}

void ItemFlyTurtle::moveSearchBack() {
    JGeometry::TVec3f kartPos;
    JGeometry::TVec3f kartZDir;
    JGeometry::TVec3f offset;  
    JGeometry::TVec3f searchVel;
    JGeometry::TVec3f search;

    ObjUtility::getKartPos(mTargetKartNo,&kartPos);
    ObjUtility::getKartZdir(mTargetKartNo,&kartZDir);

    kartZDir.y = 0.0f;
    stVecNormalize(kartZDir);
    kartZDir.negate();
    offset = kartZDir;
    offset.scale(sSearchOffsetLength);
    offset.add(kartPos);;
    searchVel.sub(offset, mPos);
    searchVel.y = 0.0f;
    stVecNormalize(searchVel);
    searchVel.scale(sSearchAddVel);
    mVel.add(searchVel);
    mVel.scale(sMoveVelDownRatio);
    _2f4 = mVel;
    stVecNormalize(_2f4);
    mCLPointController->doNextChkIsOverFwPoint(mPos);
    moveCommon();
    if(!_329)
        searchVel.negate();

    search.sub(offset, mPos);
    search.y = 0.0f;
    f32 searchLen = search.length();

    if(searchLen < sSearchMinLength || kartZDir.dot(searchVel) > 0.0f) {
        mMoveState = 3;
        mSearchFrame = 0;
    }
}

void ItemFlyTurtle::moveSearch() {
    JGeometry::TVec3f kartPos;
    JGeometry::TVec3f kartVel;
    JGeometry::TVec3f attackVel;

    ObjUtility::getKartPos(mTargetKartNo,&kartPos);
    ObjUtility::getKartVel(mTargetKartNo,&kartVel);
    attackVel.sub(kartPos, mPos);
    attackVel.y = 0.0f;
    f32 attackLen = attackVel.length();
    stVecNormalize(attackVel);
    mAttackVel = attackVel;
    attackVel.scale(sSearchAddVel);
    mVel.add(attackVel);
    kartVel.y = 0.0f;
    f32 kartVelLen = kartVel.length();    
    f32 yVel = mVel.y;
    mVel.y = 0.0f;

    if (attackLen > sSearchNearLength && mVel.length() > kartVelLen) {
        
        mVel.scale(sSearchNearVelDownRatio);
    }
    else {
        stVecNormalize(mVel);
        mVel.scale(kartVelLen);
    }
    mVel.y = yVel;
    mVel.y *= sMoveVelDownRatio;
    _2f4 = mAttackVel;
    stVecNormalize(_2f4);
    moveCommon();

    if(++mSearchFrame > sMaxSearchFrame) {
        mEquipFlags |= 1 << mTargetKartNo;
        mMoveState = 4;
        mAttackVel = attackVel;
        stVecNormalize(mAttackVel);

        JGeometry::TVec3f yNorm(0.0f, 1.0f, 0.0f);
        _300 = _240;
        _30c.cross(yNorm, _300);
        _324 = 0.0f;
        _328 = false;
        mVel.zero();
    }
}

void ItemFlyTurtle::moveAttack() {
    
    JGeometry::TVec3f kartPos;
    JGeometry::TVec3f vel;
    f32 downVel = sMoveVelDownRatio;

    ObjUtility::getKartPos(mTargetKartNo, &kartPos);
    vel.sub(kartPos, mPos);
    f32 len = vel.length();
    stVecNormalize(vel);

    if(!_328) {
        if(len <= sStraightLength) {
            _328 = true;
        }
        mAttackVel = vel;
    }

    JGeometry::TVec3f scaledVel = mAttackVel;
    scaledVel.scale(sAttackAddVel);
    mVel.add(scaledVel);

    mVel.scale(downVel);
    _2f4 = mVel;
    stVecNormalize(_2f4);
    moveCommon();

    if(mVel.length() > mMaxVel) {
        setVelMax();
    }

    mAttackFrame++;
    if(mAttackFrame > sMaxAttackFrame) 
        setStateDisappear(true);
}

void ItemFlyTurtle::moveCommon() {
    mPos.add(mVel);
    doMoveColGround(nullptr, false);
    f32 ang = (57.29578f)  *(_2f4.angle(_240));
    
    if(_2f4.x == 0.0f &&_2f4.z == 0.0f ) {
        _2f4 = _240;
        _2f4.y = 0.0f;
    }

    if(ang < sPoseAngle) {
        JGeometry::TPos3f dirMtx;
        stMakeDirectionMtx(&dirMtx, _2f4, 'Z');
        stMtx2Qt(&_44, dirMtx);
    }
    else {
        stVec2QtUpdate(_44, (const Vec &)_240, (const Vec &)_2f4);
    }

    _240 = _2f4;
}

void ItemFlyTurtle::moveRescue(CrsGround &rGround) {
    if(rGround.getAttribute() == 0xff && mPos.y <= (mHeightOffset * mDisScaleAnm.y + rGround.getHeight())) {
        setStateDisappear(true);
        return;
    }
    moveNormal(rGround);
}

void ItemFlyTurtle::find1StKartNo() {
    if(_34c != 0)
        return;

    int goalKartNo = RCMGetManager()->getGoalKartNumber();
    int origTargetNo = mTargetKartNo;

    for(int i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        if (goalKartNo + 1 == RCMGetKartChecker(i)->getRank()) {
            mTargetKartNo = i;
            break;
        }        
    }

    if(goalKartNo > m1stKartNo) {
        f32 lapUD =  mLapChecker->getUD();
        f32 tgtUD = RCMGetKartChecker(mTargetKartNo)->getLapUnitDist();
        if (lapUD > tgtUD) {
            mTargetKartNo = origTargetNo;
            _34c = 1;
#line 807
            JUT_MINMAX_ASSERT(0, mTargetKartNo, RCMGetManager()->getKartNumber());
        }        
    }

    if(_34c == 0 && goalKartNo > 0)
    {
        if (goalKartNo + 1 == RCMGetKartChecker(mOwnerNum)->getRank()) {
            for(int i = 0; i < RCMGetManager()->getKartNumber(); i++) {
                if (goalKartNo == RCMGetKartChecker(i)->getRank()) {
                    mTargetKartNo = i;
                    break;
                }        
            }    
        }
    }
    m1stKartNo = goalKartNo;
}

void ItemFlyTurtle::chkSearchTargetKartRadius() {
    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mTargetKartNo, &kartPos);

    JGeometry::TVec3f posDiff;
    posDiff.sub(mPos, kartPos);    
    f32 height = JMAAbs(posDiff.y);
    f32 chkRadius = sTargetChkRadius;
    posDiff.y = 0.0f;

    CLPoint *center = GetKartCtrl()->GetKartCenterPtr(mTargetKartNo);
    if(center == mCLPointController->getNext()) {
        chkRadius = chkRadius * sSamePointRadiusMag;
    }

    if(posDiff.length() < chkRadius && height < sTargetChkHeight) {
        mMoveState = 2;
        _329 = chkIsBackTargetKartOffsetPos();
        mLockOnKartNo = mTargetKartNo;
    }
}

bool ItemFlyTurtle::chkIsBackTargetKartOffsetPos() {
    bool ret = false;
    JGeometry::TVec3f kartPos;
    JGeometry::TVec3f zDir;    
    JGeometry::TVec3f offset;
    JGeometry::TVec3f posDiff;
    ObjUtility::getKartPos(mTargetKartNo,&kartPos);
    ObjUtility::getKartZdir(mTargetKartNo,&zDir);

    zDir.y = 0.0f;
    stVecNormalize(zDir);
    zDir.negate();

    offset = zDir;
    offset.scale(sSearchOffsetLength);
    offset.add(kartPos);
    
    posDiff.sub(offset, mPos);
    posDiff.y = 0.0f;
    stVecNormalize(posDiff);
    posDiff.negate();

    if(zDir.dot(posDiff) > 0.0f)
        ret = true;

    return ret;

}

void ItemFlyTurtle::moveNormal(CrsGround &rGround) {
    mHeight = sStdHeight;
    switch(mMoveState) {
    case 0:
        mHeight = sWaitHeight;
        break;
    case 3:
        mHeight = sSearchHeight;
        break;
    case 4:
        mHeight = sAttackHeight;
        break;
    }
    f32 moveY = mDisScaleAnm.y * mHeightOffset + mNowHeight;

    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mTargetKartNo, &kartPos);
    if(mHeight == 0.0f) {
        ObjUtility::getKartPos(mOwnerNum,&kartPos);
        moveY = kartPos.y;
    }
    else if(mMoveState == 3 || mMoveState == 4) {
        moveY = kartPos.y;
    }
    f32 moveVelY = mPos.y - moveY;
    f32 add = 0.0f;
    f32 dstAbs = JMAAbs(mHeight - moveVelY);
    f32 absDst = dstAbs > add + mHeight  ? add + mHeight : dstAbs;
    f32 len = mVel.length();
    

    if(moveVelY < mHeight) {
        if(moveVelY < 0.0f)
            mPos.y = moveY;        
        mVel.y += (absDst * sHeightYVelAddUp);
    }
    else {
        f32 yVelAddDown = sHeightYVelAddDown;
        if(mMoveState == 4)
            yVelAddDown = sAttackHeightYVelAddDown;
        else if (mMoveState == 3)
            yVelAddDown = sSearchHeightYVelAddDown;

        mVel.y -= (absDst * yVelAddDown);
    }

    f32 velY = mVel.y;
    if(len > JMAAbs(velY)) {
        mVel.y = 0.0f;
        stVecNormalize(mVel);
        f32 len2 = len*len;
        f32 velY2 = velY*velY;
        f32 len = JMAFastSqrt(len2-velY2);

        mVel.scale(len);
        mVel.y  = velY;
    }

    if(rGround.getAttribute() != 2) {
        _340 = mPos.y;
        _33d = 0;
    }
}

void ItemFlyTurtle::moveColWall(CrsGround &rGround) {
    if(_33d == 0 && mMoveState == 1 && _33c) {
        _33d = 1;
    }

    if(_33d == 0) {
        mVel.y = 0.0f;
        JGeometry::TVec3f nextPos;
        JGeometry::TVec3f prevPos;
        
        mCLPointController->getNext()->getPosition(&nextPos);
        mCLPointController->getPrev()->getPosition(&prevPos);
        JGeometry::TVec3f dist;
        dist.sub(nextPos, prevPos);
        f32 yVel = dist.y;
        dist.y = 0.0f;
        f32 len = dist.length();
        stVecNormalize(dist);

        JGeometry::TVec3f posDiff;
        posDiff.sub(mPos, prevPos);
        posDiff.y = 0.0f;
        mPos.y = (dist.dot(posDiff) / len) * (yVel) + _340;
    }
    else {
       moveNormal(rGround); 
    }
}

void ItemFlyTurtle::moveCannon(CrsGround &rGround) { moveNormal(rGround); }

void ItemFlyTurtle::initDisappear() {
    _12c &= ~1;
    mEquipFlags = 0xffffffff;
    _12c |= 0x80;

    JGeometry::TVec3f norm, frDir;
    getModelFrDir(&frDir);
    if(mTransferFlags & 2) {
        norm = (mNormal);
    }
    else {
        norm.set(0.0f, 1.0f, 0.0f);
    }

    JGeometry::TMtx34f r_m;
    ObjUtility::setRMtx(r_m, norm, frDir);
    r_m[0][3] = _1c.x;
    r_m[1][3] = _1c.y;
    r_m[2][3] = _1c.z;
    mNewBounds = mBounds;
    mBounds = mBombEffect->mSphere1;
    mBombEffect->init(r_m,1, 1.0f);
}

void ItemFlyTurtle::doDisappear() {
    mDisScaleAnm.setAll(mEquipScale);

    if(ItemBomb::isReact(mWaitFrame))
        mKartReaction = 2;
    
    if(mEmitter)
        mEmitter->setGlobalAlpha(0xff - mWaitFrame * 0x33);

    if (mWaitFrame >= 5)
        deleteEffect();
    
    if(mBombEffect->execute()) {
        if (mEmitter)
            deleteEffect();
        mState = 10;
    }
}

void ItemFlyTurtle::initFall() { setStateForceDisappear(); }

void ItemFlyTurtle::calcNowHeight() {
    JGeometry::TVec3f nextPos;
    JGeometry::TVec3f prevPos;
    
    mCLPointController->getNext()->getPosition(&nextPos);
    mCLPointController->getPrev()->getPosition(&prevPos);
    
    if(mCLPointController->tstReverse()) {
        JGeometry::TVec3f swap(nextPos);
        nextPos = prevPos;
        prevPos = swap;
    }

    JGeometry::TVec3f dist;
    dist.sub(nextPos, prevPos);
    dist.y = 0.0f;
    f32 len = dist.length();
    stVecNormalize(dist);

    JGeometry::TVec3f posDiff;
    posDiff.sub(mPos, prevPos);
    posDiff.y = 0.0f;
    mNowHeight = (dist.dot(posDiff) / len) * (nextPos.y - prevPos.y) + prevPos.y;
}

CLPointControllerFlyTurtle::CLPointControllerFlyTurtle() {}

int CLPointControllerFlyTurtle::findNextPointNo(CLPoint *point) {
    int nextPointNo = 0;
    int groupID = 0;
    for(u8 i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        if(RCMGetKartChecker(i)->getRank() == 1)
            groupID = GetKartCtrl()->GetKartCenterPtr(i)->getGroupID();
    }

    int pointNo = -1;
    u8 cnt = 0;
    bool found = false;

    if (point->getPrevPointNumber() > 1) {
        bool wasReverse = mIsReverse;
        if(!wasReverse)
            mIsReverse = true;
        
        recursiveTargetCalcBack(point, groupID, &pointNo, &cnt, &found);
        if(!found && !wasReverse)
            mIsReverse = false;
    }

    if (!found) {
        pointNo = -1;
        cnt = 0;
        found = false;
        if(point->getNextPointNumber() < 2)
            return 0;

        bool wasReverse = mIsReverse;
        if(wasReverse)
            mIsReverse = false;
        
        recursiveTargetCalc(point, groupID, &pointNo, &cnt, &found);
        if(!found && wasReverse)
            mIsReverse = true;
    }
    
    if(found && pointNo != -1)
        nextPointNo = pointNo;

    return nextPointNo;

}

void CLPointControllerFlyTurtle::recursiveTargetCalcBack(CLPoint *point, int groupID, int *pPointNo, u8 *pCount, bool *pFoundTarget) {
    (*pCount)++;
    if(*pCount > 2)
        return;
    int prevPointNo = point->getPrevPointNumber();
    if(prevPointNo == 1) {
        for(int i = 0; i < 1; i++) {} // fakematch, affects inline depth
        if(point->getPrevPoint(0)->getNextPointNumber() < 2)
            return;
    }

    for(int i = 0; i < prevPointNo; i++) {
        if(*pCount == 1) {
            *pPointNo = i;
        }
        CLPoint *prevPoint = point->getPrevPoint(i);
        u8 prevGroupID = prevPoint->getGroupID();
        if(prevGroupID == groupID)
            *pFoundTarget = true;
        else {
            CLPoint *div = prevPoint->getDivPoint();
            if (div) {
                recursiveTargetCalcBack(div, groupID, pPointNo, pCount, pFoundTarget);
                (*pCount)--;
            }
        }
        if(*pFoundTarget)
            return;
    }    
}

#include "JSystem/JAudio/JASFakeMatch2.h"