#include "Osako/shadowMgr.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/J3D/J3DColorBlock.h"
#include "Dolphin/os.h"
#include "Dolphin/gx.h"

ShadowManager* ShadowManager::mspShadowManager = nullptr;

ShadowManager::ShadowManager() : mShadowColor(0x00, 00, 0x14, 0x50)
{
    mDepthMode = 0;
    _5f = 0;
    mIsMirror = false;
    mDepth[0] = 0x40;
    mDepth[1] = 0x18;
}

ShadowManager::List &ShadowManager::getShadowModelList(ShadowModel::ShadowKind shadowKind)
{
    switch (shadowKind) {
    case ShadowModel::cShadowKind_Kart:
        return _18;
    case ShadowModel::cShadowKind_Geo:
        return _28;
    case ShadowModel::cShadowKind_2:
        return _38;
    case ShadowModel::cShadowKind_SkyShip:
        return _48;
    default:
    #line 57
        JUT_ASSERT_F(false, "%s", "Regist error!");
        return _18;
    }
}

void ShadowManager::regist(ShadowModel* pModel) 
{
    #line 68
    JUT_ASSERT_F(pModel != nullptr, "%s", "pModel != 0");

    ShadowModel::ShadowKind kind = pModel->getShadowKind();
    List& modelList = getShadowModelList(kind);
    
    modelList.push_back(pModel); 
}

// void ShadowManager::draw(u32 viewNo, List& modelList) {}

// v1
// void ShadowManager::draw(u32 viewNo, List& modelList) 
// {
//     if (_5f == 0) {
//         return;
//     }

//     // Setup initial GX state
//     GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
//     GXSetNumTexGens(0);
//     GXSetNumTevStages(1);
//     GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
//     GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//     GXSetNumChans(1);
//     GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
//     GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
//     GXSetZCompLoc(GX_TRUE);
//     GXSetColorUpdate(GX_FALSE);
//     GXSetAlphaUpdate(GX_TRUE);
//     GXSetDstAlpha(GX_DISABLE, 0);

//     // Iterate through shadow models in list
//     List::iterator it = modelList.oEnd_.pNext_;
//     List::iterator end = &modelList.oEnd_;
    
//     while (it.p_ != end.p_) {
//         ShadowModel* pModel = *(ShadowModel**)(it.p_ + 1);
        
//         // Call viewCalc on the model
//         pModel->viewCalc(viewNo);
        
//         // Check if model should be drawn
//         if ((pModel->_8c & 0xF) != 0 && pModel->_95 != 0) {
//             J3DModelData* pModelData = pModel->mModel[0]->getModelData();
//             u16 matCount = pModelData->getMaterialNum();
//             J3DMaterialTable* pMatTable = pModelData->getMaterialTable();
            
//             // Calculate alpha value
//             u8 baseDepth = mDepth[mDepthMode];
//             u8 modelAlpha = pModel->_94;
//             u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
//             if (mDepthMode == 1) {
//                 J3DModel* pJ3DModel = pModel->mModel[0];
//                 J3DModelData* pData = pJ3DModel->getModelData();
//                 J3DMaterial* pMat = pData->getMaterialTable()->getMaterial(0);
//                 u8 matAlpha = pMat->getAlpha();
//                 alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
//             }
            
//             // Set material color
//             JUTColor matColor(0, 0, 0, alphaValue);
//             GXSetChanMatColor(GX_COLOR1, matColor);
            
//             // First pass - one cull direction
//             GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_BACK);
//             } else {
//                 GXSetCullMode(GX_CULL_FRONT);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
            
//             // Second pass - opposite cull direction
//             GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_FRONT);
//             } else {
//                 GXSetCullMode(GX_CULL_BACK);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
//         }
        
//         it.p_ = it.p_->pNext_;
//     }
    
//     // Restore GX state
//     GXSetColorUpdate(GX_TRUE);
//     setDstAlpha();
//     GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
// }

// v2
// void ShadowManager::draw(u32 viewNo, List& modelList) 
// {
//     if (_5f == 0) {
//         return;
//     }

//     // Setup initial GX state
//     GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
//     GXSetNumTexGens(0);
//     GXSetNumTevStages(1);
//     GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
//     GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//     GXSetNumChans(1);
//     GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
//     GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
//     GXSetZCompLoc(GX_TRUE);
//     GXSetColorUpdate(GX_FALSE);
//     GXSetAlphaUpdate(GX_TRUE);
//     GXSetDstAlpha(GX_DISABLE, 0);

//     // Iterate through shadow models in list using proper iterators
//     for (List::iterator it = modelList.begin(); it != modelList.end(); ++it) {
//         ShadowModel* pModel = *it;
        
