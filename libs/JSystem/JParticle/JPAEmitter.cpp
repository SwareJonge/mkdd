#include "Dolphin/mtx.h"
#include "JSystem/JParticle/JPABlock.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPAShape.h"
#include "JSystem/JUtility/JUTAssert.h"

JPAEmitterCallBack::~JPAEmitterCallBack() { }

void JPABaseEmitter::init(JPAEmitterManager* manager, JPAResource* resource)
{
    mManager  = manager;
    mResource = resource;
    mResource->getDyn()->getEmitterScl(&mLocalScl);
    mResource->getDyn()->getEmitterTrs(&mLocalTrs);
    mResource->getDyn()->getEmitterDir(&mLocalDir);
    mLocalDir.normalize();
    mResource->getDyn()->getEmitterRot(&mLocalRot);
    mMaxFrame            = mResource->getDyn()->getMaxFrame();
    mLifeTime            = mResource->getDyn()->getLifetime();
    mVolumeSize          = mResource->getDyn()->getVolumeSize();
    mRate                = mResource->getDyn()->getRate();
    mRateStep            = mResource->getDyn()->getRateStep();
    mVolumeSweep         = mResource->getDyn()->getVolumeSweep();
    mVolumeMinRad        = mResource->getDyn()->getVolumeMinRad();
    mAwayFromCenterSpeed = mResource->getDyn()->getInitVelOmni();
    mAwayFromAxisSpeed   = mResource->getDyn()->getInitVelAxis();
    mDirSpeed            = mResource->getDyn()->getInitVelDir();
    mSpread              = mResource->getDyn()->getInitVelDirSp();
    mRndmDirSpeed        = mResource->getDyn()->getInitVelRndm();
    mAirResist           = mResource->getDyn()->getAirRes();
    mRandom.setSeed(mManager->pWd->mRndm.getRandU32());
    PSMTXIdentity(mGlobalRot);
    mGlobalScl.set(1.0f, 1.0f, 1.0f);
    mGlobalTrs.zero();
    mGlobalPScl.set(1.0f, 1.0f);
    mGlobalEnvClr.a = 0xff;
    mGlobalEnvClr.b = 0xff;
    mGlobalEnvClr.g = 0xff;
    mGlobalEnvClr.r = 0xff;
    mGlobalPrmClr.a = 0xff;
    mGlobalPrmClr.b = 0xff;
    mGlobalPrmClr.g = 0xff;
    mGlobalPrmClr.r = 0xff;
    resource->getBsp()->getPrmClr(&mPrmClr);
    resource->getBsp()->getEnvClr(&mEnvClr);
    mpUserWork = nullptr;
    mScaleOut  = 1.0f;
    mEmitCount = 0.0f;
    initFlag(JPAEMIT_FirstEmit | JPAEMIT_RateStepEmit);
    mDrawTimes     = 1;
    mCurrentFrame  = 0;
    mWaitTime      = 0;
    mRateStepTimer = 0;
}

JPABaseParticle* JPABaseEmitter::createParticle()
{
    if (mPtclPool->mNum != 0) {
        JPANode<JPABaseParticle>* node = mPtclPool->pop_front();
        mAlivePtclBase.push_front(node);
        mResource->getDyn()->calc(mManager->pWd);
        node->mData.init_p(mManager->pWd);
        return &node->mData;
    }
    else {
// Line 128 in TP Debug
#line 125
        JUT_WARNING_F2("%s", "JPA : Can NOT create particle more\n");
    }

    return nullptr;
}

JPABaseParticle* JPABaseEmitter::createChild(JPABaseParticle* parent)
{
    if (mPtclPool->mNum != 0) {
        JPANode<JPABaseParticle>* node = mPtclPool->pop_front();
        mAlivePtclChld.push_front(node);
        node->getObject()->init_c(mManager->pWd, parent);
        return &node->mData;
    }
    else {
// Line 151 in TP Debug
#line 148
        JUT_WARNING_F2("%s", "JPA : Can NOT create child particle more\n");
    }

    return nullptr;
}

void JPABaseEmitter::deleteAllParticle()
{
    while (mAlivePtclBase.getNum())
        mPtclPool->push_front(mAlivePtclBase.pop_back());
    while (mAlivePtclChld.getNum())
        mPtclPool->push_front(mAlivePtclChld.pop_back());
}

bool JPABaseEmitter::processTillStartFrame()
{
    if (mWaitTime >= mResource->getDyn()->getStartFrame()) {
        return true;
    }

    if (!checkStatus(JPAEMIT_StopCalc)) {
        mWaitTime++;
    }

    return false;
}

bool JPABaseEmitter::processTermination()
{
    if (checkStatus(JPAEMIT_ForceDelete)) {
        return true;
    }

    if (mMaxFrame == 0) {
        return false;
    }

    if (mMaxFrame < 0) {
        setFlag(JPAEMIT_EnableDeleteEmitter);
        return getParticleNumber() == 0;
    }

    if (mCurrentFrame >= mMaxFrame) {
        setFlag(JPAEMIT_EnableDeleteEmitter);
        if (checkStatus(JPAEMIT_Immortal)) {
            return 0;
        }
        return getParticleNumber() == 0;
    }

    return false;
}

void JPABaseEmitter::calcEmitterGlobalPosition(JGeometry::TVec3f* p1) const
{
    Mtx mtx;
    PSMTXScale(mtx, mGlobalScl.x, mGlobalScl.y, mGlobalScl.z);
    PSMTXConcat(mGlobalRot, mtx, mtx);
    mtx[0][3] = mGlobalTrs.x;
    mtx[1][3] = mGlobalTrs.y;
    mtx[2][3] = mGlobalTrs.z;
    PSMTXMultVec(mtx, (Vec*)&mLocalTrs, (Vec*)p1);
}

void JPABaseEmitter::getEmitterAxisX(JGeometry::TVec3<f32>*) const
{
    // UNUSED FUNCTION
}

void JPABaseEmitter::getEmitterAxisY(JGeometry::TVec3<f32>*) const
{
    // UNUSED FUNCTION
}

void JPABaseEmitter::getEmitterAxisZ(JGeometry::TVec3<f32>*) const
{
    // UNUSED FUNCTION
}

int JPABaseEmitter::getCurrentCreateNumber() const { return mManager->pWd->mCreateNumber; }

int JPABaseEmitter::getDrawCount() const
{
    // UNUSED FUNCTION
}

void JPABaseEmitter::loadTexture(u8, _GXTexMapID)
{
    // UNUSED FUNCTION
}
