#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/gx/GXTev.h"
#include "dolphin/gx/GXTransform.h"
#include "dolphin/mtx.h"

#include "JSystem/J3D/J3DColorBlock.h"
#include "JSystem/J3D/J3DGXColor.h"
#include "JSystem/J3D/J3DJoint.h"
#include "JSystem/J3D/J3DMaterial.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/J3D/J3DPacket.h"
#include "JSystem/J3D/J3DPE.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/J3D/J3DTexMtx.h"
#include "JSystem/J3D/J3DTexture.h"
#include "JSystem/J3D/J3DTransform.h"
#include "JSystem/J3D/J3DTypes.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/ResTIMG.h"

//#pragma sym on
#include "Kaneshige/ExModel.h"

#include "Kaneshige/DrawBuffer.h"
#include "Kaneshige/ShapePktControl.h"
#include "Kaneshige/SimpleDrawer.h"

#include "types.h"

bool ExModel::sMtxCombinationOn;
bool ExModel::sDrawingOnlyUpdZMat;
bool ExModel::sClippingOn = true;
const f32 ExModel::sExModelInvalidLODBias = -1000.0f;
ExMdlDataMgr *ExMdlDataMgr::sManager;

ExMdlDataMgr::ExMdlDataMgr(bool mirror) {
    sManager = this;
    mIsMirror = mirror;
    mRecordList.initiate();
}

ExMdlDataMgr::~ExMdlDataMgr() {
    sManager = nullptr;
}

void ExMdlDataMgr::entryModelData(J3DModelData *mdlData) {
    if (searchRecord(mdlData) == nullptr) {
        ExMDRecord *mdlRecord = new ExMDRecord(mdlData);
        mdlRecord->patch(mIsMirror);
        mRecordList.append((JSULink<ExMDRecord> *)mdlRecord);
    }
}

ExMDRecord *ExMdlDataMgr::searchRecord(J3DModelData *mdlData) {
    ExMDRecord *record = nullptr;
    for (JSUListIterator<ExMDRecord> it(mRecordList.getFirst()); it != mRecordList.getEnd(); it++) {
        if (it->mModelData == mdlData) {
            record = it.getObject();
            break;
        }
    }
    return record;
}

ExMDRecord::ExMDRecord(J3DModelData *mdlData) : mRecordLink(this) {
    mModelData = mdlData;
}

void ExMDRecord::patch(bool mirror) {
    J3DModelData *modelData = mModelData;
    if (mirror)
        ExModel::reverseCullMode(modelData);
}

ExModel::ExModel(u16 level) {
    mMagic = 'EXMD';
    _14 = 0;
    _18 = 0;
    mLevelCnt = level;
    _1a = 0;
    mSimpleTevReg = 1;
    mModelData = new J3DModelData*[level];
    mModel = new J3DModel*[level];

    for (u16 i = 0 ; i < level; i++) {
        mModelData[i] = nullptr;
        mModel[i] = nullptr;
    }

    mScale.setAll(1.0f);
    PSMTXIdentity(mBaseTRMtx);
    PSMTXIdentity(_5c);
}

ExModel::~ExModel() {
    delete mModelData;
    delete mModel;
}

void ExModel::setModelData(J3DModelData *mdlData) {
    setModelData(0, mdlData);
}

void ExModel::setModelData(u16 level, J3DModelData *mdlData) {
    if (level >= mLevelCnt)
        return;

    mModelData[level] = mdlData;
    if (mdlData) {
#line 653
        JUT_ASSERT_MSG(tstConcatViewMode(), "J3DModelData NOT CONCAT VIEW MODE")
    }

    if (level != 0 && mdlData) {
        u16 num1 = mModelData[0]->getJointNum();
        u16 num2 = mdlData->getJointNum();
#line 660
        JUT_ASSERT_F(num1 == num2, "JOINT NUM DIFF %d:%d", num1, num2);
    }
}


void ExModel::patchModelData(J3DModelData *mdlData) {
    if (mdlData) {
        ExMdlDataMgr::getManager()->entryModelData(mdlData);
    }
}

