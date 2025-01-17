#include "JSystem/J2D/J2DMaterialFactory.h"
#include "JSystem/JSupport.h"
#include "JSystem/JUtility/JUTResource.h"
#include "JSystem/J2D/J2DScreen.h"
#include "dolphin/gx/GXEnum.h"


J2DMaterialFactory::J2DMaterialFactory(const J2DMaterialBlock& header)
{
    _00               = header._08;
    mMaterialInitData = JSUConvertOffsetToPtr<J2DMaterialInitData>(&header, (void*)header.mMatInitDataOffset);
    mMatIndexTable    = JSUConvertOffsetToPtr<u16>(&header, (void*)header.mMatIndexTableOffset);

    if (header.mIndInitDataOffset && header.mIndInitDataOffset - header._14 > 4) {
        mIndInitData = JSUConvertOffsetToPtr<J2DIndInitData>(&header, (void*)header.mIndInitDataOffset);
    } else {
        mIndInitData = nullptr;
    }

    mCullMode      = JSUConvertOffsetToPtr<GXCullMode>(&header, (void*)header.mCullModeOffset);
    mMatColor      = JSUConvertOffsetToPtr<GXColor>(&header, (void*)header.mMatColorOffset);
    mColorChanNum  = JSUConvertOffsetToPtr<u8>(&header, (void*)header.mColorChanNumOffset);
    mColorChanInfo = JSUConvertOffsetToPtr<J2DColorChanInfo>(&header, (void*)header.mColorChanInfoOffset);
    mTexGenNum     = JSUConvertOffsetToPtr<u8>(&header, (void*)header.mTexGenNumOffset);

    mTexCoordInfo = JSUConvertOffsetToPtr<J2DTexCoordInfo>(&header, (void*)header.mTexCoordInfoOffset);
    mTexMtxInfo   = JSUConvertOffsetToPtr<J2DTexMtxInfo>(&header, (void*)header.mTexMtxInfoOffset);
    mTexNo        = JSUConvertOffsetToPtr<u16>(&header, (void*)header.mTexNoOffset);
    mFontNo       = JSUConvertOffsetToPtr<u16>(&header, (void*)header.mFontNoOffset);

    mTevOrderInfo     = JSUConvertOffsetToPtr<J2DTevOrderInfo>(&header, (void*)header.mTevOrderInfoOffset);
    mTevColor         = JSUConvertOffsetToPtr<GXColorS10>(&header, (void*)header.mTevColorOffset);
    mTevKColor        = JSUConvertOffsetToPtr<GXColor>(&header, (void*)header.mTevKColorOffset);
    mTevStageNum      = JSUConvertOffsetToPtr<u8>(&header, (void*)header.mTevStageNumOffset);
    mTevStageInfo     = JSUConvertOffsetToPtr<J2DTevStageInfo>(&header, (void*)header.mTevStageInfoOffset);
    mTevSwapInfo      = JSUConvertOffsetToPtr<J2DTevSwapModeInfo>(&header, (void*)header.mTevSwapInfoOffset);
    mTevSwapTableInfo = JSUConvertOffsetToPtr<J2DTevSwapModeTableInfo>(&header, (void*)header.mTevSwapTableInfoOffset);

    mAlphaCompInfo = JSUConvertOffsetToPtr<J2DAlphaCompInfo>(&header, (void*)header.mAlphaCompInfoOffset);
    mBlendInfo     = JSUConvertOffsetToPtr<J2DBlendInfo>(&header, (void*)header.mBlendInfoOffset);
    mDither        = JSUConvertOffsetToPtr<u8>(&header, (void*)header.mDitherOffset);
}

u32 J2DMaterialFactory::countStages(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    u32 manualCount               = 0;
    u32 storedCount               = 0;
    if (initData->mTevStageIdx != 0xFF) {
        storedCount = mTevStageNum[initData->mTevStageIdx];
    }

    for (int i = 0; i < 8; i++) {
        if (initData->mTexNoIdx[i] != 0xFFFF) {
            manualCount++;
        }
    }

    if ((storedCount != manualCount && manualCount != 0)) {
        return (storedCount > manualCount) ? storedCount : manualCount;
    }

    return storedCount;
}

