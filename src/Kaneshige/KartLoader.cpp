#include <JSystem/J3D/J3DAnmLoader.h>
#include <JSystem/J3D/J3DClusterLoader.h>
#include <JSystem/J3D/J3DModelLoader.h>

#include "Kaneshige/KartLoader.h"
#include "JSystem/J3D/J3DTransform.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/SysDebug.h"
#include "Kaneshige/TexLODControl.h"
#include "Kawano/osage.h"
#include "Osako/ResMgr.h"
#include "Sato/J3DEffectMgr.h"
#include "JSystem/JAudio/JASFakeMatch2.h"
#include "mathHelper.h"

KartLoader::KartLoader(int kartNum, const KartInfo *kartInfo, u16 numLevels, bool isTT, bool isAwardMode) {
    mKartNum = kartNum;
    mKartInfo = kartInfo;
    mWheelNum = mKartInfo->getWheelNumber();
    mDemoBmd = nullptr;

    for(int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mDriverTevRegKey[j][i] = 0;
            mAccessoryTevRegKey[j][i] = 0;
        }
        mBodyTevRegKey[i] = nullptr;
        mArmTevRegKey[i] = nullptr;
        mShockTevRegKey[i] = nullptr;
        mWheelLTevRegKey[i] = nullptr;
        mWheelRTevRegKey[i] = nullptr;
    }

    SYSDBG_SetHeapGroup("DRIVER MGR", nullptr)

    int awardNo = 0; // i guess that's an appropriate name
    if (isAwardMode)
        awardNo = kartNum + 1;

    for (int i = 0; i < 2; i++) {
        mDriverModels[i] = new DriverModel(this, i, numLevels, isTT, awardNo);
        mOsageModel[i] = OsageMgr::ptr()->IsOsageExist(mDriverModels[i]);
    }
}

void KartLoader::reset() {
    for(int i = 0; i < 2; i++)
        mDriverModels[i]->restart(i);
}

void KartLoader::load() {
    J3DModelData *pModelData;
    if (mDemoBmd == nullptr) {
        pModelData = J3DModelLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcKartDataBody, mKartInfo->getKartID()), 0x18220010);
    }
    else {
        pModelData = J3DModelLoaderDataBase::load(mDemoBmd, 0x18220010);
    }
    ExModel::patchModelData(pModelData);
    ExModel::setLightMask(pModelData, GX_LIGHT1);
    mBodyModel.setModelData(pModelData);
    mBodyModel.setLODBias(TexLODControl::getCharacterLODBias());
    RCMGetCourse()->setFogInfo(&mBodyModel);
    pModelData->newSharedDisplayList(0x40000);
    pModelData->simpleCalcMaterial(j3dDefaultMtx);
    pModelData->makeSharedDL();

    J3DModelData *pLTireModelData;
    J3DModelData *pRTireModelData;
    void *pTire = ResMgr::getPtr(ResMgr::mcKartTire, mKartInfo->getKartID());
    if (pTire) {
        pLTireModelData = J3DModelLoaderDataBase::load(pTire, 0x18220010);
        pRTireModelData = nullptr;
    }
    else {
        pLTireModelData = J3DModelLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcKartTireLeft, mKartInfo->getKartID()), 0x18220010);
        pRTireModelData = J3DModelLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcKartTireRight, mKartInfo->getKartID()), 0x18220010);
    }
    ExModel::patchModelData(pLTireModelData);
    ExModel::patchModelData(pRTireModelData);
    ExModel::setLightMask(pLTireModelData, GX_LIGHT1);
    ExModel::setLightMask(pRTireModelData, GX_LIGHT1);

    J3DModelData *pArmModelData = J3DModelLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcKartDataArm, mKartInfo->getKartID()), 0x18220010);
    ExModel::patchModelData(pArmModelData);
    ExModel::setLightMask(pArmModelData, GX_LIGHT1);

    void *pShock = ResMgr::getPtr(ResMgr::mcKartDataShock, mKartInfo->getKartID());
    J3DModelData *pShockModel = nullptr;
    if (pShock) 
        pShockModel = J3DModelLoaderDataBase::load(pShock, 0x18220010);
    ExModel::patchModelData(pShockModel);
    ExModel::setLightMask(pShockModel, GX_LIGHT1);

    for (int i = 0; i < 6; i++) {
        J3DModelData *pTireModelData = !(i & 1) ? ((pRTireModelData) ? pRTireModelData : pLTireModelData) : pLTireModelData;
        if (i < mWheelNum) {
            mWheelModels[i].setModelData(pTireModelData);
            mArmModels[i].setModelData(pArmModelData);
            mShockModels[i].setModelData(pShockModel);
        }
    }

    mWheelModels[0].setLODBias(TexLODControl::getCharacterLODBias());
    mWheelModels[1].setLODBias(TexLODControl::getCharacterLODBias());
    mArmModels[0].setLODBias(TexLODControl::getCharacterLODBias());
    mShockModels[0].setLODBias(TexLODControl::getCharacterLODBias());
    RCMGetCourse()->setFogInfo(&mWheelModels[0]);
    RCMGetCourse()->setFogInfo(&mWheelModels[1]);
    RCMGetCourse()->setFogInfo(&mArmModels[0]);
    RCMGetCourse()->setFogInfo(&mShockModels[0]);

    pLTireModelData->newSharedDisplayList(0x40000);
    if (pRTireModelData)
        pRTireModelData->newSharedDisplayList(0x40000);
    pArmModelData->newSharedDisplayList(0x40000);
    if (pShockModel)
        pShockModel->newSharedDisplayList(0x40000);


    pLTireModelData->simpleCalcMaterial(j3dDefaultMtx);
    if (pRTireModelData)
        pRTireModelData->simpleCalcMaterial(j3dDefaultMtx);
    pArmModelData->simpleCalcMaterial(j3dDefaultMtx);
    if (pShockModel)
        pShockModel->simpleCalcMaterial(j3dDefaultMtx);

    pLTireModelData->makeSharedDL();
    if (pRTireModelData)
        pRTireModelData->makeSharedDL();
    pArmModelData->makeSharedDL();
    if (pShockModel)
        pShockModel->makeSharedDL();

    J3DModelData *pShadowModelData = J3DModelLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcShadowDataBmd, mKartInfo->getKartID()), 0x18220010);
    ExModel::patchModelData(pShadowModelData);
    mShadowModel.setModelData(pShadowModelData);
    J3DDeformData *pDeformData =(J3DDeformData *) J3DClusterLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcShadowDataBls, mKartInfo->getKartID()));
    J3DAnmCluster *pShadowAnmCluster = (J3DAnmCluster *)J3DAnmLoaderDataBase::load(ResMgr::getPtr(ResMgr::mcShadowDataBlk, mKartInfo->getKartID()), J3DLOADER_UNK_FLAG0);
    mShadowModel.setDeformData(pDeformData);
    pDeformData->offAllFlag(1);
    mShadowModel.setAnmCluster(pShadowAnmCluster);
    if (mDemoBmd == nullptr) {
        for (int i = 0; i < 2; i++)
            loadTevAnm(i);
    }
}

