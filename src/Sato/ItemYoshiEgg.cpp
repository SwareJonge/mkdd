#include "Sato/ItemYoshiEgg.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Util.h"
#include "JSystem/JGeometry/Vec.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/ItemObj.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/ItemTurtle.h"
#include "Sato/J3DAnmObject.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/ObjUtility.h"

#include "Sato/stMath.h"
#include "kartEnums.h"
#include "mathHelper.h"
#include <algorithm>

namespace {

f32 sEquipScale = 1.0f;
f32 sReleaseScale = 4.0f;
f32 sReleaseVelMag = 110.0f;
f32 sSpReleaseVelMag = 40.0f;
f32 sHeightOffset = 25.0f;
f32 sLerpRatio = 0.3f;
f32 sScaleUpRatio = 1.1f;
f32 sFrontItemVel = 130.0f;
f32 sBananaFrontItemVel = 100.0f;
f32 sFrontItemDirY = 0.4f;
f32 sFrontSideAngle = 0.2f;
f32 sFinalBoundSpdY = 70.0f;
s16 sMaxMoveCount = 300;

u32 sTmpKindList[5];
u32 sTmpRatioList[5];

u32 sCreateItemKind[5] = {
    3, 0, 5, 6, 8
};

u32 sCreateItemRatioFront[5] = {
    20, 20, 45, 10, 5
};

u32 sCreateItemRatioBack[5] = {
    35, 35, 15, 5, 10
};
}


J3DAnmTexPattern *ItemYoshiEgg::sTexPattern;
JGeometry::TVec3f ItemYoshiEgg::sHandOffsetPos(15.0f, 5.0f, -30.0f);

ItemYoshiEgg::ItemYoshiEgg(u32 kind) : ItemRTurtle(kind) {
    reset();
    mHeightOffset = sHeightOffset;
}

void ItemYoshiEgg::reset() {
    ItemRTurtle::reset();
    mLerpRatio = sLerpRatio;
    _304 = true;
    _305 = 0;
    _306 = 0;
    _308.zero();
    mMat.resetFrame();
}

const char *ItemYoshiEgg::getBmdFileName() {
    static const char *cYoshiEggBmdName = "/Item/item_egg.bmd";
    return cYoshiEggBmdName;
}

void ItemYoshiEgg::loadAnmData(J3DModelData *mdlData) {
    J3DAnmObjMaterial::setupTexPatternAnmData(&sTexPattern, mdlData, ObjUtility::getPtrMainArc("/Item/item_egg.btp"));
}

void ItemYoshiEgg::createModel(JKRSolidHeap *heap, u32 p2, u32 p3) {
    mModel.createDifferedModel(heap, p2, p3 | (0x1000000 | 0x20000), false);
    mMat.setExModel(&mModel);
    mMat.setAnmBase(sTexPattern);
    mMat.initFrameCtrl();
}

void ItemYoshiEgg::update() {
    mMat.anmFrameProc();
    mModel.update(0);
}

void ItemYoshiEgg::doUniv() {
if (!tstAnyObjectHitting()) {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sEquipScale);
            break;
        case 4:
        case 6:
            doFallScaling();
            setScaleCol(mEquipScale);
            break;
        case 7:
            doForceDisappearScaleDown();
            break;
        default:
            if (mEquipScale < sReleaseScale) {
                mEquipScale *= sScaleUpRatio;
                if (mEquipScale > sReleaseScale) {
                    mEquipScale = sReleaseScale;
                }
            }
            setScaleCol(mEquipScale);
            break;
        }
    }

    (this->*mStateFunc)();
    
    if (mState == 2 || mState == 3) {
        mSoundMgr->setSe(0x40061);
    }    

    if (!_305 && (mState == 2 || mState == 3) && mWaitFrame > sMaxMoveCount) {
        _305 = true;
    }
    else if (mTransferFlags & 2 && _305) {
        if (!_306) {
            _306 = true;
            mVel.y += sFinalBoundSpdY;
        }
        else {
            setStateDisappear(true);
        }
    }

}