//         // Call viewCalc on the model
//         pModel->viewCalc(viewNo);
        
//         // Check if model should be drawn
//         if ((pModel->getShadowKind() & 1) != 0 && pModel->_95 != 0) {
//             J3DModelData* pModelData = pModel->mModel[0]->getModelData();
//             u16 matCount = pModelData->getMaterialNum();
//             J3DMaterialTable* pMatTable = pModelData->getMaterialTable();
            
//             // Calculate alpha value
//             u8 baseDepth = mDepth[mDepthMode];
//             u8 modelAlpha = pModel->_94;
//             u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
//             if (mDepthMode == 1) {
//                 J3DModel* pJ3DModel = pModel->mModel[0];
//                 J3DModelData* pData = pJ3DModel->getModelData();
//                 J3DMaterial* pMat = pData->getMaterialTable()->getMaterial(0);
//                 u8 matAlpha = pMat->getAlpha();
//                 alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
//             }
            
//             // Set material color
//             JUTColor matColor(0, 0, 0, alphaValue);
//             GXSetChanMatColor(GX_COLOR1, matColor);
            
//             // First pass - one cull direction
//             GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_BACK);
//             } else {
//                 GXSetCullMode(GX_CULL_FRONT);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
            
//             // Second pass - opposite cull direction
//             GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_FRONT);
//             } else {
//                 GXSetCullMode(GX_CULL_BACK);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
//         }
//     }
    
//     // Restore GX state
//     GXSetColorUpdate(GX_TRUE);
//     setDstAlpha();
//     GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
// }

// v3
// void ShadowManager::draw(u32 viewNo, List& modelList) 
// {
//     if (_5f == 0) {
//         return;
//     }

//     // Setup initial GX state
//     GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
//     GXSetNumTexGens(0);
//     GXSetNumTevStages(1);
//     GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
//     GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//     GXSetNumChans(1);
//     GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
//     GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
//     GXSetZCompLoc(GX_TRUE);
//     GXSetColorUpdate(GX_FALSE);
//     GXSetAlphaUpdate(GX_TRUE);
//     GXSetDstAlpha(GX_DISABLE, 0);

//     // Iterate through shadow models in list using proper iterators
//     for (List::iterator it = modelList.begin(); it != modelList.end(); ++it) {
//         ShadowModel* pModel = *it;
        
//         // Call viewCalc on the model
//         pModel->viewCalc(viewNo);
        
//         // Check if model should be drawn
//         if ((pModel->getFlags() & 0xF) != 0 && pModel->getFlag95() != 0) {
//             J3DModelData* pModelData = pModel->mModel[0]->getModelData();
//             u16 matCount = pModelData->getMaterialNum();
//             J3DMaterialTable* pMatTable = pModelData->getMaterialTable();
            
//             // Calculate alpha value
//             u8 baseDepth = mDepth[mDepthMode];
//             u8 modelAlpha = pModel->getAlpha();
//             u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
//             if (mDepthMode == 1) {
//                 J3DModel* pJ3DModel = pModel->mModel[0];
//                 J3DModelData* pData = pJ3DModel->getModelData();
//                 J3DMaterial* pMat = pData->getMaterialTable()->getMaterial(0);
//                 u8 matAlpha = pMat->getAlpha();
//                 alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
//             }
            
//             // Set material color
//             JUTColor matColor(0, 0, 0, alphaValue);
//             GXSetChanMatColor(GX_COLOR1, matColor);
            
//             // First pass - one cull direction
//             GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_BACK);
//             } else {
//                 GXSetCullMode(GX_CULL_FRONT);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
            
//             // Second pass - opposite cull direction
//             GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_FRONT);
//             } else {
//                 GXSetCullMode(GX_CULL_BACK);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
//         }
//     }
    
//     // Restore GX state
//     GXSetColorUpdate(GX_TRUE);
//     setDstAlpha();
//     GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
// }

// v4
// void ShadowManager::draw(u32 viewNo, List& modelList) 
// {
//     if (_5f == 0) {
//         return;
//     }

//     // Setup initial GX state
//     GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
//     GXSetNumTexGens(0);
//     GXSetNumTevStages(1);
//     GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
//     GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//     GXSetNumChans(1);
//     GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
//     GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
//     GXSetZCompLoc(GX_TRUE);
//     GXSetColorUpdate(GX_FALSE);
//     GXSetAlphaUpdate(GX_TRUE);
//     GXSetDstAlpha(GX_DISABLE, 0);

//     // Iterate through shadow models in list
//     for (List::iterator it = modelList.begin(); it != modelList.end(); ++it) {
//         ShadowModel* pModel = *it;
        
//         // Call viewCalc on the model
//         pModel->viewCalc(viewNo);
        