J2DMaterial* J2DMaterialFactory::create(J2DMaterial* material, int idx, u32 flags, J2DResReference* texRef, J2DResReference* fontRef,
                                        JKRArchive* arc) const
{
    u32 stages     = countStages(idx);
    u32 stageTemp  = ((flags & 0x1F0000) >> 16);
    u32 stageCount = stages > stageTemp ? stages : stageTemp;
    u32 texCount   = stageCount <= 8 ? stageCount : 8;

    int indRequired         = ((flags & 0x1000000) != 0);
    int indBlockNum         = (flags & 0x1F0000) ? indRequired : 0;
    bool doCreateWithHeap   = (flags & 0x1F0000);
    u16 tevBlockNum         = stageCount;
    material->mTevBlock     = J2DMaterial::createTevBlock(tevBlockNum, doCreateWithHeap);
    material->mIndBlock     = J2DMaterial::createIndBlock(indBlockNum, doCreateWithHeap);
    material->mIndex        = idx;
    material->mMaterialMode = getMaterialMode(idx);
    material->getColorBlock()->setColorChanNum(newColorChanNum(idx));
    material->getColorBlock()->setCullMode(newCullMode(idx));
    material->getTexGenBlock()->setTexGenNum(newTexGenNum(idx));
    material->getPEBlock()->setAlphaComp(newAlphaComp(idx));
    material->getPEBlock()->setBlend(newBlend(idx));
    material->getPEBlock()->setDither(newDither(idx));
    material->getTevBlock()->setTevStageNum(newTevStageNum(idx));
    material->mMaterialAlphaCalc = getMaterialAlphaCalc(idx);

    // set up textures
    JUTResReference resRef;
    for (u8 i = 0; i < texCount; i++) {
        u16 texNo  = newTexNo(idx, i);
        s8* texPtr = texRef->getResReference(texNo);
        void* timg = nullptr;

        if (texPtr) {
            // try getting timg from resource reference
            timg = resRef.getResource(texPtr, 'TIMG', arc);

            // if nothing there, try from archive
            if (!timg && arc) {
                timg = resRef.getResource(texPtr, 'TIMG', nullptr);
            }

            // if still nothing there, try from screen data manager
            if (!timg && J2DScreen::getDataManage()) {
                char texName[260];
                strcpy(texName, texRef->getName(texNo));
                timg = J2DScreen::getDataManage()->get(texName);
            }
        }

        material->getTevBlock()->insertTexture(i, (ResTIMG*)timg);
        material->getTevBlock()->setTexNo(i, texNo);
    }

    // set up font
    u16 fontNo = newFontNo(idx);
    material->getTevBlock()->setFontNo(fontNo);

    s8* fontPtr = fontRef->getResReference(material->getTevBlock()->getFontNo());
    void* font  = nullptr;
    if (fontPtr) {
        // try getting font from resource reference
        font = resRef.getResource(fontPtr, 'FONT', arc);

        // if nothing there, try from archive
        if (!font && arc) {
            font = resRef.getResource(fontPtr, 'FONT', nullptr);
        }

        // if still nothing there, try from screen data manager
        if (!font && J2DScreen::getDataManage()) {
            char fontName[260];
            strcpy(fontName, fontRef->getName(material->getTevBlock()->getFontNo()));
            font = J2DScreen::getDataManage()->get(fontName);
        }
    }

    material->getTevBlock()->setFont((ResFONT*)font);

    // set tev orders
    for (u8 i = 0; i < stageCount; i++) {
        material->getTevBlock()->setTevOrder(i, newTevOrder(idx, i));
    }

    // set tev stages
    for (u8 i = 0; i < stageCount; i++) {
        J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
        material->getTevBlock()->setTevStage(i, newTevStage(idx, i));
        if (initData->mTevSwapInfoIdx[i] != 0xFFFF) {
            material->getTevBlock()->getTevStage(i)->setTexSel(mTevSwapInfo[initData->mTevSwapInfoIdx[i]].mTexSel);
            material->getTevBlock()->getTevStage(i)->setRasSel(mTevSwapInfo[initData->mTevSwapInfoIdx[i]].mRasSel);
        }
    }

    // set tev K colors
    for (u8 i = 0; i < 4; i++) {
        material->getTevBlock()->setTevKColor(i, newTevKColor(idx, i));
    }

    // set tev colors
    for (u8 i = 0; i < 4; i++) {
        J2DTevBlock* block = material->getTevBlock();
        block->setTevColor(i, newTevColor(idx, i));
    }

    // set swap mode table
    for (u8 i = 0; i < 4; i++) {
        material->getTevBlock()->setTevSwapModeTable(i, newTevSwapModeTable(idx, i));
    }

    // set material colors
    for (u8 i = 0; i < 2; i++) {
        material->getColorBlock()->setMatColor(i, newMatColor(idx, i));
    }

    // set color channels
    for (u8 i = 0; i < 4; i++) {
        material->getColorBlock()->setColorChan(i, newColorChan(idx, i));
    }

    // set tex coordinates
    for (u8 i = 0; i < 8; i++) {
        const J2DTexCoord coord = newTexCoord(idx, i);
        material->getTexGenBlock()->setTexCoord(i, &coord);
    }

    // set tex matrices
    for (u8 i = 0; i < 8; i++) {
        material->getTexGenBlock()->setTexMtx(i, newTexMtx(idx, i));
    }

    // set tev K sels
    J2DMaterialInitData* initData2 = &mMaterialInitData[mMatIndexTable[idx]];
    for (u8 i = 0; i < stageCount; i++) {
        material->getTevBlock()->setTevKColorSel(i, initData2->mTevKColorSel[i]);
    }
    for (u8 i = 0; i < stageCount; i++) {
        material->getTevBlock()->setTevKAlphaSel(i, initData2->mTevKAlphaSel[i]);
    }

    // set indirect texture info
    if (mIndInitData || indBlockNum != 0) {
        u8 indTexStageNum = newIndTexStageNum(idx);
        material->mIndBlock->setIndTexStageNum(indTexStageNum);
        for (u8 i = 0; i < indTexStageNum; i++) {
            material->getIndBlock()->setIndTexMtx(i, newIndTexMtx(idx, i));
        }
        for (u8 i = 0; i < indTexStageNum; i++) {
            material->getIndBlock()->setIndTexOrder(i, newIndTexOrder(idx, i));
        }
        for (u8 i = 0; i < indTexStageNum; i++) {
            material->getIndBlock()->setIndTexCoordScale(i, newIndTexCoordScale(idx, i));
        }
        for (u8 i = 0; i < stageCount; i++) {
            material->getTevBlock()->setIndTevStage(i, newIndTevStage(idx, i));
        }
    }

    return material;
}