void ItemYoshiEgg::initEquip() {
    ItemGTurtle::initEquip();

    if (mColorID == 0) {
        ECharID driverID = RCMGetManager()->getKartInfo(getOwnerNum())->getDriverCharID(getDriverNum());
        if (driverID == BIRDO) {
            mColorID = 2; // Red
        }
        else {
            mColorID = 1; // Green
        }
    }
#line 274
    JUT_MINMAX_ASSERT(1, getItemColorID(), 3);

    f32 frame = 0.0f;
    switch (getColorID()) {
    case 1:
        frame = 0.0f;
        break;
    case 2:
        frame = 1.0f;
        break;
    default:
        break;
    }

    mMat.setFrame(frame);
}

void ItemYoshiEgg::initRelease() {
    ItemRTurtle::initRelease();
    mMaxVel = sReleaseVelMag;
}

void ItemYoshiEgg::initSpRelease() {
    _304 = false;
    ItemGTurtle::doUnivInitRelease(sSpReleaseVelMag, true);
}

void ItemYoshiEgg::initFall() {
    ItemObj::initFall();
    _304 = true;
}

void ItemYoshiEgg::initDisappear() {
    JGeometry::TPos3f m;
    
    mEquipFlags = 0;
    _12c &= ~1;
    mState = 10;
    
    f32 randDir = getItemRnd()->get_ufloat_1() * F_TAU;
    JGeometry::TVec3f initialPos(mPos);
    initialPos.y += 30.0f;
    
    const char *emtName = (getColorID() == 1) ? "mk_yEgg_a" : "mk_cEgg_a";
    GetJPAMgr()->createEmt(emtName, mPos);
    mSoundMgr->setSe(0x4002b);

    u32 kinds[3] = {0,0,0};
    calcCreateItem(kinds);

    m.zeroTrans();

    JGeometry::TVec3f xzDir;
    if (mTransferFlags & 4) {        
        _304 = true;
        xzDir = _308;
    }
    else if (_304) {
        xzDir = mVel;
        xzDir.y = 0.0f;
        stVecNormalize(xzDir);
    }

    for (u8 i = 0; i < 3; i++) {
        JGeometry::TVec3f vel(0.0f, 0.0f, 1.0f);

        f32 frontItemVel = 40.0f;
        f32 frontItemDirY = 2.0f;

        if (_304) {
            f32 v = sFrontSideAngle * i + -sFrontSideAngle;
            f32 s = sin(v);
            f32 c = cos(v);
            m[0][2] = s;
            m[1][1] = 1.0f;
            m[0][0] = c;
            m[2][0] = -s;
            m[2][2] = c;            
            m[2][1] = 0.0f;
            m[1][2] = 0.0f;        
            m[1][0] = 0.0f;
            m[0][1] = 0.0f;
            PSMTXMultVec(m, &xzDir, &vel);

            frontItemVel = sFrontItemVel;
            if (kinds[i] == 3 || kinds[i] == 8) {
                frontItemVel = sBananaFrontItemVel;
            }
            frontItemDirY = sFrontItemDirY;
        }
        else {
            f32 s = sin(randDir);
            f32 c = cos(randDir);
            m[0][2] = s;
            m[1][1] = 1.0f;
            m[0][0] = c;
            m[2][0] = -s;
            m[2][2] = c;            
            m[2][1] = 0.0f;
            m[1][2] = 0.0f;        
            m[1][0] = 0.0f;
            m[0][1] = 0.0f;
            PSMTXMultVec(m, &vel, &vel);
            randDir += 2.0923009f;
        }

        vel.y += frontItemDirY;
        stVecNormalize(vel);
        vel.scale(frontItemVel);
        JGeometry::TVec3f pos(initialPos);
        pos.add(vel);
        ItemObj *obj = GetItemObjMgr()->occurItem(kinds[i], pos, vel);
        if (obj) {
            obj->setOwnerNum(mOwnerNum);
            obj->setDriverNum(mDriverNum);
        }
    }
}

void ItemYoshiEgg::doDisappear() {}