//         // Check if model should be drawn
//         if ((pModel->getFlags() & 0xF) != 0 && pModel->getFlag95() != 0) {
//             J3DModel* pJ3DModel = pModel->getModel();
//             J3DModelData* pModelData = pJ3DModel->getModelData();
//             u16 matCount = pModelData->getMaterialNum();
//             J3DMaterialTable* pMatTable = pModelData->getMaterialTable();
            
//             // Calculate alpha value
//             u8 baseDepth = mDepth[mDepthMode];
//             u8 modelAlpha = pModel->getAlpha();
//             u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
//             if (mDepthMode == 1) {
//                 J3DMaterial* pMat = pMatTable->getMaterial(0);
//                 u8 matAlpha = pMat->getAlpha();
//                 alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
//             }
            
//             // Set material color
//             JUTColor matColor(0, 0, 0, alphaValue);
//             GXSetChanMatColor(GX_COLOR1, matColor);
            
//             // First pass - one cull direction
//             GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_BACK);
//             } else {
//                 GXSetCullMode(GX_CULL_FRONT);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
            
//             // Second pass - opposite cull direction
//             GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_FRONT);
//             } else {
//                 GXSetCullMode(GX_CULL_BACK);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = pMatTable->getShape(i);
//                 pShape->draw();
//             }
//         }
//     }
    
//     // Restore GX state
//     GXSetColorUpdate(GX_TRUE);
//     setDstAlpha();
//     GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
// }

//v5
// void ShadowManager::draw(u32 viewNo, List& modelList) 
// {
//     if (_5f == 0) {
//         return;
//     }

//     // Setup initial GX state
//     GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
//     GXSetNumTexGens(0);
//     GXSetNumTevStages(1);
//     GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
//     GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//     GXSetNumChans(1);
//     GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
//     GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
//     GXSetZCompLoc(GX_TRUE);
//     GXSetColorUpdate(GX_FALSE);
//     GXSetAlphaUpdate(GX_TRUE);
//     GXSetDstAlpha(GX_DISABLE, 0);

//     // Iterate through shadow models in list
//     for (List::iterator it = modelList.begin(); it != modelList.end(); ++it) {
//         ShadowModel* pModel = *it;
        
//         // Call viewCalc on the model
//         pModel->viewCalc(viewNo);
        
//         // Check if model should be drawn
//         if ((pModel->getFlags() & 0xF) != 0 && pModel->getFlag95() != 0) {
//             J3DModel* pJ3DModel = pModel->getModel();
//             J3DModelData* pModelData = pJ3DModel->getModelData();
//             u16 matCount = pModelData->getMaterialNum();
//             J3DMaterialTable& matTable = pModelData->getMaterialTable();
            
//             // Calculate alpha value
//             u8 baseDepth = mDepth[mDepthMode];
//             u8 modelAlpha = pModel->getAlpha();
//             u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
//             if (mDepthMode == 1) {
//                 J3DMaterial* pMat = matTable.getMaterialNodePointer(0);
//                 u8 matAlpha = pMat->getAlpha();
//                 alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
//             }
            
//             // Set material color
//             JUTColor matColor(0, 0, 0, alphaValue);
//             GXSetChanMatColor(GX_COLOR1, matColor);
            
//             // First pass - one cull direction
//             GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_BACK);
//             } else {
//                 GXSetCullMode(GX_CULL_FRONT);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = matTable.getShapeNodePointer(i);
//                 pShape->draw();
//             }
            
//             // Second pass - opposite cull direction
//             GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_FRONT);
//             } else {
//                 GXSetCullMode(GX_CULL_BACK);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = matTable.getShapeNodePointer(i);
//                 pShape->draw();
//             }
//         }
//     }
    
//     // Restore GX state
//     GXSetColorUpdate(GX_TRUE);
//     setDstAlpha();
//     GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
// }

// v6
// void ShadowManager::draw(u32 viewNo, List& modelList) 
// {
//     if (_5f == 0) {
//         return;
//     }

//     // Setup initial GX state
//     GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
//     GXSetNumTexGens(0);
//     GXSetNumTevStages(1);
//     GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
//     GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//     GXSetNumChans(1);
//     GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
//     GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
//     GXSetZCompLoc(GX_TRUE);
//     GXSetColorUpdate(GX_FALSE);
//     GXSetAlphaUpdate(GX_TRUE);
//     GXSetDstAlpha(GX_DISABLE, 0);

//     // Iterate through shadow models in list
//     for (List::iterator it = modelList.begin(); it != modelList.end(); ++it) {
//         ShadowModel* pModel = *it;
        
//         // Call viewCalc on the model
//         pModel->viewCalc(viewNo);
        
