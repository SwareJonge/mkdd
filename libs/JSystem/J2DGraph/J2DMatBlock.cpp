#include "JSystem/J2D/J2DColorBlock.h"
#include "JSystem/J2D/J2DTexGenBlock.h"
#include "JSystem/J2D/J2DTevBlock.h"
#include "JSystem/J2D/J2DPEBlock.h"
#include "JSystem/J2D/J2DTypes.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "dolphin/gx/GXEnum.h"
#include "types.h"

#pragma sym on

void J2DColorBlock::initialize() {
    for (int i = 0; i < 2; i++) {
        mColors[i] = j2dDefaultColInfo;
    }
    mChannelCount = 2;

    for (int i = 0; i < 4; i++) {
        mChannels[i].setColorChanInfo(j2dDefaultColorChanInfo); 
    }
    mCullMode = 0;
}

void J2DColorBlock::setGX()
{
    for (int i = 0; i < 2; i++) {
        GXSetChanMatColor(GXChannelID(GX_COLOR0A0 + i), mColors[i]);
    }
    GXSetNumChans(mChannelCount);
    const GXChannelID mapping[4] = { GX_COLOR0, GX_ALPHA0, GX_COLOR1, GX_ALPHA1 };
    for (int i = 0; i < mChannelCount << 1; i++) {
        GXSetChanCtrl(mapping[i], GX_DISABLE, GX_SRC_REG, (GXColorSrc)mChannels[i].getMatSrc(), GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
    GXSetCullMode((GXCullMode)mCullMode);
}

void J2DTexGenBlock::initialize()
{
    mTexGenNum = 0;
    for (int i = 0; i < 8; i++) {
        mTexCoords[i].setTexCoordInfo(j2dDefaultTexCoordInfo[0]);
    }
    for (int i = 0; i < 8; i++) {
        mTexMtxes[i] = nullptr;
    }
}

void J2DTexGenBlock::setGX()
{
    GXSetNumTexGens(mTexGenNum);
    for (int i = 0; i < mTexGenNum; i++) {
        GXSetTexCoordGen2((GXTexCoordID)i, (GXTexGenType)mTexCoords[i].mTexCoordInfo.mTexGenType,
                          (GXTexGenSrc)mTexCoords[i].mTexCoordInfo.mTexGenSrc, mTexCoords[i].mTexCoordInfo.mTexGenMtx, GX_FALSE, 0x7D);
    }
    for (u8 i = 0; i < 8; i++) {
        if (mTexMtxes[i] != nullptr) {
            mTexMtxes[i]->load(i);
        }
    }
}

J2DTexGenBlock::~J2DTexGenBlock()
{
    for (int i = 0; i < 8; i++) {
        delete mTexMtxes[i];
    }
}

void J2DTexGenBlock::setTexMtx(u32 index, J2DTexMtx& mtx)
{
    if (mTexMtxes[index] == nullptr) {
        mTexMtxes[index] = new J2DTexMtx(mtx.getTexMtxInfo());
        if (mTexMtxes[index] == nullptr) {
            return;
        }
    } else {
        mTexMtxes[index]->setTexMtxInfo(mtx.getTexMtxInfo());
    }
    mTexMtxes[index]->calc();
}

void J2DTexGenBlock::getTexMtx(u32 id, J2DTexMtx& texMtx)
{
    if (mTexMtxes[id]) {
        texMtx.setTexMtxInfo(mTexMtxes[id]->getTexMtxInfo());
    }
}

JUTTexture* J2DTevBlock::getTexture(u32 index) { return nullptr; }

JUTPalette* J2DTevBlock::getPalette(u32 index) { return nullptr; }

J2DTevBlock1::J2DTevBlock1()
    : J2DTevBlock()
    , mOrders()
    , mColors()
    , mStages()
    , mKColors()
    , mSwapModeTables()
    , mIndStages()
{
    mTextures[0]  = nullptr;
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock1::~J2DTevBlock1()
{
    if ((mUndeleteFlag & 0x01) != 0) {
        delete mTextures[0];
    }
    delete mPalettes[0];
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
}

void J2DTevBlock1::initialize()
{
    mTexIndices[0] = -1;
    mFontNo        = -1;
    mOrders[0].setTevOrderInfo(j2dDefaultTevOrderInfoNull);

    for (int i = 0; i < 4; i++) {
        mColors[i] = j2dDefaultTevColor;
    }

    mStages[0].setStageNo(0);

    for (int i = 0; i < 4; i++) {
        mKColors[i] = j2dDefaultTevKColor;
    }

    for (int i = 0; i < 1; i++) {
        mKColorSels[i] = 255;
        mKAlphaSels[i] = 255;
    }

    for (int i = 0; i < 4; i++) {
        mSwapModeTables[i].setTevSwapModeTableInfo(j2dDefaultTevSwapModeTable);
    }

    for (int i = 0; i < 1; i++) {
        mIndStages[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }

    mPalettes[0] = nullptr;
    mFont = nullptr;
}

bool J2DTevBlock1::prepareTexture(u8 count)
{
    if (count == 0) {
        return true;
    }
    if (mTextures[0] == nullptr) {
        mTextures[0] = new JUTTexture();
        if (mTextures[0] == nullptr) {
            return false;
        }
        mUndeleteFlag = mUndeleteFlag & 0x80 | 1;
    }
    return count <= 1;
}

bool J2DTevBlock1::insertTexture(u32 id, const ResTIMG* timg, JUTPalette* tlut)
{
    if (id != 0 || timg == nullptr) {
        return false;
    }

    if (mTextures[0] && mTextures[0]->getTexInfo()) {
        return false;
    }

    if (mTextures[0] == nullptr) {
        mTextures[0] = new JUTTexture(timg, 0);

        if (mTextures[0] == nullptr) {
            return false;
        }

        if (tlut) {
            mTextures[0]->storeTIMG(timg, tlut);
        }
    } else if (tlut == nullptr) {
        mTextures[0]->storeTIMG(timg);
    } else {
        mTextures[0]->storeTIMG(timg, tlut);
    }

    mUndeleteFlag  = (mUndeleteFlag & 0x80) | 1;
    mPalettes[0]   = nullptr;
    mTexIndices[0] = -1;
    return true;
}

bool J2DTevBlock1::insertTexture(u32 id, JUTTexture* tex)
{
    if (mTextures[0] && mTextures[0]->getTexInfo()) {
        return false;
    }

    if (id != 0 || tex == nullptr) {
        return false;
    }

    if (mUndeleteFlag & 1) {
        delete mTextures[0];
    }

    mTextures[0] = tex;
    mUndeleteFlag &= 0x80;
    mPalettes[0]   = nullptr;
    mTexIndices[0] = -1;
    return true;
}

bool J2DTevBlock1::setTexture(u32 id, const ResTIMG* timg)
{
    if (id != 0) {
        return false;
    }

    if (mTextures[0] == nullptr) {
        if (timg) {
            mTextures[0] = new JUTTexture(timg, 0);

            if (mTextures[0] == nullptr) {
                return false;
            }

            mUndeleteFlag = (mUndeleteFlag & 0x80) | 1;
        } else {
            mUndeleteFlag &= 0x80;
        }
    } else if (mUndeleteFlag & 1) {
        if (timg) {
            mTextures[0]->storeTIMG(timg);
        } else {
            delete mTextures[0];
            mTextures[0] = nullptr;
            mUndeleteFlag &= 0x80;
        }
    } else {
        mTextures[0] = nullptr;

        if (timg) {
            mTextures[0] = new JUTTexture(timg, 0);

            if (mTextures[0] == nullptr) {
                return false;
            }

            mUndeleteFlag = (mUndeleteFlag & 0x80) | 1;
        }
    }

    delete mPalettes[0];
    mPalettes[0]   = nullptr;
    mTexIndices[0] = -1;
    return true;
}

bool J2DTevBlock1::setTexture(u32 index, JUTTexture* texture)
{
    if (index != 0) {
        return false;
    }
    if ((mUndeleteFlag & 1) != 0) {
        delete mTextures[0];
    }
    mTextures[0] = texture;
    mUndeleteFlag &= 0x80;
    delete mPalettes[0];
    mPalettes[0]   = nullptr;
    mTexIndices[0] = 0xFFFF;
    return true;
}

bool J2DTevBlock1::removeTexture(u32 id)
{
    if (id >= 1) {
        return false;
    }

    if (mUndeleteFlag & 1) {
        delete mTextures[0];
    }

    mTextures[0] = nullptr;
    mUndeleteFlag &= 0x80;
    delete mPalettes[0];
    mTexIndices[0] = -1;
    return true;
}

bool J2DTevBlock1::setFont(ResFONT* font)
{
    if (font == nullptr) {
        return false;
    }
    JUTResFont* jutFont = new JUTResFont(font, nullptr);
    if (jutFont == nullptr) {
        return false;
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
    mFont         = jutFont;
    mUndeleteFlag = mUndeleteFlag & 0x7F | 0x80;
    return true;
}

bool J2DTevBlock1::setFont(JUTFont* font)
{
    if (font == nullptr) {
        return false;
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
    mFont = static_cast<JUTResFont*>(font);
    mUndeleteFlag &= 0x7F;
    return true;
}

bool J2DTevBlock1::setPalette(u32 index, const ResTLUT* lut)
{
    if (index != 0) {
        return false;
    }
    if (lut != nullptr) {
        if (mPalettes[0] == nullptr) {
            mPalettes[0] = new JUTPalette(GX_TLUT0, const_cast<ResTLUT*>(lut));
            if (mPalettes[0] == nullptr) {
                return false;
            }
        } else {
            mPalettes[0]->storeTLUT(GX_TLUT0, const_cast<ResTLUT*>(lut));
        }
        if (mTextures[0] != nullptr) {
            mTextures[0]->attachPalette(mPalettes[0]);
        }
    } else {
        delete mPalettes[0];
        mPalettes[0] = nullptr;
    }
    return true;
}

void J2DTevBlock1::shiftDeleteFlag(u8 flag, bool direction) { mUndeleteFlag = mUndeleteFlag & 0x80; }

void J2DTevBlock1::setGX()
{
    loadTexture(GX_TEXMAP0, 0);
    GXSetTevOrder(GX_TEVSTAGE0, mOrders[0].getTexCoord(), mOrders[0].getTexMap(), mOrders[0].getColor());

    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mColors[i]);
    }

    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GXTevColorArg(mStages[0].getColorA()), GXTevColorArg(mStages[0].getColorB()),
                    GXTevColorArg(mStages[0].getColorC()), GXTevColorArg(mStages[0].getColorD()));
    GXSetTevAlphaIn(GX_TEVSTAGE0, GXTevAlphaArg(mStages[0].getAlphaA()), GXTevAlphaArg(mStages[0].getAlphaB()),
                    GXTevAlphaArg(mStages[0].getAlphaC()), GXTevAlphaArg(mStages[0].getAlphaD()));
    GXSetTevColorOp(GX_TEVSTAGE0, GXTevOp(mStages[0].getCOp()), GXTevBias(mStages[0].getCBias()), GXTevScale(mStages[0].getCScale()),
                    mStages[0].getCClamp(), GXTevRegID(mStages[0].getCReg()));
    GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)mStages[0].getAOp(), GXTevBias(mStages[0].getABias()), GXTevScale(mStages[0].getAScale()),
                    mStages[0].getAClamp(), GXTevRegID(mStages[0].getAReg()));
    GXSetTevKColorSel(GX_TEVSTAGE0, mKColorSels[0] != 0xff ? GXTevKColorSel(mKColorSels[0]) : GX_TEV_KCSEL_1);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, mKAlphaSels[0] != 0xff ? GXTevKAlphaSel(mKAlphaSels[0]) : GX_TEV_KASEL_1);
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mKColors[i]);
    }
    GXSetTevSwapMode(GX_TEVSTAGE0, GXTevSwapSel(mStages[0].getRasSel()), GXTevSwapSel(mStages[0].getTexSel()));
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mSwapModeTables[i].getR()), GXTevColorChan(mSwapModeTables[i].getG()),
                              GXTevColorChan(mSwapModeTables[i].getB()), GXTevColorChan(mSwapModeTables[i].getA()));
    }
    mIndStages->load(0);
}