void KartLoader::patchTevRegKey(J3DModelData *data, J3DAnmTevRegKey *key) {
    u16 matNum = data->getMaterialNum();
    if (key->getCRegAnmNum() > matNum)
        key->setCRegAnmNum(matNum);

    if (key->getKRegAnmNum() > matNum)
        key->setKRegAnmNum(matNum);
}

void KartLoader::loadTevAnm(int effNo) {
#line 604
    JUT_MINMAX_ASSERT(0, effNo, 2);
    u32 efctIDTable[] = {
        0, 2
    };

    for (int i = 0; i < 2; i++) {
        const void *pCharAnm = GetJ3DEfctKartAnmMgr()->getSimpleCharBrkAnm(efctIDTable[effNo], 0);
        if (pCharAnm) {
            mDriverTevRegKey[i][effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pCharAnm, J3DLOADER_UNK_FLAG0);
            patchTevRegKey(mDriverModels[i]->getModelData(), mDriverTevRegKey[i][effNo]);
        }

        if (mOsageModel[i] && pCharAnm) {
            mAccessoryTevRegKey[i][effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pCharAnm, J3DLOADER_UNK_FLAG0);
            patchTevRegKey(mOsageModel[i]->getModelData(), mAccessoryTevRegKey[i][effNo]);
        }
    }

    const void *pKartAnm = GetJ3DEfctKartAnmMgr()->getSimpleKartBrkAnm(efctIDTable[effNo], 0);
    if (pKartAnm) {
        mBodyTevRegKey[effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pKartAnm, J3DLOADER_UNK_FLAG0);
        mArmTevRegKey[effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pKartAnm, J3DLOADER_UNK_FLAG0);
        mWheelLTevRegKey[effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pKartAnm, J3DLOADER_UNK_FLAG0);
        mWheelRTevRegKey[effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pKartAnm, J3DLOADER_UNK_FLAG0);
    
        if (pKartAnm) // ???
            mShockTevRegKey[effNo] = (J3DAnmTevRegKey *)J3DAnmLoaderDataBase::load(pKartAnm, J3DLOADER_UNK_FLAG0);
    }

    for (int i = 0; i < 2; i++) {
        if (mDriverTevRegKey[i][effNo])
            createMaterialAnmInUpdateMaterial(mDriverModels[i]->getModelData(), mDriverTevRegKey[i][effNo]);
        
        if (mAccessoryTevRegKey[i][effNo])
            createMaterialAnmInUpdateMaterial(mOsageModel[i]->getModelData(), mAccessoryTevRegKey[i][effNo]);
    }

    if (mBodyTevRegKey[effNo]) {
        createMaterialAnmInUpdateMaterial(mBodyModel.getModelData(), mBodyTevRegKey[effNo]);
        createMaterialAnmInUpdateMaterial(mArmModels[0].getModelData(), mArmTevRegKey[effNo]);
        createMaterialAnmInUpdateMaterial(mWheelModels[0].getModelData(), mWheelLTevRegKey[effNo]);
        createMaterialAnmInUpdateMaterial(mWheelModels[1].getModelData(), mWheelRTevRegKey[effNo]);

        if (isShockExist())
            createMaterialAnmInUpdateMaterial(mShockModels->getModelData(), mShockTevRegKey[effNo]);
    }
}

