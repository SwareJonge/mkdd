#include <std/math.h>

#include "Osako/shadowMgr.h"
#include "Osako/shadowModel.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

bool ShadowModel::createModel(JKRSolidHeap *heap, u32 p2, u32 p3)
{
    ShadowManager::ptr()->regist(this);
    return ExModel::createModel(heap, p2, p3);
}

void KartShadowModel::calc()
{
    Mtx r_m, s_m;
    const Mtx &modelMtx = mModel[0]->getBaseTRMtx();
    const Vec up = {0.0f, 1.0f, 0.0f};

    PSMTXScale(s_m, mScaleX, 1.0f, mScaleY);
    /*(180.0f / PI) */
    PSMTXRotAxisRad(r_m, &up, MTXDegToRad(57.29578f * std::atan2f(modelMtx[0][2], modelMtx[2][2])));
    PSMTXConcat(r_m, s_m, r_m);

    f32 x, y, z; // fakematch?

    x = modelMtx[0][3];
    z = modelMtx[2][3];
    y = modelMtx[1][3];

    r_m[0][3] = x;
    r_m[1][3] = y;
    r_m[2][3] = z;

    mModel[0]->setBaseTRMtx(r_m);

    f32 roll = mRoll / (3.14f/2.0f);

    for (; roll > 3.0f; roll -= 2.0f)
        ;
    for (; roll < -3.0f; roll += 2.0f)
        ;

    if (roll > 1.0f)
        roll = 2.0f - roll;
    if (roll < -1.0f)
        roll = roll + 2.0f;
    roll *= 10.0f;

    mAnmCluster->setFrame(10.0f + roll);
    mModel[0]->getBaseTRMtx()[1][3] = mPosY;
    ExModel::calc();
}

bool KartShadowModel::createModel(JKRSolidHeap *heap, u32 p2, u32 p3)
{
    ShadowManager::ptr()->regist(this);
    bool ret = ExModel::createModel(heap, p2, p3);
    mModel[0]->setDeformData(mDeformData, 1);
    mDeformData->setAnm(mAnmCluster);
    return ret;
}