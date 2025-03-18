#include "Kaneshige/Objects/GeoItemGen.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"

#include "mathHelper.h"

s16 GeoItemGen::sNextOccurTime = 60;
s16 GeoItemGen::sOccurLimitSpeed = 50;

const u32 GeoItemGen::sRandomTable[10] = {
    8, 8, 3, 3,
    6, 6, 5, 5,
    6, 10
};

StateObserver::StateFuncSet<GeoItemGen> GeoItemGen::sTable[1] = {
    {0, &GeoItemGen::initFuncWait, &GeoItemGen::doFuncWait }
};

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoItemGen::GeoItemGen(const CrsData::SObject &obj) : GeographyObj(obj) {
    reset();
}

void GeoItemGen::reset() {
    GeographyObj::resetObject();
    _58 = 2;
    GeographyObj::setAllCheckKartHitFlag();
    GeographyObj::clrObjFlagCheckItemHitting();
    StateObserver::ResetState();
    mGenId = mObjData->mParam1 % 100;

    if (mGenId >= 5 || mGenId < 0) {
#line 180
        JUT_ASSERT_F(false, "UNKNOWN GENID(%d)", mGenId);
    }
        
    if (mObjData->mParam1 / 100 > 0) {
        _15c = true;
    }
    else {
        _15c = false;
    }

    if (mObjData->mParam2 != 0) {
        _160 = mObjData->mParam2;
    }
    else {
        _160 = 100.0f;
    }

    _164 = mObjData->mParam3;
    _168 = mObjData->mParam5;

    mOccurId = mObjData->mParam4;
    if ((mOccurId >= 5 || mOccurId < 0)) {
#line 212
        JUT_ASSERT_F(false, "UNKNOWN OCCURID(%d)", mOccurId);
    }
    
    mTimer = 0;

    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);

    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
}

void GeoItemGen::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(50.0f * mScale.x, 100.0f * mScale.y, 1.0f);
}

u32 GeoItemGen::setItemKind() {
    u32 ret = 5;

    switch (mGenId) {
        case 0: {
            u32 rand = GeographyObj::getGeoRnd()->getRandom() % 10;
#line 266
            JUT_MINMAX_ASSERT(0, rand, 10);
            ret = sRandomTable[rand];
            break;
        }
        case 1: {
            ret = 8;
            break;
        }
        case 2: {
            ret = 3;
            break;
        }
        case 3: {
            ret = 6;
            break;
        }
        case 4: {
            ret = 5;
            break;
        }
    }
    return ret;
}

void GeoItemGen::checkKartCollision() {
    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
        if (GeographyObj::tstIsHitKartFlg(i)) {
            u64 kartStatus = KartCtrl::getKartCtrl()->GetKartStatus(i);
            bool b = false;
            if (kartStatus & 4) {
                b = true;
            }
            else if (_15c) {
                if (GetKartCtrl()->GetCarSpeed(i) >= sOccurLimitSpeed) {
                    b = true;
                }
            }

            JGeometry::TVec3f zDir;
            ObjUtility::getKartZdir(i, &zDir);
            bool positive = true;
            if (zDir.dot(mFrDir) < 0.0f) {
                positive = false;
            }

            if (b) {
                switch (mOccurId) {
                case 1: {
                    if (!positive)
                        b =false;
                    break;
                }
                case 2: {
                    if (positive) {
                        positive = false;
                    }
                    break;
                }
                }
            }

            if (b && mTimer == 0) {
                mTimer = sNextOccurTime;
                JGeometry::TVec3f dir(mFrDir);

                if (!positive) {
                    dir.scale(-1.0f);
                }

                if (mOccurId == 3) {
                    dir.set(zDir);
                }
                else if (mOccurId == 4) {
                    s16 rnd = (u16)GeographyObj::getGeoRnd()->getRandom();
                    f32 s = JMASSin(rnd);
                    f32 c = JMASCos(rnd);

                    dir.zero();
                    dir.scaleAdd(s, mLfDir, dir);
                    dir.scaleAdd(c, mFrDir, dir);
                }
                JGeometry::TVec3f v1;
                v1.scaleAdd(0.3f, mUpDir, dir);
                v1.normalize();
                v1.scale(_160);

                JGeometry::TVec3f v2;
                v2.scaleAdd(_164, dir, mPos);
                v2.scaleAdd(30.0f + _168, mUpDir, v2);
                ItemObj *obj = GetItemObjMgr()->occurItem(setItemKind(), v2, v1);

                if (obj) {
                    obj->setOwnerNum(i);
                    obj->setFrameNoColKart(30, false);
                }
            }


        }
    }
}

void GeoItemGen::calc() {
    if (mTimer > 0) {
        mTimer--;
    }

    checkKartCollision();
    StateObserver::ExecuteState();
}

void GeoItemGen::InitExec() { Observer_FindAndInit(GeoItemGen, 1); }

void GeoItemGen::MoveExec() { Observer_FindAndExec(GeoItemGen, 1); }

void GeoItemGen::initFuncWait() {}

void GeoItemGen::doFuncWait() {}