void KartLoader::createMaterialAnmInUpdateMaterial(J3DModelData *data, J3DAnmTevRegKey *key) {
    for (u16 i = 0; i < key->getCRegUpdateMaterialNum(); i++) {
        u16 updateID = key->getCRegUpdateMaterialID(i);
        if (updateID != 0xffff) {
            J3DMaterial *mat = nullptr;
            if (updateID < data->getMaterialNum()) {
                mat =data->getMaterialNodePointer(updateID);
            }
            createMaterialAnm(mat);
        }
    }

    for (u16 i = 0; i < key->getKRegUpdateMaterialNum(); i++) {
        u16 updateID = key->getKRegUpdateMaterialID(i);
        if (updateID != 0xffff) {
            J3DMaterial *mat = nullptr;
            if (updateID <  data->getMaterialNum()) {
                mat = data->getMaterialNodePointer(updateID);
            }
            createMaterialAnm(mat);
        }
    }
}
void KartLoader::createMaterialAnm(J3DMaterial *mat) {
    if (mat == nullptr)
        return;

    if (mat->getMaterialAnm() == nullptr) {
        J3DMaterialAnm *anm = new J3DMaterialAnm();
        mat->change();
        mat->setMaterialAnm(anm);
    }
}

void KartLoader::entryTevRegAnimator(int tevanmID) {
    if (isDriverTevAnmEnable(tevanmID)) {
        for (int i = 0 ; i < 2; i++) {
            if (getDriverTevRegKey(i, tevanmID)) {
                getExModelDriver(i)->getModelData()->entryTevRegAnimator(getDriverTevRegKey(i, tevanmID));
            }
            if (getAccessoryTevRegKey(i, tevanmID)) {
                mOsageModel[i]->getModelData()->entryTevRegAnimator(getAccessoryTevRegKey(i, tevanmID));
            }
        }
    }

    if (isKartTevAnmEnable(tevanmID)) {
        getExModelBody()->getModelData()->entryTevRegAnimator(getBodyTevRegKey(tevanmID));
        getExModelArm(0)->getModelData()->entryTevRegAnimator(getArmTevRegKey(tevanmID));
        getExModelWheel(0)->getModelData()->entryTevRegAnimator(getWheelLTevRegKey(tevanmID));
        getExModelWheel(1)->getModelData()->entryTevRegAnimator(getWheelRTevRegKey(tevanmID));

        if (isShockExist()) {
            getExModelShock(0)->getModelData()->entryTevRegAnimator(getShockTevRegKey(tevanmID));
        }
    }
}

void KartLoader::removeTevRegAnimator(int tevanmID) {
    if (isDriverTevAnmEnable(tevanmID)) {
        for (int i = 0 ; i < 2; i++) {
            if (getDriverTevRegKey(i, tevanmID)) {
                getExModelDriver(i)->getModelData()->removeTevRegAnimator(getDriverTevRegKey(i, tevanmID));
            }
            if (getAccessoryTevRegKey(i, tevanmID)) {
                mOsageModel[i]->getModelData()->removeTevRegAnimator(getAccessoryTevRegKey(i, tevanmID));
            }
        }
    }

    if (isKartTevAnmEnable(tevanmID)) {
        getExModelBody()->getModelData()->removeTevRegAnimator(getBodyTevRegKey(tevanmID));
        getExModelArm(0)->getModelData()->removeTevRegAnimator(getArmTevRegKey(tevanmID));
        getExModelWheel(0)->getModelData()->removeTevRegAnimator(getWheelLTevRegKey(tevanmID));
        getExModelWheel(1)->getModelData()->removeTevRegAnimator(getWheelRTevRegKey(tevanmID));

        if (isShockExist()) {
            getExModelShock(0)->getModelData()->removeTevRegAnimator(getShockTevRegKey(tevanmID));
        }
    }
}

void KartLoader::createModel(JKRSolidHeap *heap, u32 camNo) {
    for (int i = 0; i < 2; i++)
        mDriverModels[i]->createModel(heap, camNo, 0x80000);
    mShadowModel.createModel(heap, camNo, 0);
}

bool KartLoader::isCommonWheelModelData() {
    bool ret = false;
    if (mWheelModels[0].getModelData() == mWheelModels[1].getModelData())
        ret = true;
    return ret;
} 