JUTColor J2DMaterialFactory::newMatColor(int idx, int colorIdx) const
{
    GXColor white             = { 255, 255, 255, 255 };
    JUTColor jutWhite = GXColor(white);

    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mMatColorIdx[colorIdx] != 0xFFFF) {
        return mMatColor[initData->mMatColorIdx[colorIdx]];
    }
    return jutWhite;
}

u8 J2DMaterialFactory::newColorChanNum(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mColorChanIdx != 0xFF) {
        return mColorChanNum[initData->mColorChanIdx];
    }
    return 0;
}

J2DColorChan J2DMaterialFactory::newColorChan(int idx, int chan) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mColorChanInfoIdx[chan] != 0xFFFF) {
        return J2DColorChan(mColorChanInfo[initData->mColorChanInfoIdx[chan]]);
    }
    return J2DColorChan();
}

u32 J2DMaterialFactory::newTexGenNum(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTexGenIdx != 0xFF) {
        return mTexGenNum[initData->mTexGenIdx];
    }
    return 0;
}

J2DTexCoord J2DMaterialFactory::newTexCoord(int idx, int texCoord) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTexCoordIdx[texCoord] != 0xFFFF) {
        return J2DTexCoord(mTexCoordInfo[initData->mTexCoordIdx[texCoord]]);
    }
    return J2DTexCoord();
}

J2DTexMtx* J2DMaterialFactory::newTexMtx(int idx, int texMtx) const
{
    J2DTexMtx* mtx                = nullptr;
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTexMtxIdx[texMtx] != 0xFFFF) {
        mtx = new J2DTexMtx(mTexMtxInfo[initData->mTexMtxIdx[texMtx]]);
        mtx->calc();
    }
    return mtx;
}

u8 J2DMaterialFactory::newCullMode(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mCullModeIdx != 0xFF) {
        return mCullMode[initData->mCullModeIdx];
    }
    return 0xff;
}

u16 J2DMaterialFactory::newTexNo(int idx, int p2) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTexNoIdx[p2] != 0xFFFF) {
        return mTexNo[initData->mTexNoIdx[p2]];
    }
    return 0xFFFF;
}

u16 J2DMaterialFactory::newFontNo(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mFontNoIdx != 0xFFFF) {
        return mFontNo[initData->mFontNoIdx];
    }
    return 0xFFFF;
}