//         // Check if model should be drawn
//         if ((pModel->getFlags() & 0xF) != 0 && pModel->getFlag95() != 0) {
//             J3DModel* pJ3DModel = pModel->getModel();
//             J3DModelData* pModelData = pJ3DModel->getModelData();
//             u16 matCount = pModelData->getMaterialNum();
//             J3DMaterialTable& matTable = pModelData->getMaterialTable();
            
//             // Calculate alpha value
//             u8 baseDepth = mDepth[mDepthMode];
//             u8 modelAlpha = pModel->getAlpha();
//             u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
//             if (mDepthMode == 1) {
//                 J3DMaterial* pMat = matTable.getMaterialNodePointer(0);
//                 J3DGXColor* matColor = pMat->getColorBlock()->getMatColor(0);
//                 u8 matAlpha = matColor->a;
//                 alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
//             }
            
//             // Set material color
//             JUTColor chanColor(0, 0, 0, alphaValue);
//             GXSetChanMatColor(GX_COLOR1, chanColor);
            
//             // First pass - one cull direction
//             GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_BACK);
//             } else {
//                 GXSetCullMode(GX_CULL_FRONT);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = matTable.getShapeNodePointer(i);
//                 pShape->draw();
//             }
            
//             // Second pass - opposite cull direction
//             GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
//             if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
//                 GXSetCullMode(GX_CULL_FRONT);
//             } else {
//                 GXSetCullMode(GX_CULL_BACK);
//             }
            
//             for (u16 i = 0; i < matCount; i++) {
//                 J3DShape* pShape = matTable.getShapeNodePointer(i);
//                 pShape->draw();
//             }
//         }
//     }
    
//     // Restore GX state
//     GXSetColorUpdate(GX_TRUE);
//     setDstAlpha();
//     GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
// }

// v7
void ShadowManager::draw(u32 viewNo, List& modelList) 
{
    if (_5f == 0) {
        return;
    }

    // Setup initial GX state
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_DISABLE, 0);

    // Iterate through shadow models in list
    for (List::iterator it = modelList.begin(); it != modelList.end(); ++it) {
        ShadowModel* pModel = *it;
        
        // Call viewCalc on the model
        pModel->viewCalc(viewNo);
        
        // Check if model should be drawn
        if ((pModel->getFlags() & 0xF) != 0 && pModel->getFlag95() != 0) {
            J3DModel* pJ3DModel = pModel->getModel();
            J3DModelData* pModelData = pJ3DModel->getModelData();
            u16 matCount = pModelData->getMaterialNum();
            J3DMaterialTable& matTable = pModelData->getMaterialTable();
            
            // Calculate alpha value
            u8 baseDepth = mDepth[mDepthMode];
            u8 modelAlpha = pModel->getAlpha();
            u8 alphaValue = (baseDepth * modelAlpha) / 0xFF;
            
            if (mDepthMode == 1) {
                J3DMaterial* pMat = matTable.getMaterialNodePointer(0);
                J3DGXColor* matColor = pMat->getColorBlock()->getMatColor(0);
                u8 matAlpha = matColor->a;
                alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
            }
            
            // Set material color
            JUTColor chanColor(0, 0, 0, alphaValue);
            GXSetChanMatColor(GX_COLOR1, chanColor);
            
            // First pass - one cull direction
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
            if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
                GXSetCullMode(GX_CULL_BACK);
            } else {
                GXSetCullMode(GX_CULL_FRONT);
            }
            
            for (u16 i = 0; i < matCount; i++) {
                J3DShape* pShape = pModelData->getShapeNodePointer(i);
                pShape->draw();
            }
            
            // Second pass - opposite cull direction
            GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
            
            if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
                GXSetCullMode(GX_CULL_FRONT);
            } else {
                GXSetCullMode(GX_CULL_BACK);
            }
            
            for (u16 i = 0; i < matCount; i++) {
                J3DShape* pShape = pModelData->getShapeNodePointer(i);
                pShape->draw();
            }
        }
    }
    
    // Restore GX state
    GXSetColorUpdate(GX_TRUE);
    setDstAlpha();
    GXSetZMode(GX_TRUE, GX_GEQUAL, GX_TRUE);
}

// void ShadowManager::calc() {}

// void ShadowManager::calc() {}

// void ShadowManager::viewCalc(u32 viewNo) {}

void ShadowManager::setDstAlpha()
{
    // Enable alpha updates
    GXSetAlphaUpdate(GX_TRUE);
    
    // u8 alphaValue = (mDepthMode == 1) ? 0xff - (mDepth[1] & 0xfc) : 0xff;
    u8 alphaValue = getDstAlpha();
    
    // Set destination alpha
    GXSetDstAlpha(GX_ENABLE, alphaValue);
}
