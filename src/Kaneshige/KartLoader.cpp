#include "Kaneshige/KartLoader.h"
#include "Kaneshige/SysDebug.h"
#include "Kawano/osage.h"
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
        mOsagModel[i] = OsageMgr::ptr()->IsOsageExist(mDriverModels[i]);
    }
}

//void KartInfo::getWheelNumber() const {}

//void KartShadowModel::KartShadowModel() {}

//void ShadowModel::ShadowModel(unsigned char, ShadowModel::ShadowKind, unsigned char) {}

//void ViewCtrlModel::ViewCtrlModel(unsigned char) {}

void KartLoader::reset() {
    for(int i = 0; i < 2; i++) {
        mDriverModels[i]->restart(i);
    }
}

void KartLoader::load() {}

//void KartShadowModel::setAnmCluster(J3DAnmCluster *) {}

//void KartShadowModel::setDeformData(J3DDeformData *) {}

//void TexLODControl::getCharacterLODBias() {}

//void KartInfo::getKartID() const {}

void KartLoader::patchTevRegKey(J3DModelData *data, J3DAnmTevRegKey *key) {
    u16 matNum = data->getMaterialNum();
    if (key->getCRegAnmNum() > matNum)
        key->setCRegAnmNum(matNum);

    if (key->getKRegAnmNum() > matNum)
        key->setKRegAnmNum(matNum);
}

//void J3DAnmTevRegKey::setKRegAnmNum(unsigned short) {}

//void J3DAnmTevRegKey::getKRegAnmNum() const {}

//void J3DAnmTevRegKey::setCRegAnmNum(unsigned short) {}

//void J3DAnmTevRegKey::getCRegAnmNum() const {}

void KartLoader::loadTevAnm(int effNo) {
    JUT_MINMAX_ASSERT(0, effNo, 2);
}

//void KartLoader::isShockExist() const {}

//void GetJ3DEfctKartAnmMgr() {}

//void J3DEfctKartAnmMgr::getMgr() {}

void KartLoader::createMaterialAnmInUpdateMaterial(J3DModelData *data, J3DAnmTevRegKey *key) {

}

//void J3DAnmTevRegKey::getKRegUpdateMaterialNum() const {}

//void J3DAnmTevRegKey::getKRegUpdateMaterialID(unsigned short) const {}

//void J3DAnmTevRegKey::getCRegUpdateMaterialNum() const {}

//void J3DAnmTevRegKey::getCRegUpdateMaterialID(unsigned short) const {}

void KartLoader::createMaterialAnm(J3DMaterial *mat) {
    if (mat == nullptr)
        return;

    if (mat->getMaterialAnm() == nullptr) {
        J3DMaterialAnm *anm = new J3DMaterialAnm();
        mat->change();
        mat->setMaterialAnm(anm);
    }
}

//void J3DMaterial::setMaterialAnm(J3DMaterialAnm *) {}

//void J3DMaterialAnm::J3DMaterialAnm() {}

void KartLoader::entryTevRegAnimator(int) {}

//void KartLoader::getShockTevRegKey(int) {}

//void KartLoader::getExModelShock(int) {}

//void KartLoader::getWheelRTevRegKey(int) {}

//void KartLoader::getWheelLTevRegKey(int) {}

//void KartLoader::getExModelWheel(int) {}

//void KartLoader::getArmTevRegKey(int) {}

//void KartLoader::getExModelArm(int) {}

//void KartLoader::getBodyTevRegKey(int) {}

//void KartLoader::getExModelBody() {}

//void KartLoader::isKartTevAnmEnable(int) {}

//void KartLoader::getAccessoryTevRegKey(int, int) {}

//void J3DModelData::entryTevRegAnimator(J3DAnmTevRegKey *) {}

//void KartLoader::getExModelDriver(int) {}

//void KartLoader::getDriverTevRegKey(int, int) {}

//void KartLoader::isDriverTevAnmEnable(int) {}

void KartLoader::removeTevRegAnimator(int) {}

//void J3DModelData::removeTevRegAnimator(J3DAnmTevRegKey *) {}

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