void ExModel::setLODBias(f32 rate) {
    for (u16 i = 0; i < mLevelCnt; i++) {
        if (mModelData[i]) {
            setLODBias(mModelData[i], rate);
        }
    }
}

void ExModel::setLODBias(J3DModelData *mdlData, f32 rate) {
    if (rate != sExModelInvalidLODBias) {
        J3DTexture *tex = mdlData->getTexture();
        for (u16 i = 0; i < tex->getNum(); i++) {
            tex->getResTIMG(i)->mLODBias = rate * 100.0f;
        }

    }
}

void ExModel::disableTexLOD(J3DModelData *mdlData) {
    J3DTexture *tex = mdlData->getTexture();
    for (u16 i = 0; i < tex->getNum(); i++) {
        ResTIMG *img = tex->getResTIMG(i);
        img->mMinFilterType = 1;
        img->mMagFilterType = 1;
        img->mIsMIPmapEnabled = GX_FALSE;
    }
}

void ExModel::setFogInfo(J3DModelData *mdlData, u8 type, f32 start, f32 end, f32 near, f32 far, JUTColor *color) {
    if (mdlData) {
        for (u16 i = 0; i < mdlData->getMaterialNum(); i++) {
            J3DMaterial *node = mdlData->getMaterialNodePointer(i);
            J3DFog *fog = node->getPEBlock()->getFog();

            if (fog->getType() != 0) {
                fog->setType(type);
                fog->setStartZ(start);
                fog->setEndZ(end);
                fog->setNearZ(near);
                fog->setFarZ(far);
                fog->setFogColor(*color);

            }
        }
    }    
}

void ExModel::reverseCullMode(J3DModelData *mdlData) {
    if (mdlData) {
        for (u16 i = 0; i < mdlData->getMaterialNum(); i++) {
            J3DMaterial *node = mdlData->getMaterialNodePointer(i);
            u8 cullMode = node->getColorBlock()->getCullMode();
            if (cullMode == 1) {
                node->getColorBlock()->setCullMode(2);
            }
            else if (cullMode == 2) {
                node->getColorBlock()->setCullMode(1);
            }            
        }
    }
}

void ExModel::setLightMask(J3DModelData *mdlData, GXLightID lightId) {
    if (mdlData) {
        for (u16 i = 0; i < mdlData->getMaterialNum(); i++) {
            J3DMaterial *node = mdlData->getMaterialNodePointer(i);
            J3DColorBlock *colorBlock = node->getColorBlock();
            u32 numChans = colorBlock->getColorChanNum();

            for (u32 i = 0; i < numChans; i++) {
                colorBlock->getColorChan(i)->setLightMask(lightId);
            }
        }
    }
}

void ExModel::setTevColor(J3DModelData *mdlData, u32 index, const J3DGXColorS10 &color) {
    if (mdlData) {
        for (u16 i = 0; i < mdlData->getMaterialNum(); i++) {
            J3DMaterial *node = mdlData->getMaterialNodePointer(i);
            node->getTevBlock()->setTevColor(index, color);
        }
    }
}

void ExModel::setTevColor(u32 index, const J3DGXColorS10 &color) {
    setTevColor(getModelData(), index, color);
}

bool ExModel::createModel(JKRSolidHeap *slid, u32 p2, u32 size) {
    bool ret = true;
    JKRHeap *curHeap = slid->becomeCurrentHeap();

    for (u16 i = 0; i < mLevelCnt; i++) {
        if (mModelData[i]) {
            mModel[i] = new J3DModel(mModelData[i], size, 1);

            int numJoints = mModelData[i]->getJointNum();
            for (int j = 0; j < numJoints; j++) {
                PSMTXIdentity(mModel[i]->getAnmMtx(j));
            }
            mModel[i]->setUserArea((u32)this);
            setShapePktControl(i);
        }

        if (mModel[i] == nullptr) {
            ret = false;
        }
    }

    curHeap->becomeCurrentHeap();
    return ret;
}

bool ExModel::createDifferedModel(JKRSolidHeap *slid, u32 p2, u32 p3, bool p4) {
    bool ret = ExModel::createModel(slid, p2, 0x80000);

    for (u16 i = 0; i < mLevelCnt; i++) {
        if (mModelData[i]) {
            mModel[i]->newDifferedDisplayList(p3);
            if (p4)
                mModel[i]->newDifferedTexMtx(TEXDIFF_Material);
        }
    }
    return ret;
}

