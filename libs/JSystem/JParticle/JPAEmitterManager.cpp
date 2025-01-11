#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAList.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTAssert.h"

JPAEmitterManager::JPAEmitterManager(u32 ptclNum_, u32 emtrNum_, JKRHeap *heap, u8 groups, u8 resnum) {
    emtrNum = emtrNum_;
    ptclNum = ptclNum_;
    gidMax  = groups;
    ridMax  = resnum;
#line 40
    JUT_ASSERT(emtrNum && ptclNum && gidMax && ridMax);

#line 43
    JPABaseEmitter *p_emtr_link = new (heap, 0) JPABaseEmitter[emtrNum];
    JUT_ASSERT(p_emtr_link);
    for (int i = 0; i < emtrNum; i++) {
        mFreeEmtrList.prepend(&p_emtr_link[i].mLink);
    }

#line 50
    JPANode<JPABaseParticle> *p_ptcl_node = new (heap, 0) JPANode<JPABaseParticle>[ptclNum];
    JUT_ASSERT(p_ptcl_node);
    for (int i = 0; i < ptclNum; i++) {
        mPtclPool.push_back(&p_ptcl_node[i]);
    }

#line 57
    pEmtrUseList = new (heap, 0) JSUList<JPABaseEmitter>[gidMax];
    JUT_ASSERT(pEmtrUseList);
#line 61
    pResMgrAry   = new (heap, 0) JPAResourceManager*[ridMax];
    JUT_ASSERT(pResMgrAry);
    for (int i = 0; i < ridMax; i++) {
        pResMgrAry[i] = nullptr;
    }
#line 66
    pWd = new (heap, 0) JPAEmitterWorkData;
    JUT_ASSERT(pWd);
}

JPABaseEmitter *JPAEmitterManager::createSimpleEmitterID(const JGeometry::TVec3f &pos, u16 resID, u8 group_id, u8 res_mgr_id,
                                                         JPAEmitterCallBack *ecback, JPAParticleCallBack *pcback) {
#line 88
    JUT_ASSERT(group_id < gidMax);
    JUT_ASSERT(res_mgr_id < ridMax);
    JUT_ASSERT(pResMgrAry[res_mgr_id] != 0);
    JPAResource *res = pResMgrAry[res_mgr_id]->getResource(resID);

    if (res == nullptr) {
        JUT_WARNING_F3("JPA : User Index %d is NOT exist\n", resID);
    }
    else if (mFreeEmtrList.getNumLinks() == 0) {
        JUT_WARNING_F3("JPA : Can NOT create emitter more\n");
    }
    else {
        JSULink<JPABaseEmitter> *link = mFreeEmtrList.getFirst();
        mFreeEmtrList.remove(link);
        pEmtrUseList[group_id].append(link);

        JPABaseEmitter *emit = link->getObject();
        emit->init(this, res);
        emit->mPtclPool         = &mPtclPool;
        emit->mEmitterCallback  = ecback;
        emit->mParticleCallback = pcback;
        emit->mGroupID          = group_id;
        emit->mResMgrID         = res_mgr_id;
        emit->mGlobalTrs.set(pos);
        return emit;
    }

    return nullptr;
}

JPABaseEmitter *JPAEmitterManager::createSimpleEmitter(const JGeometry::TVec3f &pos, u16 resID, JPAEmitterCallBack *ecback, JPAParticleCallBack *pcback) {
    return createSimpleEmitterID(pos, resID, 0, 0, ecback, pcback);
}

void JPAEmitterManager::calc(u8 group_id) {
#line 154
    JUT_ASSERT(group_id < gidMax);
    JSULink<JPABaseEmitter> *link = pEmtrUseList[group_id].getFirst();
    while (link != pEmtrUseList[group_id].getEnd()) {
        JSULink<JPABaseEmitter> *nextLink = link->getNext();
        JPABaseEmitter *emit      = link->getObject();
        link      = nextLink;
        bool stat = emit->mResource->calc(pWd, emit);
        if (stat && !emit->checkStatus(JPAEMIT_Unk10)) {
            forceDeleteEmitter(emit);
        }
    }
}

void JPAEmitterManager::calc() {
    for (u8 i = 0; i < gidMax; i++) {
        calc(i);
    }
}

