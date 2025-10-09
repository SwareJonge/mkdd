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

void ShadowManager::draw(u32 viewNo, List& modelList) {
    if (_5f == 0) {
        return;
    }

    // Setup initial GX state
    GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_DISABLE, 0);
    
    List::iterator it = modelList.begin();
    List::const_iterator itEnd = modelList.end();
    
    for (; it != itEnd; ++it) {
        (*it)->setCurrentViewNo(viewNo);
        
        if (((*it)->getFlags() & 0xF) != 0 && (*it)->getFlag95() != 0) {
            J3DModelData* pModelData = (*it)->getModelData();
            J3DModel* pJ3DModel = (*it)->getModel();
            u32 numShapes = pModelData->getShapeNum();
            
            // Calculate alpha value
            u8 alphaValue = (mDepth[mDepthMode] * (*it)->getAlpha()) / 0xFF;
            
            if (mDepthMode == 1) {
                J3DMaterial* pMat = pModelData->getMaterialNodePointer(0);
                u8 matAlpha = pMat->getColorBlock()->getMatColor(0)->a;
                alphaValue = (alphaValue * matAlpha) / 0xFF & 0xFC;
            }
            
            // Set material color
            JUTColor chanColor = JUTColor(0, 0, 0, alphaValue);
            GXSetChanMatColor(GX_COLOR0A0, chanColor);
            
            // First pass
            GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
            
            if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
                GXSetCullMode(GX_CULL_BACK);
            } else {
                GXSetCullMode(GX_CULL_FRONT);
            }
            
            for (u16 i = 0; i < numShapes; i++) {
                pJ3DModel->getShapePacket(i)->draw();
            }
            
            // Second pass
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
            
            if ((mDepthMode == 1 && !mIsMirror) || (mDepthMode == 0 && mIsMirror)) {
                GXSetCullMode(GX_CULL_FRONT);
            } else {
                GXSetCullMode(GX_CULL_BACK);
            }
            
            for (u16 i = 0; i < numShapes; i++) {
                pJ3DModel->getShapePacket(i)->draw();
            }
        }
    }
    
    // Restore GX state
    GXSetColorUpdate(GX_TRUE);
    setDstAlpha();
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

void ShadowManager::calc()
{
    for (u8 i = 0; i < 4; i++) {
        List& modelList = getShadowModelList((ShadowModel::ShadowKind)i);
        
        List::iterator it = modelList.begin();
        List::const_iterator itEnd = modelList.end();
        
        for (; it != itEnd; ++it) {
            if (((*it)->getFlags() & 0xF) != 0) {
                (*it)->calc();
                (*it)->setFlag95(true);
            } else {
                (*it)->setFlag95(false);
            }
        }
    }
    
    if (_5f == 0) {
        _5f += 1;
    }
}

void ShadowManager::viewCalc(u32 viewNo)
{
    for (u8 i = 0; i < 4; i++) {
        List& modelList = getShadowModelList((ShadowModel::ShadowKind)i);
        
        List::const_iterator itEnd = modelList.end();
        List::iterator it = modelList.begin();
        
        for (; it != itEnd; ++it) {
            if (((*it)->getFlags() & 0xF) != 0) {
                (*it)->viewCalc(viewNo);
            }
        }
    }
}

void ShadowManager::setDstAlpha()
{
    // Enable alpha updates
    GXSetAlphaUpdate(GX_TRUE);
    
    // u8 alphaValue = (mDepthMode == 1) ? 0xff - (mDepth[1] & 0xfc) : 0xff;
    u8 alphaValue = getDstAlpha();
    
    // Set destination alpha
    GXSetDstAlpha(GX_ENABLE, alphaValue);
}