void ExModel::makeSharedDL() {
    for (u16 i = 0; i < mLevelCnt; i++) {
        J3DModel *mdl = mModel[i];
        if (mdl) {
            mdl->calc();
            mdl->calcMaterial();
            mdl->makeDL();
        }
    }
}

void ExModel::lock() {
    for (u16 i = 0; i < mLevelCnt; i++) {
        if (mModel[i]) {
            mModel[i]->lock();
        }
    }
}

void ExModel::setShapePktControl(u16 level) {
#line 1028
    JUT_MINMAX_ASSERT(0, level, mLevelCnt);

    u16 numShapes = mModelData[level]->getShapeNum();
    if (numShapes != 0) {
        ShapePktControl *pShapeCtrl = new ShapePktControl[numShapes];
        for (u16 i = 0; i < numShapes; i++) {
            ShapePktControl *shapeCtrl = &pShapeCtrl[i];
            J3DShapePacket *shpPkt = mModel[level]->getShapePacket(i);
            shapeCtrl->setShapePacket(shpPkt);
            shpPkt->setUserArea((u32)shapeCtrl);
        }
    }
}

bool ExModel::isAllShapePacketHidding(u16 level, u32 viewNo) {
    bool ret = false;
#line 1053
    JUT_MINMAX_ASSERT(0, level, mLevelCnt);
    J3DModel *mdl = mModel[level];
    if (mdl) {
        ret = true;
        u16 numShapes = mModelData[level]->getShapeNum();
        for (u16 i = 0; i < numShapes; i++) {
            ShapePktControl *shapeCtrl = (ShapePktControl *)mdl->getShapePacket(i)->getUserArea();
#line 1064
            JUT_ASSERT(shapeCtrl->isValid());
            if (!shapeCtrl->isHidding(viewNo)) {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

void ExModel::setCurMtx() {
    if (mModel[0]) {
        PSMTXCopy(mModel[0]->getBaseTRMtx(), mBaseTRMtx);
    }
}

void ExModel::calc() {
    calc(0);
}

void ExModel::calc(u16 level) {
    if (level < mLevelCnt) {
        if (mModel[level]) {
            if (level == 0) {
                mModel[level]->calc();
            }
            else {
                J3DModel *mdl0 = mModel[0];
                J3DModel *mdl = mModel[level];
                
                for (int i = 0; i < mdl->getModelData()->getJointNum(); i++) {
                    PSMTXCopy(mdl0->getAnmMtx(i), mdl->getAnmMtx(i));
                }
                for (int i = 0; i < mdl->getModelData()->getWEvlpMtxNum(); i++) {
                    PSMTXCopy(mdl0->getWeightAnmMtx(i), mdl->getWeightAnmMtx(i));
                }
            }   
        }
    }
    if (level == 0) {
        setCurMtx();
    }

}

void ExModel::update(u16 level) {
    J3DModel *mdl = mModel[level];

    if (level < mLevelCnt && mdl) {
        if (level == 0) {
            mdl->calc();
        }
        else {
            J3DModel *mdl0 = mModel[0];

            for (int i = 0; i < mdl->getModelData()->getJointNum(); i++) {
                PSMTXCopy(mdl0->getAnmMtx(i), mdl->getAnmMtx(i));
            }
            for (int i = 0; i < mdl->getModelData()->getWEvlpMtxNum(); i++) {
                PSMTXCopy(mdl0->getWeightAnmMtx(i), mdl->getWeightAnmMtx(i));
            }

            J3DGXColorS10 *tevColor = mModelData[0]->getMaterialNodePointer(0)->getTevBlock()->getTevColor(0);
            if (tevColor) {
                ExModel::setTevColor(mModelData[1], 0, *tevColor);
            }
        }

        if (mdl->getDiffFlag() != 0) {
            mdl->calcMaterial();
            mdl->diff();
        }
        
        if (DrawBuffer::isValidDrawBuffer(j3dSys.getDrawBuffer(0))) {
            mdl->entry();
        }
        mdl->prepareShapePackets();

        if (tstDiffTexMtxMode()) {
            mdl->calcDiffTexMtx();
        }
        
    }

    if (level == 0) {
        setCurMtx();
    }   
}

void ExModel::setCurrentViewNo(u32 viewNo) {
    for (u16 i = 0; i < mLevelCnt; i++) {
        J3DModel *mdl = mModel[i];
        if (mdl) {
            u16 numShapes = mModelData[i]->getShapeNum();
            for (u16 j = 0; j < numShapes; j++) {
                ShapePktControl *shapeCtrl = (ShapePktControl *)mdl->getShapePacket(j)->getUserArea();
#line 1280
                JUT_ASSERT(shapeCtrl->isValid());
                shapeCtrl->ctrlPacket(viewNo);
            }
        }
    }
}

void ExModel::calcBBoardMtx(Mtx dst, Mtx src) {
    JGeometry::TVec3f srcX(src[0][0], src[0][1], src[0][2]);
    JGeometry::TVec3f srcY(src[1][0], src[1][1], src[1][2]);
    JGeometry::TVec3f srcZ(src[2][0], src[2][1], src[2][2]);
    
    JGeometry::TVec3f srcScale;
    srcScale.zero();

    srcScale.scaleAdd(-src[0][3], srcX, srcScale);
    srcScale.scaleAdd(-src[1][3], srcY, srcScale);
    srcScale.scaleAdd(-src[2][3], srcZ, srcScale);

    JGeometry::TVec3f dstScale(dst[0][3], dst[1][3], dst[2][3]);

    JGeometry::TVec3f dirZ;
    dirZ.sub(srcScale, dstScale);
    dirZ.normalize();

    JGeometry::TVec3f dirX;
    dirX.cross(srcY, dirZ);
    dirX.normalize();

    JGeometry::TVec3f dirY;
    dirY.cross(dirZ, dirX);
    
    dst[0][0] = dirX.x;
    dst[1][0] = dirX.y;
    dst[2][0] = dirX.z;
    dst[0][1] = dirY.x;
    dst[1][1] = dirY.y;
    dst[2][1] = dirY.z;
    dst[0][2] = dirZ.x;
    dst[1][2] = dirZ.y;
    dst[2][2] = dirZ.z;
}

void ExModel::simpleDraw(u32 viewNo, Mtx m, u32 p3) {
    J3DModelData *mdlData0 = mModelData[0];

#line 1345
    JUT_ASSERT_MSG(mModel[0] == nullptr, "CREATE J3DMODEL AND CALL SIMPLEDRAW");
    SimpleDrawer *drawer = SimpleDrawer::getDrawingDrawer();
    if (_1a & (1 << viewNo)) {
        return;
    }

    if (drawer->isPhaseCheckHidding()) {
        drawer->enableDrawMaterial();
        return;
    }

    J3DMaterial *mat = drawer->getDrawingMaterial();
    if (sDrawingOnlyUpdZMat) {
        if (mat->getPEBlock()->getZMode()->getUpdateEnable() != 1)
            return;
    }

    if (tstKartBBoardOn()) {
        switch (mdlData0->getJointNodePointer(0)->getMtxType()) {
        case 1:
            calcBBoardMtx(mBaseTRMtx, j3dSys.getViewMtx());
            break;
        }
    }

    Mtx m1, m2, m3;
    PSMTXScale(m2, mScale.x, mScale.y, mScale.z);
    PSMTXConcat(mBaseTRMtx, m2, m3);
    PSMTXConcat(j3dSys.getViewMtx(), m3, m1);


    switch (mdlData0->getJointNodePointer(0)->getMtxType()) {
    case 1:
        if (!tstKartBBoardOn())  {
            J3DCalcBBoardMtx(m1);
        }
        break;
    case 2:
        J3DCalcYBBoardMtx(m1);
        break;
    }

    if (mat->getMaterialAnm()) {
        mat->getMaterialAnm()->calc(mat);
    }

    mat->calc(m3);

    J3DTexMtx* texMtx = mat->getTexMtx(p3);
    
    if (texMtx) {
        Mtx m4;
        u8 info = texMtx->getTexMtxInfo().mInfo;
        bool b = false;
        switch (info & 0x3f) {
        case 6:
        case 7: {
            
            PSMTXCopy(m1, m4);
            m4[0][3] = 0.0f;
            m4[1][3] = 0.0f;
            m4[2][3] = 0.0f;
            texMtx->calc(m4);
            b = true;
            break;
        }
        case 5:
        case 10:
        case 11: {
            if (m) {
                PSMTXCopy(m3, m4);
                m4[0][3] = 0.0f;
                m4[1][3] = 0.0f;
                m4[2][3] = 0.0f;
                texMtx->setEffectMtx(m);
                texMtx->calc(m4);
                b = true;
            }
            break;
        }         
        }

        if (b) {
             GXLoadTexMtxImm(texMtx->getMtx(), p3 * 3 + 30, GX_MTX3x4);
        }       
    }

    if (mat->getMaterialAnm()) {
        if (mat->getMaterialAnm()->getTexNoAnm(0).getAnmTexPattern()) {
            u16 texNo = mat->getTevBlock()->getTexNo(0);
            mModelData[0]->getTexture()->loadGX(texNo, GX_TEXMAP0);
        }

        for (u32 i = 0; i < 2; i++) {
            if (mat->getMaterialAnm()->getTexMtxAnm(i).getAnmTransform()) {
                J3DTexMtx* texMtx = mat->getTexGenBlock()->getTexMtx(i);
                if (texMtx) {
                    GXLoadTexMtxImm(texMtx->getMtx(), (i * 3) + 30, (GXTexMtxType)texMtx->getTexMtxInfo().mProjection);
                }
            }
        }

        if (mat->getMaterialAnm()->getTevKColorAnm(0).getAnmTevReg()) {
            J3DGXColor *kColor = mat->getTevBlock()->getTevKColor(0);
            GXSetTevKColor(GX_KCOLOR0, *kColor);
        }
    }

    for (u32 i = 0; i < 3; i++) {
        GXTevRegID regIDs[] = {
            GX_TEVREG0, GX_TEVREG1, GX_TEVREG2
        };
        if (tstSimpleTevReg(i)) {
            GXColorS10* tevColor = mat->getTevBlock()->getTevColor(i);
            if (tevColor) {
                GXSetTevColorS10(regIDs[i], *tevColor);
            }
        }
    }

    GXLoadPosMtxImm(m1, 0);
    GXLoadNrmMtxImm(m1, 0);
    mat->getShape()->simpleDrawCache();
}

void ExModel::show() {
    for (u16 i = 0; i < mLevelCnt; i++) {
        if (mModel[i]) {
            u16 numShapes = mModelData[i]->getShapeNum();
            for (u16 j = 0; j < numShapes; j++) {
                J3DShapePacket *shpPkt = mModel[i]->getShapePacket(j);
                ShapePktControl *shapeCtrl = (ShapePktControl *)shpPkt->getUserArea();
#line 1527
                JUT_ASSERT(shapeCtrl->isValid());
                shapeCtrl->show();
                shpPkt->show();                
            }
        }
        else {
            _1a = 0;
        }
    }
}

void ExModel::hide() {
    for (u16 i = 0; i < mLevelCnt; i++) {
        if (mModel[i]) {
            u16 numShapes = mModelData[i]->getShapeNum();
            for (u16 j = 0; j < numShapes; j++) {
                J3DShapePacket *shpPkt = mModel[i]->getShapePacket(j);
                ShapePktControl *shapeCtrl = (ShapePktControl *)shpPkt->getUserArea();
#line 1559
                JUT_ASSERT(shapeCtrl->isValid());
                shapeCtrl->hide();
                shpPkt->hide();                
            }
        }
        else {
            _1a = 0xffff;
        }
    }
}

void ExModel::clipAll(u32 p1, bool showPkt) {
    if (isAvailable()) {
        for (u16 i = 0; i < getModelData()->getJointNum(); i++) {
            clipJoint(p1, i, showPkt);
        }
    }
}

void ExModel::clipBySphere(u32 viewNo, J3DUClipper *clipper, const Mtx m, f32 r) {
    J3DModelData *mdlData0 = mModelData[0];

    if (mdlData0) {
        if (mModel[0]) {
            for (u16 i = 0; i < mdlData0->getJointNum(); i++) {
                int doClip = 0;
                if (sClippingOn) {
                    if (mdlData0->getJointNodePointer(i)->getKind() != 0)
                        continue;

                    JGeometry::TVec3f point;
                    point.x = mModel[0]->getAnmMtx(i)[0][3];
                    point.y = mModel[0]->getAnmMtx(i)[1][3];
                    point.z = mModel[0]->getAnmMtx(i)[2][3];
                    doClip = clipper->clip(m, point, mdlData0->getJointNodePointer(i)->getRadius() * r);
                }
                
                clipJoint(viewNo, i, doClip == 0);
            }
        }
        else {
            int clipped = 0;
            if (sClippingOn) {
                JGeometry::TVec3f point;
                point.x = mBaseTRMtx[0][3];
                point.y = mBaseTRMtx[1][3];
                point.z = mBaseTRMtx[2][3];
                clipped = clipper->clip(m, point, mdlData0->getJointNodePointer(0)->getRadius() * r);
            }

            if (clipped == 0) {
                _1a &= ~(1 << viewNo);
            }
            else {
                _1a |= 1 << viewNo;
            }

        }
    }
}

void ExModel::clipJoint(u32 viewNo, u16 nodeIdx, bool showPkt) {
    if (isAvailable()) {
        for (u16 i = 0; i < mLevelCnt; i++) {            
            if (mModel[i]) {
                J3DModel *mdl = mModel[i];
                J3DJoint *joint = mdl->getModelData()->getJointNodePointer(nodeIdx);
                if (joint->getKind() != 0) {
                    break;
                }

                for (J3DMaterial *mat = joint->getMesh(); mat != nullptr; mat = mat->getNext()) {
                    u16 idx = mat->getShape()->getIndex();
                    ShapePktControl *shpCtrl = (ShapePktControl *)mdl->getShapePacket(idx)->getUserArea();
#line 1726
                    JUT_ASSERT(shpCtrl->isValid());

                    if (showPkt) {
                        shpCtrl->show(viewNo);
                    }
                    else {
                        shpCtrl->hide(viewNo);
                    }
                }
            }
            else if (showPkt) {
                _1a &= ~(1 << viewNo);
            }
            else {
                _1a |= 1 << viewNo;
            }
        }
    }
}

void ExModel::setBaseTRMtx(Mtx m) {
    if (isAvailable()) {
        for (u16 i = 0; i < mLevelCnt; i++) {
            J3DModel *mdl = mModel[i];
            if (mdl) {
                mdl->setBaseTRMtx(m);
            }
            else {
                PSMTXCopy(m, mBaseTRMtx);
            }
        }
    }
}

void ExModel::setBaseScale(JGeometry::TVec3f &scale) {
    if (isAvailable()) {
        for (u16 i = 0; i < mLevelCnt; i++) {
            J3DModel *mdl = mModel[i];
            if (mdl) {
                mdl->setBaseScale(scale);
            }
            else {
                mScale.set(scale);
            }
        }
    }
}

bool ExModel::tstDiffTexMtxMode() {
    bool ret = false;
    if (mModel[0]->getShapePacket(0)->getTexMtxObj())
        ret = true;
   return ret;
}

void ExModel::setEffectMtx(Mtx m, u32 p2) {
    for (u16 i = 0; i < mLevelCnt; i++) { 
        J3DModel *mdl = mModel[i];
        if (mdl) {
            u16 numShapes = mdl->getModelData()->getShapeNum();

            for (u16 j = 0; j < numShapes; j++) {
                J3DShapePacket *pkt = mdl->getShapePacket(j);
                J3DTexMtx *texMtx = pkt->getShape()->getMaterial()->getTexMtx(p2);
                if (texMtx) {
                    switch (texMtx->getTexMtxInfo().mInfo & 0x3f) {
                        case 0x5:
                        case 0xb: {
                            pkt->getTexMtxObj()->setEffectMtx(p2, *(Mtx *)m);
                        }
                    }
                }
            }
        }
    }
}