bool ItemYoshiEgg::calcRotQuat(Quaternion *q) {
    f32 len = mVel.length();
    len = len / (sReleaseVelMag * 0.8f);
    f32 f = ((len) > (1.0f) ? (1.0f) : (len));
    f = f * 0.3f + 0.01f;
    f = ((0.35f) < (f) ? (0.35f) : f);

    ItemObj::rotationRad(_2b0, f);

    JGeometry::TVec3f velXZ(mVel);
    velXZ. y = 0.0f;
    stVecNormalize(velXZ);
    JGeometry::TVec3f dir;
    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    
    dir.cross(up, velXZ);

    JGeometry::TPos3f m;
    m.setRotate(dir, _2b0);
    stMtx2Qt(&_44, m);
    return false;
}

void ItemYoshiEgg::calcCreateItem(u32 *outKinds) {
    u32 listSize = 5;
    u32 used[2] = {0, 0};
    const u32 *pItemRatio = _304 ? sCreateItemRatioFront : sCreateItemRatioBack;
    
    for (u8 i = 0; i < 5; i++) {
        sTmpRatioList[i] = pItemRatio[i];
        sTmpKindList[i] = sCreateItemKind[i];
    }

    //----------------------------------------------------------------------
    // 4.  Produce three item IDs
    //----------------------------------------------------------------------
    for (u8 egg = 0; egg < 3; egg++)
    {
        //--------------------------------------------------------------
        // 4?a. Choose one item using the current working set
        //--------------------------------------------------------------
        outKinds[egg]    = calcRndItemKind(listSize, sTmpKindList, sTmpRatioList);

        //--------------------------------------------------------------
        // 4?b.  Mark whether we just produced one of the "rare" kinds
        //--------------------------------------------------------------
        if (sCreateItemKind[3] == outKinds[egg])      // table index 3  (stack flag 0)
            used[0] = 1;
        else if (sCreateItemKind[4] == outKinds[egg]) // table index 4  (stack flag 1)
            used[1] = 1;

        //--------------------------------------------------------------
        // 4?c.  Append any still?unused rare kinds to the working set
        //--------------------------------------------------------------
        u8 appended = 0;
        for (u8 srcIdx = 2; srcIdx < 4; )   // indices 3 and 4
        {
            srcIdx++;
            if (used[srcIdx - 3] != 1)                   // not produced yet?
            {
                sTmpRatioList[3 + appended] = pItemRatio[srcIdx];
                sTmpKindList[3 + appended]  = sCreateItemKind[srcIdx];
                appended++;
            }
        }

        //--------------------------------------------------------------
        // 4?d.  Next round will draw from the three gcommonh entries
        //       plus whatever we have just appended (0?2 entries)
        //--------------------------------------------------------------
        listSize = appended + 3;     // 3 mandatory + newly added
    }
}

u32 ItemYoshiEgg::calcRndItemKind(u32 n, u32 *pKindList, u32 *pRatioList) {
    u32 max = 0;
    for (u32 i = 0; i < n; i++) {
        max += pRatioList[i];
    }

    u32 rnd = getItemRnd()->getRandomMax(max - 1);
    u32 prevRatio = 0;
    u32 tmpRatio = 0;
    u32 ret = 0;
    for (u32 i = 0; i < n; i++) {
        tmpRatio += pRatioList[i];
        if (prevRatio <= rnd && tmpRatio > rnd) {
            ret = pKindList[i];
            break;
        }
        prevRatio = tmpRatio;
    }
    return ret;
}

ItemObj::ItemColFlag ItemYoshiEgg::calcColReaction(ItemObj *obj) {
    if (obj->mItemKind == 1 && ((obj->mVel.x != 0.0f) || (obj->mVel.z != 0.0f)) || ((mVel.x == 0.0f) && (mVel.z == 0.0f))) {
        mVel = obj->mVel;
    }
    return cColFlag_4;
}

void ItemYoshiEgg::moveColWall(CrsGround &ground) {
    ItemObj::moveColWall(ground);
    ground.getWallNormal(&_308, nullptr);
    ItemObj::setStateDisappear(true);
}

const bool ItemYoshiEgg::checkKartColDisappear(int kartIndex) {
    if (mVel.x == 0.0f && mVel.z == 0.0f) {
        ObjUtility::getKartVel(kartIndex, &mVel);
    } 
    return true;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