void J2DTevBlock1::loadTexture(_GXTexMapID id, u32 index)
{
    if (index == 0 && mTextures[index] && mTextures[index]->mTexInfo) {
        mTextures[index]->load(id);
    }
}

J2DTevBlock2::J2DTevBlock2()

{
    mTextures[0] = nullptr;
    mTextures[1] = nullptr;
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock2::~J2DTevBlock2()
{
    if ((mUndeleteFlag & 1) != 0) {
        delete mTextures[0];
    }
    if ((mUndeleteFlag & 2) != 0) {
        delete mTextures[1];
    }
    delete mPalettes[0];
    delete mPalettes[1];
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
}

void J2DTevBlock2::initialize()
{
    mTexIndices[0] = -1;
    mTexIndices[1] = -1;
    mFontNo = -1;

    mOrders[0].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    mOrders[1].setTevOrderInfo(j2dDefaultTevOrderInfoNull);

    for (int i = 0; i < 4; i++) {
        mColors[i] = j2dDefaultTevColor;
    }

    mStageNum = 1;
    mStages[0].setStageNo(0);
    mStages[1].setStageNo(1);

    for (int i = 0; i < 4; i++) {
        mKColors[i] = j2dDefaultTevKColor;
    }


    mKColorSels[0] = 0xff;
    mKColorSels[1] = 0xff;
    mKAlphaSels[0] = 0xff;
    mKAlphaSels[1] = 0xff;

    for (int i = 0; i < 4; i++) {
        mSwapModeTables[i] = j2dDefaultTevSwapModeTable;
    }

    mIndStages[0].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    mIndStages[1].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    mPalettes[0] = nullptr;
    mPalettes[1] = nullptr;
    mFont = nullptr;
}

bool J2DTevBlock2::prepareTexture(u8 count)
{
    for (u8 i = 0; i < count; i++) {
        if (i >= 2) {
            return false;
        }
        if (mTextures[i] == nullptr) {
            mTextures[i] = new JUTTexture();
            if (mTextures[i] == nullptr) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }
    return true;
}

bool J2DTevBlock2::insertTexture(u32 id, const ResTIMG* timg, JUTPalette* tlut)
{
    if (id >= 2 || timg == nullptr) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 2; texNo++) {
        if (mTextures[texNo] == nullptr || mTextures[texNo]->getTexInfo() == nullptr) {
            break;
        }
    }

    if (texNo < id) {
        return false;
    }

    if (texNo == 2) {
        return false;
    }

    JUTTexture* tex;
    u8 tlutid = 0;
    if (timg->mPaletteFormat && tlut == nullptr) {
        tex = mTextures[0];
        if (tex != nullptr && tex->getTexInfo() != nullptr && tex->getTexInfo()->mPaletteFormat) {
            GXTlut tlutname = tex->getTlutName();
            if (tlutname == GX_TLUT0 || tlutname == GX_BIGTLUT0) {
                tlutid = 1;
            }
        }
    }

    tex = mTextures[texNo];
    if (tex == nullptr) {
        JUTTexture* newtex = new JUTTexture(timg, tlutid);

        if (newtex == nullptr) {
            return false;
        }

        if (tlut != nullptr) {
            newtex->storeTIMG(timg, tlut);
        }

        if (id == 0) {
            mTextures[1]   = mTextures[0];
            mPalettes[1]   = mPalettes[0];
            mTexIndices[1] = mTexIndices[0];
        }
        mTextures[id] = newtex;
        shiftDeleteFlag(id, true);
        mUndeleteFlag |= (1 << id);
    } else {
        if (tlut == nullptr) {
            tex->storeTIMG(timg, tlutid);
        } else {
            tex->storeTIMG(timg, tlut);
        }

        bool local_28[2];
        for (u8 i = 0; i < 2; i++) {
            local_28[i] = mUndeleteFlag & (1 << i);
        }

        if (id == 0 && texNo == 1) {
            mTextures[1]   = mTextures[0];
            mPalettes[1]   = mPalettes[0];
            mTexIndices[1] = mTexIndices[0];
            local_28[1]    = local_28[0];
        }
        mTextures[id] = tex;
        mUndeleteFlag &= 0x80;

        for (u8 i = 0; i < 2; i++) {
            if (local_28[i]) {
                mUndeleteFlag |= (1 << i);
            }
        }

        mUndeleteFlag |= (1 << id);
    }

    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock2::insertTexture(u32 id, JUTTexture* tex)
{
    if (id >= 2 || tex == nullptr) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 2; texNo++) {
        if (mTextures[texNo] == nullptr || mTextures[texNo]->getTexInfo() == nullptr) {
            break;
        }
    }

    if (texNo < id) {
        return false;
    }

    if (texNo == 2) {
        return false;
    }

    if (mTextures[1] != nullptr && mTextures[1]->getTexInfo() == nullptr) {
        if (mUndeleteFlag & 2) {
            delete mTextures[1];
        }

        mUndeleteFlag &= ~2;
    }

    if (id == 0) {
        mTextures[1]   = mTextures[0];
        mPalettes[1]   = mPalettes[0];
        mTexIndices[1] = mTexIndices[0];
    }

    mTextures[id]   = tex;
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    shiftDeleteFlag(id, true);
    mUndeleteFlag &= ~(1 << id);
    return true;
}

bool J2DTevBlock2::setTexture(u32 id, const ResTIMG* timg)
{
    if (id >= 2) {
        return false;
    }

    u8 tlutid = 0;
    if (timg && timg->mPaletteFormat) {
        JUTTexture* tex = mTextures[id == 0];
        if (tex) {
            const ResTIMG* timg = tex->getTexInfo();
            if (timg != nullptr && timg->mPaletteFormat) {
                GXTlut tlutName = tex->getTlutName();
                u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
                if (!bVar1) {
                    tlutid = 1;
                }
            }
        }
    }

    if (mTextures[id] == nullptr) {
        if (timg) {
            mTextures[id] = new JUTTexture(timg, tlutid);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        } else {
            mUndeleteFlag &= ~(1 << id);
        }
    } else if (mUndeleteFlag & (1 << id)) {
        if (timg != nullptr) {
            mTextures[id]->storeTIMG(timg, tlutid);
        } else {
            delete mTextures[id];
            mTextures[id] = nullptr;
            mUndeleteFlag &= ~(1 << id);
        }
    } else {
        mTextures[id] = nullptr;

        if (timg != nullptr) {
            mTextures[id] = new JUTTexture(timg, 0);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        }
    }

    delete mPalettes[id];
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock2::setTexture(u32 index, JUTTexture* texture)
{
    if (index >= 2) {
        return false;
    }
    if ((mUndeleteFlag & 1 << index) != 0) {
        delete mTextures[index];
    }
    mTextures[index] = texture;
    mUndeleteFlag &= ~(1 << index);
    delete mPalettes[index];
    mPalettes[index]   = nullptr;
    mTexIndices[index] = 0xFFFF;
    return true;
}

bool J2DTevBlock2::removeTexture(u32 id)
{
    if (id >= 2) {
        return false;
    }

    if (mUndeleteFlag & (1 << id)) {
        delete mTextures[id];
    }
    delete mPalettes[id];

    if (id == 0) {
        mTextures[0]   = mTextures[1];
        mPalettes[0]   = mPalettes[1];
        mTexIndices[0] = mTexIndices[1];
    }

    mTextures[1]   = nullptr;
    mPalettes[1]   = nullptr;
    mTexIndices[1] = -1;
    shiftDeleteFlag(id, false);
    return true;
}

bool J2DTevBlock2::setFont(ResFONT* font)
{
    if (font == nullptr) {
        return false;
    }
    JUTResFont* jutFont = new JUTResFont(font, nullptr);
    if (jutFont == nullptr) {
        return false;
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
    mFont         = jutFont;
    mUndeleteFlag = mUndeleteFlag & 0x7F | 0x80;
    return true;
}

bool J2DTevBlock2::setFont(JUTFont* font)
{
    if (font == nullptr) {
        return false;
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
    mFont = static_cast<JUTResFont*>(font);
    mUndeleteFlag &= 0x7F;
    return true;
}

bool J2DTevBlock2::setPalette(u32 index, const ResTLUT* lut)
{
    if (index >= 2) {
        return false;
    }
    if (lut) {
        if (mPalettes[index] == nullptr) {
            mPalettes[index] = new JUTPalette((GXTlut)index, const_cast<ResTLUT*>(lut));
            if (mPalettes[index] == nullptr) {
                return false;
            }
        } else {
            mPalettes[index]->storeTLUT(GX_TLUT0, const_cast<ResTLUT*>(lut));
        }
        if (mTextures[index]) {
            mTextures[index]->attachPalette(mPalettes[index]);
        }
    } else {
        delete mPalettes[index];
        mPalettes[index] = nullptr;
    }
    return true;
}

void J2DTevBlock2::shiftDeleteFlag(u8 p1, bool p2)
{
    u8 fontFlag = mUndeleteFlag & 0x80;
    mUndeleteFlag &= 0x7F;
    if (p2) {
        u32 v1        = ((1 << p1) - 1);
        mUndeleteFlag = (mUndeleteFlag & v1) | ((mUndeleteFlag & ~v1) << 1);
    } else {
        mUndeleteFlag = (mUndeleteFlag & (1 << p1) - 1) | ((mUndeleteFlag & ~((1 << (p1 + 1)) - 1)) >> 1);
    }
    mUndeleteFlag = mUndeleteFlag | fontFlag;
}

void J2DTevBlock2::setGX()
{
    for (int i = 0; i < 2; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), mOrders[i].getTexCoord(), mOrders[i].getTexMap(), mOrders[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mColors[i]);
    }
    GXSetNumTevStages(mStageNum);
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mStages[i].getColorA()), GXTevColorArg(mStages[i].getColorB()),
                        GXTevColorArg(mStages[i].getColorC()), GXTevColorArg(mStages[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mStages[i].getAlphaA()), GXTevAlphaArg(mStages[i].getAlphaB()),
                        GXTevAlphaArg(mStages[i].getAlphaC()), GXTevAlphaArg(mStages[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mStages[i].getCOp()), GXTevBias(mStages[i].getCBias()), GXTevScale(mStages[i].getCScale()),
                        mStages[i].getCClamp(), GXTevRegID(mStages[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mStages[i].getAOp(), GXTevBias(mStages[i].getABias()), GXTevScale(mStages[i].getAScale()),
                        mStages[i].getAClamp(), GXTevRegID(mStages[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mKColorSels[i] != 0xff ? GXTevKColorSel(mKColorSels[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mKAlphaSels[i] != 0xff ? GXTevKAlphaSel(mKAlphaSels[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mKColors[i]);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mStages[i].getRasSel()), GXTevSwapSel(mStages[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mSwapModeTables[i].getR()), GXTevColorChan(mSwapModeTables[i].getG()),
                              GXTevColorChan(mSwapModeTables[i].getB()), GXTevColorChan(mSwapModeTables[i].getA()));
    }
    for (u8 i = 0; i < mStageNum; i++) {
        mIndStages[i].load(i);
    }
}

void J2DTevBlock2::loadTexture(_GXTexMapID id, u32 index)
{
    if (index < 2 && mTextures[index] != nullptr && mTextures[index]->mTexInfo != nullptr) {
        mTextures[index]->load(id);
    }
}

J2DTevBlock4::J2DTevBlock4()
    : J2DTevBlock()
    , mOrders()
    , mColors()
    , mStages()
    , mKColors()
    , mSwapModeTables()
    , mIndStages()
{
    for (int i = 0; i < 4; i++) {
        mTextures[i] = nullptr;
    }
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock4::~J2DTevBlock4()
{
    for (int i = 0; i < 4; i++) {
        if ((mUndeleteFlag & 1 << i) != 0) {
            delete mTextures[i];
        }
        delete mPalettes[i];
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
}

void J2DTevBlock4::initialize()
{
    for (int i = 0; i < 4; i++) {
        mTexIndices[i] = -1;
    }
    mFontNo = -1;
    for (int i = 0; i < 4; i++) {
        mOrders[i].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    }
    for (int i = 0; i < 4; i++) {
        mColors[i] = j2dDefaultTevColor;
    }
    mStageNum = 1;
    for (int i = 0; i < 4; i++) {
        mStages[i].setStageNo(i);
    }
    for (int i = 0; i < 4; i++) {
        mKColors[i] = j2dDefaultTevKColor;
    }
    for (int i = 0; i < 4; i++) {
        mKColorSels[i] = -1;
        mKAlphaSels[i] = -1;
    }
    for (int i = 0; i < 4; i++) {
        mSwapModeTables[i] = j2dDefaultTevSwapModeTable;
    }
    for (int i = 0; i < 4; i++) {
        mIndStages[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }
    for (int i = 0; i < 4; i++) {
        mPalettes[i] = nullptr;
    }
    mFont = nullptr;
}


bool J2DTevBlock4::prepareTexture(u8 count)
{
    for (u8 i = 0; i < count; i++) {
        if (i >= 4) {
            return false;
        }
        if (mTextures[i] == nullptr) {
            mTextures[i] = new JUTTexture();
            if (mTextures[i] == nullptr) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }
    return true;
}

bool J2DTevBlock4::insertTexture(u32 id, const ResTIMG* timg, JUTPalette* palette)
{
    if (id >= 4 || !timg) {
        return false;
    }
    u8 idx;
    for (idx = 0; idx < 4; idx++) {
        if (!mTextures[idx] || !mTextures[idx]->getTexInfo()) {
            break;
        }
    }
    if (idx < id) {
        return false;
    }
    if (idx == 4) {
        return false;
    }
    u8 local_43 = 0;
    if (timg->mPaletteFormat && !palette) {
        u8 local_44 = 0;
        for (int i = 0; i < 3; i++) {
            if (!mTextures[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTextures[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->mPaletteFormat) {
                continue;
            }
            GXTlut tlutName = mTextures[i]->getTlutName();
            u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (bVar1 < 4) {
                local_44 |= 1 << bVar1;
            }
        }
        for (u8 i = 0; i < 4; i++) {
            if ((local_44 & 1 << i) == 0) {
                local_43 = i;
                break;
            }
        }
    }
    if (!mTextures[idx]) {
        JUTTexture* texture = new JUTTexture(timg, local_43);
        if (!texture) {
            return false;
        }
        if (palette) {
            texture->storeTIMG(timg, palette);
        }
        for (u8 i = 3; i > id; i--) {
            mTextures[i]   = mTextures[i - 1];
            mPalettes[i]   = mPalettes[i - 1];
            mTexIndices[i] = mTexIndices[i - 1];
        }
        mTextures[id] = texture;
        shiftDeleteFlag(id, true);
        mUndeleteFlag |= 1 << id;
    } else {
        JUTTexture* texture = mTextures[idx];
        if (!palette) {
            texture->storeTIMG(timg, local_43);
        } else {
            texture->storeTIMG(timg, palette);
        }
        bool local_38[4];
        for (u8 i = 0; i < 4; i++) {
            local_38[i] = (mUndeleteFlag & 1 << i);
        }
        for (u8 n = idx; n > id; n--) {
            mTextures[n]   = mTextures[n - 1];
            mPalettes[n]   = mPalettes[n - 1];
            mTexIndices[n] = mTexIndices[n - 1];
            local_38[n]    = local_38[n - 1];
        }
        mTextures[id] = texture;
        mUndeleteFlag &= 0x80;
        for (u8 i = 0; i < 4; i++) {
            if (local_38[i]) {
                mUndeleteFlag |= 1 << i;
            }
        }
        mUndeleteFlag |= 1 << id;
    }
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock4::insertTexture(u32 id, JUTTexture* tex)
{
    if (id >= 4 || tex == nullptr) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 4; texNo++) {
        if (mTextures[texNo] == nullptr || mTextures[texNo]->getTexInfo() == nullptr) {
            break;
        }
    }

    if (texNo < id) {
        return false;
    }

    if (texNo == 4) {
        return false;
    }

    if (mTextures[3] && mTextures[3]->getTexInfo() == nullptr) {
        if (mUndeleteFlag & 0x8) {
            delete mTextures[3];
        }

        mUndeleteFlag &= ~0x8;
    }

    for (u8 i = 3; i > id; i--) {
        mTextures[i]   = mTextures[i - 1];
        mPalettes[i]   = mPalettes[i - 1];
        mTexIndices[i] = mTexIndices[i - 1];
    }

    mTextures[id]   = tex;
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    shiftDeleteFlag(id, true);
    mUndeleteFlag &= ~(1 << id);
    return true;
}

bool J2DTevBlock4::setTexture(u32 id, const ResTIMG* timg)
{
    if (id >= 4) {
        return false;
    }

    u8 tlutid = 0;
    if (timg && timg->mPaletteFormat) {
        u8 local_44 = 0;
        for (int i = 0; i < 4; i++) {
            if (i == id) {
                continue;
            }
            if (!mTextures[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTextures[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->mPaletteFormat) {
                continue;
            }
            GXTlut tlutName = mTextures[i]->getTlutName();
            u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (bVar1 < 4) {
                local_44 |= 1 << bVar1;
            }
        }
        for (u8 i = 0; i < 4; i++) {
            if ((local_44 & 1 << i) == 0) {
                tlutid = i;
                break;
            }
        }
    }

    if (mTextures[id] == nullptr) {
        if (timg) {
            mTextures[id] = new JUTTexture(timg, tlutid);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        } else {
            mUndeleteFlag &= ~(1 << id);
        }
    } else if (mUndeleteFlag & (1 << id)) {
        if (timg != nullptr) {
            mTextures[id]->storeTIMG(timg, tlutid);
        } else {
            delete mTextures[id];
            mTextures[id] = nullptr;
            mUndeleteFlag &= ~(1 << id);
        }
    } else {
        mTextures[id] = nullptr;

        if (timg != nullptr) {
            mTextures[id] = new JUTTexture(timg, 0);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        }
    }

    delete mPalettes[id];
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock4::setTexture(u32 index, JUTTexture* texture)
{
    if (index >= 4) {
        return false;
    }
    if ((mUndeleteFlag & 1 << index) != 0) {
        delete mTextures[index];
    }
    mTextures[index] = texture;
    mUndeleteFlag &= ~(1 << index);
    delete mPalettes[index];
    mPalettes[index]   = nullptr;
    mTexIndices[index] = 0xFFFF;
    return true;
}

bool J2DTevBlock4::removeTexture(u32 id)
{
    if (id >= 4) {
        return false;
    }

    if (mUndeleteFlag & (1 << id)) {
        delete mTextures[id];
    }
    delete mPalettes[id];

    for (u32 i = id; i < 3; i++) {
        mTextures[i]   = mTextures[i + 1];
        mPalettes[i]   = mPalettes[i + 1];
        mTexIndices[i] = mTexIndices[i + 1];
    }

    mTextures[3]   = nullptr;
    mPalettes[3]   = nullptr;
    mTexIndices[3] = -1;
    shiftDeleteFlag(id, false);
    return true;
}

bool J2DTevBlock4::setFont(ResFONT* font)
{
    if (font == nullptr) {
        return false;
    }
    JUTResFont* jutFont = new JUTResFont(font, nullptr);
    if (jutFont == nullptr) {
        return false;
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
    mFont         = jutFont;
    mUndeleteFlag = mUndeleteFlag & 0x7F | 0x80;
    return true;
}

bool J2DTevBlock4::setFont(JUTFont* font)
{
    if (font == nullptr) {
        return false;
    }
    if ((mUndeleteFlag & 0x80) != 0) {
        delete mFont;
    }
    mFont = static_cast<JUTResFont*>(font);
    mUndeleteFlag &= 0x7F;
    return true;
}

bool J2DTevBlock4::setPalette(u32 index, const ResTLUT* lut)
{
    if (index >= 4) {
        return false;
    }
    if (lut) {
        if (mPalettes[index] == nullptr) {
            mPalettes[index] = new JUTPalette((GXTlut)index, const_cast<ResTLUT*>(lut));
            if (mPalettes[index] == nullptr) {
                return false;
            }
        } else {
            mPalettes[index]->storeTLUT(GX_TLUT0, const_cast<ResTLUT*>(lut));
        }
        if (mTextures[index]) {
            mTextures[index]->attachPalette(mPalettes[index]);
        }
    } else {
        delete mPalettes[index];
        mPalettes[index] = nullptr;
    }
    return true;
}

void J2DTevBlock4::shiftDeleteFlag(u8 p1, bool p2)
{
    u8 fontFlag = mUndeleteFlag & 0x80;
    mUndeleteFlag &= 0x7F;
    if (p2) {
        u32 v1        = ((1 << p1) - 1);
        mUndeleteFlag = (mUndeleteFlag & v1) | ((mUndeleteFlag & ~v1) << 1);
    } else {
        mUndeleteFlag = (mUndeleteFlag & (1 << p1) - 1) | ((mUndeleteFlag & ~((1 << (p1 + 1)) - 1)) >> 1);
    }
    mUndeleteFlag = mUndeleteFlag | fontFlag;
}

void J2DTevBlock4::setGX()
{
    for (int i = 0; i < 4; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), mOrders[i].getTexCoord(), mOrders[i].getTexMap(), mOrders[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mColors[i]);
    }
    GXSetNumTevStages(mStageNum);
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mStages[i].getColorA()), GXTevColorArg(mStages[i].getColorB()),
                        GXTevColorArg(mStages[i].getColorC()), GXTevColorArg(mStages[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mStages[i].getAlphaA()), GXTevAlphaArg(mStages[i].getAlphaB()),
                        GXTevAlphaArg(mStages[i].getAlphaC()), GXTevAlphaArg(mStages[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mStages[i].getCOp()), GXTevBias(mStages[i].getCBias()), GXTevScale(mStages[i].getCScale()),
                        mStages[i].getCClamp(), GXTevRegID(mStages[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mStages[i].getAOp(), GXTevBias(mStages[i].getABias()), GXTevScale(mStages[i].getAScale()),
                        mStages[i].getAClamp(), GXTevRegID(mStages[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mKColorSels[i] != 0xff ? GXTevKColorSel(mKColorSels[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mKAlphaSels[i] != 0xff ? GXTevKAlphaSel(mKAlphaSels[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mKColors[i]);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mStages[i].getRasSel()), GXTevSwapSel(mStages[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mSwapModeTables[i].getR()), GXTevColorChan(mSwapModeTables[i].getG()),
                              GXTevColorChan(mSwapModeTables[i].getB()), GXTevColorChan(mSwapModeTables[i].getA()));
    }
    for (u8 i = 0; i < mStageNum; i++) {
        mIndStages[i].load(i);
    }
}

void J2DTevBlock4::loadTexture(_GXTexMapID id, u32 index)
{
    if (index < 4 && mTextures[index] && mTextures[index]->mTexInfo) {
        mTextures[index]->load(id);
    }
}

J2DTevBlock8::J2DTevBlock8()
    : J2DTevBlock()
    , mOrders()
    , mColors()
    , mStages()
    , mKColors()
    , mSwapModeTables()
    , mIndStages()
{
    for (int i = 0; i < 8; i++) {
        mTextures[i] = nullptr;
    }
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock8::~J2DTevBlock8()
{
    for (int i = 0; i < 8; i++) {
        if ((mUndeleteFlag & 1 << i) != 0) {
            delete mTextures[i];
        }
        delete mPalettes[i];
    }
    if (mFontUndeleteFlag != 0) {
        delete mFont;
    }
}

void J2DTevBlock8::initialize()
{
    for (int i = 0; i < 8; i++) {
        mTexIndices[i] = -1;
    }
    mFontNo = -1;
    for (int i = 0; i < 8; i++) {
        mOrders[i].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    }
    for (int i = 0; i < 4; i++) {
        mColors[i] = j2dDefaultTevColor;
    }
    mStageNum = 1;
    for (int i = 0; i < 8; i++) {
        mStages[i].setStageNo(i);
    }
    for (int i = 0; i < 4; i++) {
        mKColors[i] = j2dDefaultTevKColor;
    }
    for (int i = 0; i < 8; i++) {
        mKColorSels[i] = -1;
        mKAlphaSels[i] = -1;
    }
    for (int i = 0; i < 4; i++) {
        mSwapModeTables[i] = j2dDefaultTevSwapModeTable;
    }
    for (int i = 0; i < 8; i++) {
        mIndStages[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }
    for (int i = 0; i < 8; i++) {
        mPalettes[i] = nullptr;
    }
    mFont = nullptr;
    mFontUndeleteFlag = 0;
}

bool J2DTevBlock8::prepareTexture(u8 count)
{
    for (u8 i = 0; i < count; i++) {
        if (i >= 8) {
            return false;
        }
        if (mTextures[i] == nullptr) {
            mTextures[i] = new JUTTexture();
            if (mTextures[i] == nullptr) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }
    return true;
}

bool J2DTevBlock8::insertTexture(u32 id, const ResTIMG* timg, JUTPalette* palette)
{
    if (id >= 8 || !timg) {
        return false;
    }
    u8 idx;
    for (idx = 0; idx < 8; idx++) {
        if (!mTextures[idx] || !mTextures[idx]->getTexInfo()) {
            break;
        }
    }
    if (idx < id) {
        return false;
    }
    if (idx == 8) {
        return false;
    }
    u8 local_43 = 0;
    if (timg->mPaletteFormat && !palette) {
        u8 local_44 = 0;
        for (int i = 0; i < 7; i++) {
            if (!mTextures[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTextures[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->mPaletteFormat) {
                continue;
            }
            GXTlut tlutName = mTextures[i]->getTlutName();
            u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (bVar1 < 8) {
                local_44 |= 1 << bVar1;
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if ((local_44 & 1 << i) == 0) {
                local_43 = i;
                break;
            }
        }
    }
    if (!mTextures[idx]) {
        JUTTexture* texture = new JUTTexture(timg, local_43);
        if (!texture) {
            return false;
        }
        if (palette) {
            texture->storeTIMG(timg, palette);
        }
        for (u8 i = 7; i > id; i--) {
            mTextures[i]   = mTextures[i - 1];
            mPalettes[i]   = mPalettes[i - 1];
            mTexIndices[i] = mTexIndices[i - 1];
        }
        mTextures[id] = texture;
        shiftDeleteFlag(id, true);
        mUndeleteFlag |= 1 << id;
    } else {
        JUTTexture* texture = mTextures[idx];
        if (!palette) {
            texture->storeTIMG(timg, local_43);
        } else {
            texture->storeTIMG(timg, palette);
        }
        bool local_38[4];
        for (u8 i = 0; i < 8; i++) {
            local_38[i] = (mUndeleteFlag & 1 << i);
        }
        for (u8 n = idx; n > id; n--) {
            mTextures[n]   = mTextures[n - 1];
            mPalettes[n]   = mPalettes[n - 1];
            mTexIndices[n] = mTexIndices[n - 1];
            local_38[n]    = local_38[n - 1];
        }
        mTextures[id] = texture;
        mUndeleteFlag = 0;
        for (u8 i = 0; i < 8; i++) {
            if (local_38[i]) {
                mUndeleteFlag |= 1 << i;
            }
        }
        mUndeleteFlag |= 1 << id;
    }
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock8::insertTexture(u32 id, JUTTexture* tex)
{
    if (id >= 8 || tex == nullptr) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 8; texNo++) {
        if (mTextures[texNo] == nullptr || mTextures[texNo]->getTexInfo() == nullptr) {
            break;
        }
    }

    if (texNo < id) {
        return false;
    }

    if (texNo == 8) {
        return false;
    }

    if (mTextures[7] && mTextures[7]->getTexInfo() == nullptr) {
        if (mUndeleteFlag & 0x80) {
            delete mTextures[7];
        }

        mUndeleteFlag &= 0x7F;
    }

    for (u8 i = 7; i > id; i--) {
        mTextures[i]   = mTextures[i - 1];
        mPalettes[i]   = mPalettes[i - 1];
        mTexIndices[i] = mTexIndices[i - 1];
    }

    mTextures[id]   = tex;
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    shiftDeleteFlag(id, true);
    mUndeleteFlag &= ~(1 << id);
    return true;
}

bool J2DTevBlock8::setTexture(u32 id, const ResTIMG* timg)
{
    if (id >= 8) {
        return false;
    }

    u8 tlutid = 0;
    if (timg && timg->mPaletteFormat) {
        u8 local_44 = 0;
        for (int i = 0; i < 8; i++) {
            if (i == id) {
                continue;
            }
            if (!mTextures[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTextures[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->mPaletteFormat) {
                continue;
            }
            GXTlut tlutName = mTextures[i]->getTlutName();
            u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (bVar1 < 8) {
                local_44 |= 1 << bVar1;
            }
        }
        for (u8 i = 0; i < 4; i++) {
            if ((local_44 & 1 << i) == 0) {
                tlutid = i;
                break;
            }
        }
    }

    if (mTextures[id] == nullptr) {
        if (timg) {
            mTextures[id] = new JUTTexture(timg, tlutid);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        } else {
            mUndeleteFlag &= ~(1 << id);
        }
    } else if (mUndeleteFlag & (1 << id)) {
        if (timg != nullptr) {
            mTextures[id]->storeTIMG(timg, tlutid);
        } else {
            delete mTextures[id];
            mTextures[id] = nullptr;
            mUndeleteFlag &= ~(1 << id);
        }
    } else {
        mTextures[id] = nullptr;

        if (timg != nullptr) {
            mTextures[id] = new JUTTexture(timg, 0);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        }
    }

    delete mPalettes[id];
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock8::setTexture(u32 index, JUTTexture* texture)
{
    if (index >= 8) {
        return false;
    }
    if ((mUndeleteFlag & 1 << index) != 0) {
        delete mTextures[index];
    }
    mTextures[index] = texture;
    mUndeleteFlag &= ~(1 << index);
    delete mPalettes[index];
    mPalettes[index]   = nullptr;
    mTexIndices[index] = 0xFFFF;
    return true;
}

bool J2DTevBlock8::removeTexture(u32 id)
{
    if (id >= 8) {
        return false;
    }

    if (mUndeleteFlag & (1 << id)) {
        delete mTextures[id];
    }
    delete mPalettes[id];

    for (u32 i = id; i < 7; i++) {
        mTextures[i]   = mTextures[i + 1];
        mPalettes[i]   = mPalettes[i + 1];
        mTexIndices[i] = mTexIndices[i + 1];
    }

    mTextures[7]   = nullptr;
    mPalettes[7]   = nullptr;
    mTexIndices[7] = -1;
    shiftDeleteFlag(id, false);
    return true;
}

bool J2DTevBlock8::setFont(ResFONT* font)
{
    if (font == nullptr) {
        return false;
    }
    JUTResFont* jutFont = new JUTResFont(font, nullptr);
    if (jutFont == nullptr) {
        return false;
    }
    if (mFontUndeleteFlag != 0) {
        delete mFont;
    }
    mFont             = jutFont;
    mFontUndeleteFlag = 1;
    return true;
}

bool J2DTevBlock8::setFont(JUTFont* font)
{
    if (font == nullptr) {
        return false;
    }
    if (mFontUndeleteFlag != 0) {
        delete mFont;
    }
    mFont             = static_cast<JUTResFont*>(font);
    mFontUndeleteFlag = 0;
    return true;
}

bool J2DTevBlock8::setPalette(u32 index, const ResTLUT* lut)
{
    if (index >= 8) {
        return false;
    }
    if (lut != nullptr) {
        if (mPalettes[index] == nullptr) {
            mPalettes[index] = new JUTPalette((GXTlut)index, const_cast<ResTLUT*>(lut));
            if (mPalettes[index] == nullptr) {
                return false;
            }
        } else {
            mPalettes[index]->storeTLUT(GX_TLUT0, const_cast<ResTLUT*>(lut));
        }
        if (mTextures[index] != nullptr) {
            mTextures[index]->attachPalette(mPalettes[index]);
        }
    } else {
        delete mPalettes[index];
        mPalettes[index] = nullptr;
    }
    return true;
}

void J2DTevBlock8::shiftDeleteFlag(u8 flag, bool direction)
{
    if (direction) {
        u32 v1        = ((1 << flag) - 1);
        mUndeleteFlag = (mUndeleteFlag & v1) | ((mUndeleteFlag & ~v1) << 1);
    } else {
        mUndeleteFlag = (mUndeleteFlag & (1 << flag) - 1) | ((mUndeleteFlag & ~((1 << (flag + 1)) - 1)) >> 1);
    }
}

void J2DTevBlock8::setGX()
{
    for (int i = 0; i < 8; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), mOrders[i].getTexCoord(), mOrders[i].getTexMap(), mOrders[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mColors[i]);
    }
    GXSetNumTevStages(mStageNum);
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mStages[i].getColorA()), GXTevColorArg(mStages[i].getColorB()),
                        GXTevColorArg(mStages[i].getColorC()), GXTevColorArg(mStages[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mStages[i].getAlphaA()), GXTevAlphaArg(mStages[i].getAlphaB()),
                        GXTevAlphaArg(mStages[i].getAlphaC()), GXTevAlphaArg(mStages[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mStages[i].getCOp()), GXTevBias(mStages[i].getCBias()), GXTevScale(mStages[i].getCScale()),
                        mStages[i].getCClamp(), GXTevRegID(mStages[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mStages[i].getAOp(), GXTevBias(mStages[i].getABias()), GXTevScale(mStages[i].getAScale()),
                        mStages[i].getAClamp(), GXTevRegID(mStages[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mKColorSels[i] != 0xff ? GXTevKColorSel(mKColorSels[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mKAlphaSels[i] != 0xff ? GXTevKAlphaSel(mKAlphaSels[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mKColors[i]);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mStages[i].getRasSel()), GXTevSwapSel(mStages[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mSwapModeTables[i].getR()), GXTevColorChan(mSwapModeTables[i].getG()),
                              GXTevColorChan(mSwapModeTables[i].getB()), GXTevColorChan(mSwapModeTables[i].getA()));
    }
    for (u8 i = 0; i < mStageNum; i++) {
        mIndStages[i].load(i);
    }
}

void J2DTevBlock8::loadTexture(_GXTexMapID id, u32 index)
{
    if (index < 8 && mTextures[index] != nullptr && mTextures[index]->mTexInfo != nullptr) {
        mTextures[index]->load(id);
    }
}

J2DTevBlock16::J2DTevBlock16()
    : J2DTevBlock()
    , mOrders()
    , mColors()
    , mStages()
    , mKColors()
    , mSwapModeTables()
    , mIndStages()
{
    for (int i = 0; i < 8; i++) {
        mTextures[i] = nullptr;
    }
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock16::~J2DTevBlock16()
{
    for (int i = 0; i < 8; i++) {
        if ((mUndeleteFlag & 1 << i) != 0) {
            delete mTextures[i];
        }
        delete mPalettes[i];
    }
    if (mFontUndeleteFlag != 0) {
        delete mFont;
    }
}

void J2DTevBlock16::initialize()
{
    for (int i = 0; i < 8; i++) {
        mTexIndices[i] = -1;
    }
    mFontNo = -1;
    for (int i = 0; i < 16; i++) {
        mOrders[i].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    }
    for (int i = 0; i < 4; i++) {
        mColors[i] = j2dDefaultTevColor;
    }
    mStageNum = 1;
    for (int i = 0; i < 16; i++) {
        mStages[i].setStageNo(i);
    }
    for (int i = 0; i < 4; i++) {
        mKColors[i] = j2dDefaultTevKColor;
    }
    for (int i = 0; i < 16; i++) {
        mKColorSels[i] = -1;
        mKAlphaSels[i] = -1;
    }
    for (int i = 0; i < 4; i++) {
        mSwapModeTables[i] = j2dDefaultTevSwapModeTable;
    }
    for (int i = 0; i < 16; i++) {
        mIndStages[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }
    for (int i = 0; i < 8; i++) {
        mPalettes[i] = nullptr;
    }
    mFont = nullptr;
    mFontUndeleteFlag = 0;
}

bool J2DTevBlock16::prepareTexture(u8 count)
{
    for (u8 i = 0; i < count; i++) {
        if (i >= 8) {
            return false;
        }
        if (mTextures[i] == nullptr) {
            mTextures[i] = new JUTTexture();
            if (mTextures[i] == nullptr) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }
    return true;
}

bool J2DTevBlock16::insertTexture(u32 id, const ResTIMG* timg, JUTPalette* palette)
{
    if (id >= 8 || !timg) {
        return false;
    }
    u8 idx;
    for (idx = 0; idx < 8; idx++) {
        if (!mTextures[idx] || !mTextures[idx]->getTexInfo()) {
            break;
        }
    }
    if (idx < id) {
        return false;
    }
    if (idx == 8) {
        return false;
    }
    u8 local_43 = 0;
    if (timg->mPaletteFormat && !palette) {
        u8 local_44 = 0;
        for (int i = 0; i < 7; i++) {
            if (!mTextures[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTextures[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->mPaletteFormat) {
                continue;
            }
            GXTlut tlutName = mTextures[i]->getTlutName();
            u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (bVar1 < 8) {
                local_44 |= 1 << bVar1;
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if ((local_44 & 1 << i) == 0) {
                local_43 = i;
                break;
            }
        }
    }
    if (!mTextures[idx]) {
        JUTTexture* texture = new JUTTexture(timg, local_43);
        if (!texture) {
            return false;
        }
        if (palette) {
            texture->storeTIMG(timg, palette);
        }
        for (u8 i = 7; i > id; i--) {
            mTextures[i]   = mTextures[i - 1];
            mPalettes[i]   = mPalettes[i - 1];
            mTexIndices[i] = mTexIndices[i - 1];
        }
        mTextures[id] = texture;
        shiftDeleteFlag(id, true);
        mUndeleteFlag |= 1 << id;
    } else {
        JUTTexture* texture = mTextures[idx];
        if (!palette) {
            texture->storeTIMG(timg, local_43);
        } else {
            texture->storeTIMG(timg, palette);
        }
        bool local_38[4];
        for (u8 i = 0; i < 8; i++) {
            local_38[i] = (mUndeleteFlag & 1 << i);
        }
        for (u8 n = idx; n > id; n--) {
            mTextures[n]   = mTextures[n - 1];
            mPalettes[n]   = mPalettes[n - 1];
            mTexIndices[n] = mTexIndices[n - 1];
            local_38[n]    = local_38[n - 1];
        }
        mTextures[id] = texture;
        mUndeleteFlag = 0;
        for (u8 i = 0; i < 8; i++) {
            if (local_38[i]) {
                mUndeleteFlag |= 1 << i;
            }
        }
        mUndeleteFlag |= 1 << id;
    }
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock16::insertTexture(u32 id, JUTTexture* tex)
{
    if (id >= 8 || tex == nullptr) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 8; texNo++) {
        if (mTextures[texNo] == nullptr || mTextures[texNo]->getTexInfo() == nullptr) {
            break;
        }
    }

    if (texNo < id) {
        return false;
    }

    if (texNo == 8) {
        return false;
    }

    if (mTextures[7] != nullptr && mTextures[7]->getTexInfo() == nullptr) {
        if (mUndeleteFlag & 0x80) {
            delete mTextures[7];
        }

        mUndeleteFlag &= 0x7F;
    }

    for (u8 i = 7; i > id; i--) {
        mTextures[i]   = mTextures[i - 1];
        mPalettes[i]   = mPalettes[i - 1];
        mTexIndices[i] = mTexIndices[i - 1];
    }

    mTextures[id]   = tex;
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    shiftDeleteFlag(id, true);
    mUndeleteFlag &= ~(1 << id);
    return true;
}

bool J2DTevBlock16::setTexture(u32 id, const ResTIMG* timg)
{
    if (id >= 8) {
        return false;
    }

    u8 tlutid = 0;
    if (timg && timg->mPaletteFormat) {
        u8 local_44 = 0;
        for (int i = 0; i < 8; i++) {
            if (i == id) {
                continue;
            }
            if (!mTextures[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTextures[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->mPaletteFormat) {
                continue;
            }
            GXTlut tlutName = mTextures[i]->getTlutName();
            u8 bVar1 = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (bVar1 < 8) {
                local_44 |= 1 << bVar1;
            }
        }
        for (u8 i = 0; i < 4; i++) {
            if ((local_44 & 1 << i) == 0) {
                tlutid = i;
                break;
            }
        }
    }

    if (mTextures[id] == nullptr) {
        if (timg) {
            mTextures[id] = new JUTTexture(timg, tlutid);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        } else {
            mUndeleteFlag &= ~(1 << id);
        }
    } else if (mUndeleteFlag & (1 << id)) {
        if (timg != nullptr) {
            mTextures[id]->storeTIMG(timg, tlutid);
        } else {
            delete mTextures[id];
            mTextures[id] = nullptr;
            mUndeleteFlag &= ~(1 << id);
        }
    } else {
        mTextures[id] = nullptr;

        if (timg != nullptr) {
            mTextures[id] = new JUTTexture(timg, 0);

            if (mTextures[id] == nullptr) {
                return false;
            }

            mUndeleteFlag |= (1 << id);
        }
    }

    delete mPalettes[id];
    mPalettes[id]   = nullptr;
    mTexIndices[id] = -1;
    return true;
}

bool J2DTevBlock16::setTexture(u32 index, JUTTexture* texture)
{
    if (index >= 8) {
        return false;
    }
    if ((mUndeleteFlag & 1 << index) != 0) {
        delete mTextures[index];
    }
    mTextures[index] = texture;
    mUndeleteFlag &= ~(1 << index);
    delete mPalettes[index];
    mPalettes[index]   = nullptr;
    mTexIndices[index] = 0xFFFF;
    return true;
}

bool J2DTevBlock16::removeTexture(u32 id)
{
    if (id >= 8) {
        return false;
    }

    if (mUndeleteFlag & (1 << id)) {
        delete mTextures[id];
    }
    delete mPalettes[id];

    for (u32 i = id; i < 7; i++) {
        mTextures[i]   = mTextures[i + 1];
        mPalettes[i]   = mPalettes[i + 1];
        mTexIndices[i] = mTexIndices[i + 1];
    }

    mTextures[7]   = nullptr;
    mPalettes[7]   = nullptr;
    mTexIndices[7] = -1;
    shiftDeleteFlag(id, false);
    return true;
}

bool J2DTevBlock16::setFont(ResFONT* font)
{
    if (font == nullptr) {
        return false;
    }
    JUTResFont* jutFont = new JUTResFont(font, nullptr);
    if (jutFont == nullptr) {
        return false;
    }
    if (mFontUndeleteFlag != 0) {
        delete mFont;
    }
    mFont             = jutFont;
    mFontUndeleteFlag = 1;
    return true;
}

bool J2DTevBlock16::setFont(JUTFont* font)
{
    if (font == nullptr) {
        return false;
    }
    if (mFontUndeleteFlag != 0) {
        delete mFont;
    }
    mFont             = static_cast<JUTResFont*>(font);
    mFontUndeleteFlag = 0;
    return true;
}

bool J2DTevBlock16::setPalette(u32 index, const ResTLUT* lut)
{
    if (index >= 8) {
        return false;
    }
    if (lut != nullptr) {
        if (mPalettes[index] == nullptr) {
            mPalettes[index] = new JUTPalette((GXTlut)index, const_cast<ResTLUT*>(lut));
            if (mPalettes[index] == nullptr) {
                return false;
            }
        } else {
            mPalettes[index]->storeTLUT(GX_TLUT0, const_cast<ResTLUT*>(lut));
        }
        if (mTextures[index] != nullptr) {
            mTextures[index]->attachPalette(mPalettes[index]);
        }
    } else {
        delete mPalettes[index];
        mPalettes[index] = nullptr;
    }
    return true;
}

void J2DTevBlock16::shiftDeleteFlag(u8 flag, bool direction)
{
    if (direction) {
        u32 v1        = ((1 << flag) - 1);
        mUndeleteFlag = (mUndeleteFlag & v1) | ((mUndeleteFlag & ~v1) << 1);
    } else {
        mUndeleteFlag = (mUndeleteFlag & (1 << flag) - 1) | ((mUndeleteFlag & ~((1 << (flag + 1)) - 1)) >> 1);
    }
}

void J2DTevBlock16::setGX()
{
    for (int i = 0; i < 8; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), mOrders[i].getTexCoord(), mOrders[i].getTexMap(), mOrders[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mColors[i]);
    }
    GXSetNumTevStages(mStageNum);
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mStages[i].getColorA()), GXTevColorArg(mStages[i].getColorB()),
                        GXTevColorArg(mStages[i].getColorC()), GXTevColorArg(mStages[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mStages[i].getAlphaA()), GXTevAlphaArg(mStages[i].getAlphaB()),
                        GXTevAlphaArg(mStages[i].getAlphaC()), GXTevAlphaArg(mStages[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mStages[i].getCOp()), GXTevBias(mStages[i].getCBias()), GXTevScale(mStages[i].getCScale()),
                        mStages[i].getCClamp(), GXTevRegID(mStages[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mStages[i].getAOp(), GXTevBias(mStages[i].getABias()), GXTevScale(mStages[i].getAScale()),
                        mStages[i].getAClamp(), GXTevRegID(mStages[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mKColorSels[i] != 0xff ? GXTevKColorSel(mKColorSels[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mKAlphaSels[i] != 0xff ? GXTevKAlphaSel(mKAlphaSels[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mKColors[i]);
    }
    for (int i = 0; i < mStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mStages[i].getRasSel()), GXTevSwapSel(mStages[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mSwapModeTables[i].getR()), GXTevColorChan(mSwapModeTables[i].getG()),
                              GXTevColorChan(mSwapModeTables[i].getB()), GXTevColorChan(mSwapModeTables[i].getA()));
    }
    for (u8 i = 0; i < mStageNum; i++) {
        mIndStages[i].load(i);
    }
}

void J2DTevBlock16::loadTexture(_GXTexMapID id, u32 index)
{
    if (index < 8 && mTextures[index] && mTextures[index]->mTexInfo) {
        mTextures[index]->load(id);
    }
}

void J2DIndBlockFull::initialize()
{
    mTexStageNum = 0;
    for (int i = 0; i < ARRAY_SIZE(mTexOrders); i++) {
        mTexOrders[i].mOrder = j2dDefaultIndTexOrderNull;
    }
    for (int i = 0; i < 3; i++) {
        mTexMtxes[i].mMtxInfo = j2dDefaultIndTexMtxInfo;
    }
    for (int i = 0; i < ARRAY_SIZE(mTexCoordScales); i++) {
        mTexCoordScales[i].mScaleInfo = j2dDefaultIndTexCoordScaleInfo;
    }
}

void J2DIndBlockFull::setGX()
{
    GXSetNumIndStages(mTexStageNum);
    for (u8 i = 0; i < mTexStageNum; i++) {
        mTexMtxes[i].load(i);
    }
    for (u8 i = 0; i < mTexStageNum; i++) {
        mTexCoordScales[i].load(i);
    }
    for (u8 i = 0; i < mTexStageNum; i++) {
        mTexOrders[i].load(i);
    }
}

void J2DPEBlock::initialize()
{
    mAlphaComp = J2DAlphaComp();
    mBlendInfo.setBlendInfo(j2dDefaultBlendInfo);
    mDither = j2dDefaultDither;
}

void J2DPEBlock::setGX()
{
    GXSetAlphaCompare(GXCompare(mAlphaComp.getComp0()), mAlphaComp.getRef0(), GXAlphaOp(mAlphaComp.getOp()),
                      GXCompare(mAlphaComp.getComp1()), mAlphaComp.getRef1());
    GXSetBlendMode(GXBlendMode(mBlendInfo.getType()), GXBlendFactor(mBlendInfo.getSrcFactor()), GXBlendFactor(mBlendInfo.getDstFactor()),
                   GXLogicOp(mBlendInfo.getOp()));
    GXSetDither(mDither);
}