void JPAEmitterManager::draw(const JPADrawInfo *info, u8 group_id) {
#line 192
    JUT_ASSERT(group_id < gidMax);
    PSMTXCopy(info->mCamMtx, pWd->mPosCamMtx);
    PSMTXCopy(info->mPrjMtx, pWd->mPrjMtx);
    calcYBBCam();
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_TRUE, GX_TRUE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_TRUE, GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetCoPlanar(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetCurrentMtx(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(0);

    JSULink<JPABaseEmitter> *link = pEmtrUseList[group_id].getFirst();
    for (; link != pEmtrUseList[group_id].getEnd(); link = link->getNext()) {
        JPABaseEmitter *emit = link->getObject();
        if (!emit->checkStatus(JPAEMIT_StopDraw)) {
            pWd->mResourceMgr = pResMgrAry[emit->mResMgrID];
            emit->mResource->draw(pWd, emit);
        }
    }
}

void JPAEmitterManager::draw(Mtx m, u8 group_id)
{
    // UNUSED FUNCTION
}

void JPAEmitterManager::draw(const JPADrawInfo *drawInfo) {
    for (u8 i = 0; i < gidMax; i++) {
        draw(drawInfo, i);
    }
}

void JPAEmitterManager::draw(Mtx m)
{
    // UNUSED FUNCTION
}

void JPAEmitterManager::forceDeleteAllEmitter() {
    for (u8 i = 0; i < gidMax; i++) {
        forceDeleteGroupEmitter(i);
    }
}

void JPAEmitterManager::forceDeleteGroupEmitter(u8 group_id) {
#line 288
    JUT_ASSERT(group_id < gidMax);

    while (pEmtrUseList[group_id].mLinkCount) {
        forceDeleteEmitter((JPABaseEmitter*)pEmtrUseList[group_id].mTail->getObjectPtr());
    }
}

void JPAEmitterManager::forceDeleteEmitter(JPABaseEmitter *emit) {
    emit->deleteAllParticle();
    emit->setFlag(JPAEMIT_ForceDelete | JPAEMIT_Unk10);
    pEmtrUseList[emit->mGroupID].remove(&emit->mLink);
    mFreeEmtrList.prepend(&emit->mLink);
}

void JPAEmitterManager::entryResourceManager(JPAResourceManager *mgr, u8 grpID) {
#line 325
    JUT_ASSERT_F(grpID < ridMax && !pResMgrAry[grpID], "res_id %d res_id_max %d array[%d] = %x", grpID, ridMax, grpID, pResMgrAry[grpID]);
    pResMgrAry[grpID] = mgr; 
}

void JPAEmitterManager::clearResourceManager(u8 res_mgr_id) {
    JUT_ASSERT(res_mgr_id < ridMax);

    for (u8 i = 0; i < gidMax; i++) {
        for (JSULink<JPABaseEmitter> *link = pEmtrUseList[i].getFirst(); link != pEmtrUseList[i].getEnd(); ) {
            JSULink<JPABaseEmitter> *next = link = link->getNext();
            if (res_mgr_id == link->getObject()->getResourceManagerID()) {
                forceDeleteEmitter(link->getObject());
            }
            link = next;
        }
    }
    pResMgrAry[res_mgr_id] = nullptr;
}

void JPAEmitterManager::calcYBBCam() {
#line 366
    JGeometry::TVec3f v(0.0f, pWd->mPosCamMtx[1][1], pWd->mPosCamMtx[2][1]);
    JUT_ASSERT(!v.isZero());
    v.normalize();
    pWd->mYBBCamMtx[0][0] = 1.0f;
    pWd->mYBBCamMtx[0][1] = 0.0f;
    pWd->mYBBCamMtx[0][2] = 0.0f;
    pWd->mYBBCamMtx[0][3] = pWd->mPosCamMtx[0][3];

    pWd->mYBBCamMtx[1][0] = 0.0f;
    pWd->mYBBCamMtx[1][1] = v.y;
    pWd->mYBBCamMtx[1][2] = -v.z;
    pWd->mYBBCamMtx[1][3] = pWd->mPosCamMtx[1][3];

    pWd->mYBBCamMtx[2][0] = 0.0f;
    pWd->mYBBCamMtx[2][1] = v.z;
    pWd->mYBBCamMtx[2][2] = v.y;
    pWd->mYBBCamMtx[2][3] = pWd->mPosCamMtx[2][3];
}