J2DTevOrder J2DMaterialFactory::newTevOrder(int idx, int p2) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTevOrderInfoIdx[p2] != 0xFFFF) {
        return J2DTevOrder(mTevOrderInfo[initData->mTevOrderInfoIdx[p2]]);
    }
    return J2DTevOrder();
}

J2DGXColorS10 J2DMaterialFactory::newTevColor(int idx, int p2) const
{
    const J2DGXColorS10 c( (GXColorS10){ 0,0,0,0 });

    J2DMaterialInitData& initData = mMaterialInitData[mMatIndexTable[idx]];
    
    if (initData.mTevColorIdx[p2] != 0xFFFF) {
        return mTevColor[initData.mTevColorIdx[p2]];
    }
    return c;

}

JUTColor J2DMaterialFactory::newTevKColor(int idx, int p2) const
{
    GXColor white                 = { 0xFF, 0xFF, 0xFF, 0xFF };
    JUTColor jutWhite     = GXColor(white);
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTevKColorIdx[p2] != 0xFFFF) {
        return mTevKColor[initData->mTevKColorIdx[p2]];
    }
    return jutWhite;
}

u8 J2DMaterialFactory::newTevStageNum(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTevStageIdx != 0xFF) {
        return mTevStageNum[initData->mTevStageIdx];
    }
    return 0xFF;
}

J2DTevStage J2DMaterialFactory::newTevStage(int idx, int p2) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTevStageInfoIdx[p2] != 0xFFFF) {
        return mTevStageInfo[initData->mTevStageInfoIdx[p2]]; // this needs fixing
    }
    return J2DTevStage();
}

J2DTevSwapModeTable J2DMaterialFactory::newTevSwapModeTable(int idx, int p2) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mTevSwapModeTableInfoIdx[p2] != 0xFFFF) {
        return J2DTevSwapModeTable(mTevSwapTableInfo[initData->mTevSwapModeTableInfoIdx[p2]]);
    }
    return J2DTevSwapModeTable(j2dDefaultTevSwapModeTable);
}

u8 J2DMaterialFactory::newIndTexStageNum(int idx) const
{
    if (mIndInitData && mIndInitData[idx].mUseData == 1) {
        return mIndInitData[idx].mIndTexStageNum;
    }
    return 0;
}

J2DIndTexOrder J2DMaterialFactory::newIndTexOrder(int idx, int p2) const
{
    J2DIndTexOrder texOrder;
    if (mIndInitData && mIndInitData[idx].mUseData == 1) {
        return J2DIndTexOrder(mIndInitData[idx].mIndTexOrderInfo[p2]);
    }
    return texOrder;
}

J2DIndTexMtx J2DMaterialFactory::newIndTexMtx(int idx, int p2) const
{
    J2DIndTexMtx mtx;
    if (mIndInitData && mIndInitData[idx].mUseData == 1) {
        return J2DIndTexMtx(mIndInitData[idx].mIndTexMtxInfo[p2]);
    }
    return mtx;
}

J2DIndTevStage J2DMaterialFactory::newIndTevStage(int idx, int p2) const
{
    J2DIndTevStage tevStage;
    if (mIndInitData && mIndInitData[idx].mUseData == 1) {
        return J2DIndTevStage(mIndInitData[idx].mIndTevStageInfo[p2]);
    }
    return tevStage;
}

J2DIndTexCoordScale J2DMaterialFactory::newIndTexCoordScale(int idx, int p2) const
{
    J2DIndTexCoordScale coordScale;
    if (mIndInitData && mIndInitData[idx].mUseData == 1) {
        return J2DIndTexCoordScale(mIndInitData[idx].mIndTexCoordScaleInfo[p2]);
    }
    return coordScale;
}

J2DAlphaComp J2DMaterialFactory::newAlphaComp(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mAlphaCompInfoIdx != 0xFFFF) {
        return J2DAlphaComp(mAlphaCompInfo[initData->mAlphaCompInfoIdx]);
    }
    return J2DAlphaComp();
}

J2DBlend J2DMaterialFactory::newBlend(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mBlendInfoIdx != 0xFFFF) {
        return J2DBlend(mBlendInfo[initData->mBlendInfoIdx]);
    }
    return J2DBlend();
}

u8 J2DMaterialFactory::newDither(int idx) const
{
    J2DMaterialInitData* initData = &mMaterialInitData[mMatIndexTable[idx]];
    if (initData->mDitherIdx != 0xFF) {
        return mDither[initData->mDitherIdx];
    }
    return 0;
}
