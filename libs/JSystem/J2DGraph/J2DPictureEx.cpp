#include "dolphin/gx.h"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/mtx.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "JSystem/J2D/J2DMaterial.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DTypes.h"
#include "JSystem/J3D/J3DAnmVtxColor.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "types.h"

J2DPictureEx::J2DPictureEx()
    : J2DPicture()
{
    initialize(0x1100000);
}

J2DPictureEx::J2DPictureEx(const ResTIMG* img, u32 matFlag)
    : J2DPicture()
{
    initialize(matFlag);
    append(img, 1.0f);
}

J2DPictureEx::J2DPictureEx(const char* fileName, u32 matFlag)
    : J2DPicture()
{
    initialize(matFlag);
    append(fileName, 1.0f);
}

void J2DPictureEx::initiate(const ResTIMG* img, const ResTLUT* lut)
{
    if (mMaterial->mTexGenBlock.mTexGenNum != 0) {
        return;
    }
    if (append(img, 1.0f) && lut && !mMaterial->mTevBlock->getPalette(0)) {
        mMaterial->mTevBlock->setPalette(0, lut);
    }
}

void J2DPictureEx::initialize(u32 matFlag)
{
    mBloBlockType   = 'PIC2';
    mMaterial       = new J2DMaterial(matFlag);
    mWasInitialized = true;
    mMaterialNum    = 0xFFFF;
    mMaterialID     = 0xFFFF;
    for (int i = 0; i < 4; i++) {
        _170[i] = 0xFFFF;
    }

    for (int i = 0; i < 4; i++) {
        mExBlendColorRatio[i] = 1.0f;
        mExBlendAlphaRatio[i] = 1.0f;
    }
    setTexCoord(nullptr, J2DBIND_Unk15, J2DMIRROR_Unk0, false);
    mAnmVisibility = nullptr;
    mAnmVtxColor   = nullptr;
}

J2DPictureEx::J2DPictureEx(J2DPane* parent, JSURandomInputStream* input, u32 flags, J2DMaterial* materials)
{
    mAnmVisibility   = nullptr;
    mAnmVtxColor     = nullptr;
    int origPosition = input->getPosition();

    u32 info[2];
    input->read(info, 8);
    mBloBlockType = info[0];

    int newPosition = input->getPosition();
    u32 nextInfo[2];
    input->peek(nextInfo, 8);
    makePaneExStream(parent, input);
    input->seek(newPosition + nextInfo[1], SEEK_SET);
    J2DScrnBlockPictureParameter trailer;
    input->read(&trailer, sizeof(J2DScrnBlockPictureParameter));
    mMaterialNum = trailer.mMaterialNum;
    mMaterialID  = trailer.mMaterialID;

    for (int i = 0; i < 4; i++) {
        _170[i]       = trailer._08[i];
        mTexCoords[i] = trailer.mTexCoords[i];
        mCornerColors[i] = trailer.mCornerColor[i];
    }

    input->seek(origPosition + info[1], SEEK_SET);
    mMaterial = nullptr;
    if (mMaterialID != 0xFFFF) {
        mMaterial                    = &(materials[mMaterialID]);
        materials[mMaterialID].mPane = this;
    }
    mWasInitialized = false;
    mAlpha          = 255;
    rewriteAlpha();
    for (int i = 0; i < 4; i++) {
        mBlendColorRatio[i] = 1.0f;
        mBlendAlphaRatio[i] = 1.0f;
    }
    for (int i = 0; i < 4; i++) {
        mExBlendColorRatio[i] = 1.0f;
        mExBlendAlphaRatio[i] = 1.0f;
    }
}

J2DPictureEx::J2DPictureEx(u64 id, const JGeometry::TBox2f& bounds, const ResTIMG* img, u32 matFlag)
    : J2DPicture(id, bounds)
{
    mBloBlockType = 'PIC2';
    initialize(matFlag);
    append(img, 1.0f);
}

J2DPictureEx::J2DPictureEx(u64 id, const JGeometry::TBox2f& bounds, const char* fileName, u32 matFlag)
    : J2DPicture(id, bounds)
{
    mBloBlockType = 'PIC2';
    initialize(matFlag);
    append(fileName, 1.0f);
}

J2DPictureEx::~J2DPictureEx()
{
    if (mWasInitialized) {
        delete mMaterial;
    }
}

bool J2DPictureEx::prepareTexture(u8 texNum)
{
    if (!mMaterial->getTevBlock()->prepareTexture(texNum)) {
        return false;
    }

    for (u8 i = 0; i < texNum; i++) {
        J2DTexMtx texMtx;
        mMaterial->getTexGenBlock()->setTexMtx(i, texMtx);
    }
    return true;
}

void J2DPictureEx::drawSelf(f32 x, f32 y, Mtx* texMtx)
{
    if (!mMaterial) {
        return;
    }
    mMaterial->setGX();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    drawFullSet(x, y, getWidth(), getHeight(), texMtx);
}

void J2DPictureEx::drawFullSet(f32 x, f32 y, f32 width, f32 height, Mtx* texMtx)
{
    drawTexCoord(mBounds.i.x, mBounds.i.y, width, height, mTexCoords[0].x, mTexCoords[0].y, mTexCoords[1].x, mTexCoords[1].y,
                 mTexCoords[2].x, mTexCoords[2].y, mTexCoords[3].x, mTexCoords[3].y, texMtx);
}

void J2DPictureEx::drawTexCoord(f32 x, f32 y, f32 width, f32 height, s16 xTex0, s16 yTex0, s16 xTex1, s16 yTex1, s16 xTex2, s16 yTex2,
                                s16 xTex3, s16 yTex3, Mtx* texMtx)
{
    f32 x2 = x + width;
    f32 y2 = y + height;
    Mtx v1;
    PSMTXConcat(*texMtx, mGlobalMtx, v1);
    if (mMaterial && mMaterial->mIsVisible == 0) {
        return;
    }
    GXLoadPosMtxImm(v1, 0);
    JUTColor color1(mCornerColors[0]);
    JUTColor color2(mCornerColors[1]);
    JUTColor color3(mCornerColors[2]);
    JUTColor color4(mCornerColors[3]);

    if (mMaterial) {
        if ((mMaterial->mColorBlock.mChannels[1].getMatSrc()) == 1) {
            if (mMaterial->getMaterialAlphaCalc() == 1) {
                color1.a = (color1.a * mColorAlpha) / 255;
                color2.a = (color2.a * mColorAlpha) / 255;
                color3.a = (color3.a * mColorAlpha) / 255;
                color4.a = (color4.a * mColorAlpha) / 255;            
            }
        } else if (mIsInfluencedAlpha) {
            GXSetChanMatColor(GX_ALPHA0, JUTColor(mColorAlpha));
        }
    }
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 8);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    f32 z = 0.0f;
    // bottom left
    GXPosition3f32(x, y, z);
    GXColor1u32(color1);
    GXTexCoord2s16(xTex0, yTex0);

    // bottom right
    GXPosition3f32(x2, y, z);
    GXColor1u32(color2);
    GXTexCoord2s16(xTex1, yTex1);
    
    // top left
    GXPosition3f32(x2, y2, z);
    GXColor1u32(color4);
    GXTexCoord2s16(xTex3, yTex3);

    // top right
    GXPosition3f32(x, y2, z);
    GXColor1u32(color3);
    GXTexCoord2s16(xTex2, yTex2);
    GXEnd();

    
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 15);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

bool J2DPictureEx::append(const ResTIMG* img, JUTPalette* palette, f32 blendRatio)
{
    return (!mMaterial) ? false : insert(img, palette, mMaterial->mTexGenBlock.mTexGenNum, blendRatio);
}

bool J2DPictureEx::append(const char* p1, JUTPalette* palette, f32 blendRatio)
{
    return (!mMaterial) ? false : insert(p1, palette, mMaterial->mTexGenBlock.mTexGenNum, blendRatio);
}

bool J2DPictureEx::append(JUTTexture* texture, f32 blendRatio)
{
    return (!mMaterial) ? false : insert(texture, mMaterial->mTexGenBlock.mTexGenNum, blendRatio);
}

bool J2DPictureEx::insert(const ResTIMG* img, JUTPalette* palette, u8 index, f32 blendRatio)
{
    if (!img) {
        return false;
    }
    if (!isInsert(index)) {
        return false;
    }
    insertCommon(index, blendRatio);
    mMaterial->mTevBlock->insertTexture(index, img, palette);
    return true;
}

bool J2DPictureEx::insert(const char* filename, JUTPalette* palette, u8 index, f32 blendRatio)
{
    return insert((ResTIMG*)J2DScreen::getNameResource(filename), palette, index, blendRatio);
}

bool J2DPictureEx::insert(JUTTexture* texture, u8 index, f32 blendRatio)
{
    if (!texture) {
        return false;
    }
    if (!isInsert(index)) {
        return false;
    }
    insertCommon(index, blendRatio);
    mMaterial->mTevBlock->insertTexture(index, texture);
    return true;
}

void J2DPictureEx::insertCommon(u8 index, f32 blendRatio)
{
    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    mMaterial->getTevBlock()->getMaxStage();
    u8 stageNum = mMaterial->getTevBlock()->getTevStageNum();

    bool check;
    if (texGenNum <= 1) {
        check = stageNum != 1;
    } else {
        check = (stageNum != (texGenNum + 1));
    }

    shiftSetBlendRatio(index, blendRatio, true, true);
    shiftSetBlendRatio(index, blendRatio, false, true);

    texGenNum++;

    mMaterial->getTexGenBlock()->setTexGenNum(texGenNum);

    J2DTexCoordInfo coordInfo;
    coordInfo.mTexGenType = 1;
    coordInfo.mTexGenSrc = 4;

    // some loop copy nonsense like this
    for (int i = (texGenNum - 1); i >= index; i--) {
        coordInfo.mTexGenMtx = (i * 3 + 30);
        J2DTexCoord coord(coordInfo);
        mMaterial->getTexGenBlock()->setTexCoord((u8)i, coord);
    }

    for (int i = texGenNum - 1; i > index; i--) {
        mMaterial->getTexGenBlock()->setTexMtx((u8)i, mMaterial->getTexGenBlock()->getTexMtx((u8)(i - 1)));
    }

    J2DTexMtx texMtx;
    mMaterial->getTexGenBlock()->setTexMtx(index, texMtx);

    if (texGenNum == 1) {
        stageNum = 1;
    } else {
        stageNum = texGenNum + (check ? 2 : 1);
    }

    mMaterial->getTevBlock()->setTevStageNum(stageNum);

    setTevOrder(texGenNum, stageNum, check);
    setTevStage(texGenNum, stageNum, check);
    setTevKColor(texGenNum);
    setTevKColorSel(texGenNum);
    setTevKAlphaSel(texGenNum);
}

bool J2DPictureEx::isInsert(u8 p1) const
{
    if (!mMaterial) {
        return false;
    }
    if (!mMaterial->mTevBlock) {
        return false;
    }
    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    if (!((texGenNum < 8 && p1 < 8) && p1 <= texGenNum)) {
        return false;
    }
    u32 maxStage = mMaterial->getTevBlock()->getMaxStage();
    if ((u8)maxStage <= 2 && texGenNum != 0) {
        return false;
    }
    u8 tevStageNum = mMaterial->getTevBlock()->getTevStageNum();
    if ((u8)maxStage == tevStageNum && texGenNum != 0) {
        return false;
    }
    return true;
}

BOOL J2DPictureEx::remove(u8 id)
{
    if (!isRemove(id)) {
        return FALSE;
    }
    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 tevStageNum = (u8)mMaterial->getTevBlock()->getTevStageNum();
    bool v1 = (tevStageNum == (texGenNum + 1)) ? false : true;
    shiftSetBlendRatio(id, 0.0f, true, false);
    shiftSetBlendRatio(id, 0.0f, false, false);
    texGenNum--;
    mMaterial->getTexGenBlock()->setTexGenNum(texGenNum);

    // it needs to stop optimising texGenNum - 1 down into a new variable and dynamically do it
    mMaterial->getTevBlock()->removeTexture(id);
    if (texGenNum != 1) {
        tevStageNum = texGenNum + (v1 ? 2 : 1);
    } else {
        tevStageNum = v1 ? 2 : 1;
    }
    mMaterial->getTevBlock()->setTevStageNum(tevStageNum);
    setTevOrder(texGenNum, tevStageNum, v1);
    setTevStage(texGenNum, tevStageNum, v1);
    setTevKColor(texGenNum);
    setTevKColorSel(texGenNum);
    setTevKAlphaSel(texGenNum);
    return TRUE;
}

BOOL J2DPictureEx::remove()
{
    if (!mMaterial) {
        return FALSE;
    }

    return (!mMaterial->getTevBlock()) ? FALSE : remove((u8)mMaterial->getTexGenBlock()->getTexGenNum() - 1);
}

BOOL J2DPictureEx::remove(JUTTexture* texture)
{
    if (mMaterial == nullptr) {
        return FALSE;
    }

    if (mMaterial->getTevBlock() == nullptr) {
        return FALSE;
    }

    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 i;
    for (i = 0; i < texGenNum; i++) {
        if (mMaterial->getTevBlock()->getTexture(i) == texture)
            break;
    }
    return remove(i);
}

bool J2DPictureEx::isRemove(u8 index) const
{
    if (!mMaterial) {
        return false;
    }
    if (!mMaterial->mTevBlock) {
        return false;
    }
    u8 texGenNum = mMaterial->mTexGenBlock.mTexGenNum;
    if (texGenNum <= index || texGenNum == 1) {
        return false;
    }
    return true;
}

void J2DPictureEx::draw(f32 p1, f32 p2, u8 index, bool p4, bool p5, bool p6)
{
    if (mMaterial && mMaterial->mTevBlock && mIsVisible && index < mMaterial->mTexGenBlock.mTexGenNum) {
        JUTTexture* texture = mMaterial->mTevBlock->getTexture(index);
        if (texture) {
            draw(p1, p2, texture->getWidth(), texture->getHeight(), p4, p5, p6);
        }
    }
}

void J2DPictureEx::draw(f32 x, f32 y, f32 width, f32 height, bool p5, bool p6, bool p7)
{
    if (!isVisible()) {
        return;
    }

    if (!mMaterial) {
        return;
    }

    if (!mMaterial->getTevBlock()) {
        return;
    }

    if (mMaterial->getTexGenBlock()->mTexGenNum == 0) {
        return;
    }

    mMaterial->setGX();
    makeMatrix(x, y, 0.0f, 0.0f);
    GXLoadPosMtxImm(mPositionMtx, 0);
    GXSetCurrentMtx(0);

    if (!mMaterial->isVisible()) {
        return;
    }

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    mColorAlpha = mAlpha;
    JUTColor colors[4];
    for (int i = 0; i < 4; i++) {
        colors[i] = mCornerColors[i];
    }

    if ((int)mMaterial->mMaterialAlphaCalc == 1) {
        for (int i = 0; i < 4; i++) {
            colors[i].a = (colors[i].a * mColorAlpha) / 255;
        }
    }

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    f32 z = 0.0f;
    GXPosition3f32(z, z, z);
    GXColor1u32(colors[0]);

    if (!p7) {
        GXTexCoord2s16(p5 ? 0x8000 : 0, p6 ? 0x8000 : 0);
    } else {
        GXTexCoord2s16(p5 ? 0x8000 : 0, p6 ? 0 : 0x8000);
    }

    GXPosition3f32(width, z, z);
    GXColor1u32(colors[1]);

    if (!p7) {
        GXTexCoord2s16(p5 ? 0 : 0x8000, p6 ? 0x8000 : 0);
    } else {
        GXTexCoord2s16(p5 ? 0x8000 : 0, p6 ? 0x8000 : 0);
    }


    GXPosition3f32(width, height, z);
    GXColor1u32(colors[3]);

    if (!p7) {
        GXTexCoord2s16(p5 ? 0 : 0x8000, p6 ? 0 : 0x8000);
    } else {
        GXTexCoord2s16(p5 ? 0 : 0x8000, p6 ? 0x8000 : 0);
    }


    GXPosition3f32(z, height, z);
    GXColor1u32(colors[2]);

    if (!p7) {
        GXTexCoord2s16(p5 ? 0x8000 : 0, p6 ? 0 : 0x8000);
    } else {
        GXTexCoord2s16(p5 ? 0 : 0x8000, p6 ? 0 : 0x8000);
    }


    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }

    GXSetNumIndStages(0);

    for (int i = 0; i < 16; i++) {
        GXSetTevDirect(GXTevStageID(i));
    }

    Mtx posMtx;
    PSMTXIdentity(posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DPictureEx::drawOut(f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6)
{
    if (mMaterial && mMaterial->mTevBlock && mIsVisible) {
        JUTTexture* texture = mMaterial->mTevBlock->getTexture(0);
        if (texture) {
            drawOut(JGeometry::TBox2f(p1, p2, p1 + p3, p2 + p4),
                    JGeometry::TBox2f(p5, p6, p5 + texture->getWidth(), p6 + texture->getHeight()));
        }
    }
}

void J2DPictureEx::drawOut(const JGeometry::TBox2<f32>& boundBox, const JGeometry::TBox2<f32>& texBox)
{

    if (!mMaterial) {
        return;
    }

    if (!mMaterial->getTevBlock()) {
        return;
    }

    if (!isVisible()) {
        return;
    }
    mMaterial->setGX();

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    if (!mMaterial->isVisible()) {
        return;
    }
    mColorAlpha = mAlpha;
    JUTColor colors[4];
    for (int i = 0; i < 4; i++) {
        colors[i] = mCornerColors[i];
    }

    if ((int)mMaterial->mMaterialAlphaCalc == 1) {
        for (int i = 0; i < 4; i++) {
            colors[i].a = (colors[i].a * mColorAlpha) / 255;
        }
    }

    f32 texX0 = (boundBox.i.x - texBox.i.x) / texBox.getWidth();
    f32 texX1 = 1.0f + ((boundBox.f.x - texBox.f.x) / texBox.getWidth());
    f32 texY0 = (boundBox.i.y - texBox.i.y) / texBox.getHeight();
    f32 texY1 = 1.0f + ((boundBox.f.y - texBox.f.y) / texBox.getHeight());

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    f32 z = 0.0f;
    GXPosition3f32(boundBox.i.x, boundBox.i.y, z);
    GXColor1u32(colors[0]);
    GXPosition2f32(texX0, texY0);

    GXPosition3f32(boundBox.f.x, boundBox.i.y, z);
    GXColor1u32(colors[1]);
    GXPosition2f32(texX1, texY0);

    GXPosition3f32(boundBox.f.x, boundBox.f.y, z);
    GXColor1u32(colors[3]);
    GXPosition2f32(texX1, texY1);

    GXPosition3f32(boundBox.i.x, boundBox.f.y, z);
    GXColor1u32(colors[2]);
    GXPosition2f32(texX0, texY1);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U16, 15);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }

    GXSetNumIndStages(0);

    for (int i = 0; i < 16; i++) {
        GXSetTevDirect(GXTevStageID(i));
    }

    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DPictureEx::load(GXTexMapID p1, u8 p2)
{
    if (mMaterial && mMaterial->mTevBlock) {
        mMaterial->mTevBlock->loadTexture(p1, p2);
    }
}

void J2DPictureEx::setTevOrder(u8 p1, u8 p2, bool p3)
{
    u16 data[16];
    if (p1 == 1) {
        if (p3 == false) {
            data[0] = 4;
        } else {
            data[0] = 0x00FF;
            data[1] = 0xFF04;
        }
    } else {
        for (u8 i = 0; i < p1; i++) {
            data[i] = i * 0x100 | 0xFF;
        }
        if (p3 == false) {
            data[p1] = 0xFF04;
        } else {
            data[p1]     = 0xFFFF;
            data[p1 + 1] = 0xFF04;
        }
    }
    for (u8 i = 0; i < p2; i++) {
        J2DTevOrder order(data[i] >> 8, data[i] >> 8, data[i]);
        mMaterial->mTevBlock->setTevOrder(i, order);
    }
}

void J2DPictureEx::setTevStage(u8 p1, u8 p2, bool p3)
{
    J2DTevStage* stage;
    if (p1 == 1) {
        stage  = mMaterial->getTevBlock()->getTevStage(0);
        JUTTexture* texture = mMaterial->getTevBlock()->getTexture(0);
        bool v2             = false;
        if (texture != nullptr) {
            if (texture->getTexInfo() != nullptr) {
                if ((texture->getFormat() == (s8)GX_TF_I4 || texture->getFormat() == (s8)GX_TF_I8)
                    && texture->getTransparency() == (s8)0) {
                    v2 = true;
                }
            }
        }
        if (p3 == false) {
            setStage(stage, v2 ? Stage_1 : Stage_0);
        } else {
            setStage(stage, (v2 ? Stage_6 : Stage_5));
            stage = mMaterial->getTevBlock()->getTevStage((1));
            setStage(stage, Stage_4);
        }
    } else {
        if (p3 == false) {
            stage = mMaterial->getTevBlock()->getTevStage((0));
            setStage(stage, Stage_2);
            for (u8 i = 1; i < p1; i++) {
                stage = mMaterial->getTevBlock()->getTevStage((i));
                setStage(stage, Stage_3);
            }
            stage = mMaterial->getTevBlock()->getTevStage((p1));
            setStage(stage, Stage_4);
        } else {
            stage = mMaterial->getTevBlock()->getTevStage((0));
            setStage(stage, Stage_2);
            for (u8 i = 1; i < p1; i++) {
                stage = mMaterial->getTevBlock()->getTevStage((i));
                setStage(stage, Stage_3);
            }
            stage = mMaterial->getTevBlock()->getTevStage((p1));
            setStage(stage, Stage_7);
            stage = mMaterial->getTevBlock()->getTevStage((u8)(p1 + 1));
            setStage(stage, Stage_4);
        }
    }
}

void J2DPictureEx::setStage(J2DTevStage* tevStage, J2DPictureEx::stage_enum i)
{
    const u8 colorABCDs[8][4] = { { 15, 8, 10, 15 }, { 15, 8, 10, 15 }, { 15, 8, 14, 15 }, { 15, 8, 14, 0 },
                            { 15, 10, 0, 15 }, { 2, 4, 8, 15 },   { 2, 4, 8, 15 },   { 2, 4, 0, 15 } };
    const u8 alphaABCDs[8][4] = { { 7, 4, 5, 7 }, { 5, 7, 7, 7 }, { 7, 4, 6, 7 }, { 7, 4, 6, 0 },
                            { 7, 5, 0, 7 }, { 1, 2, 4, 7 }, { 7, 7, 7, 2 }, { 1, 2, 0, 7 } };
    const u8 ops[8][5]        = { { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
                     { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 0 } };

    tevStage->setTevColorAB(colorABCDs[i][0], colorABCDs[i][1]);
    tevStage->setTevColorCD(colorABCDs[i][2], colorABCDs[i][3]);
    tevStage->setTevColorOp(ops[i][0], ops[i][1], ops[i][2], ops[i][3], ops[i][4]);
    tevStage->setAlphaABCD(alphaABCDs[i][0], alphaABCDs[i][1], alphaABCDs[i][2], alphaABCDs[i][3]);
    tevStage->setTevAlphaOp(ops[i][0], ops[i][1], ops[i][2], ops[i][3], ops[i][4]);
}

void J2DPictureEx::setTevKColor(u8 p1)
{
    if (p1 == 1) {
        return;
    }

    f32 colorRatio = 0.0f;
    f32 alphaRatio = 0.0f;
    if (p1 < 4) {
        for (u8 i = 0; i < p1; i++) {
            colorRatio += mBlendColorRatio[i];
            alphaRatio += mBlendAlphaRatio[i];
        }
    } else {
        for (u8 i = 0; i < 4; i++) {
            colorRatio += mBlendColorRatio[i];
            alphaRatio += mBlendAlphaRatio[i];
        }

        for (u8 i = 0; i < (p1 - 4); i++) {
            colorRatio += mExBlendColorRatio[i];
            alphaRatio += mExBlendAlphaRatio[i];
        }
    }

    u8 colA[8];
    u8 colB[8];

    for (u8 i = 0; i < 8; i++) {
        colA[i] = 255;
        colB[i] = 255;
    }

    for (u8 i = 0; i < p1; i++) {
        if (i < 4) {
            colA[i] = (255.0f * mBlendColorRatio[i] / colorRatio);
            colB[i] = (255.0f * mBlendAlphaRatio[i] / alphaRatio);
        } else {
            colA[i] = (255.0f * mExBlendColorRatio[i - 4] / colorRatio);
            colB[i] = (255.0f * mExBlendAlphaRatio[i - 4] / alphaRatio);
        }
    }

    JUTColor jColors[4];
    jColors[3] = colA[0] | (colA[1] << 8 | (colA[3] << 24 | (colA[2] << 16)));
    jColors[2] = colA[4] | (colA[5] << 8 | (colA[7] << 24 | (colA[6] << 16)));
    jColors[1] = colB[0] | (colB[1] << 8 | (colB[3] << 24 | (colB[2] << 16)));
    jColors[0] = colB[4] | (colB[5] << 8 | (colB[7] << 24 | (colB[6] << 16)));

    for (u8 i = 0; i < 4; i++) {
        mMaterial->getTevBlock()->setTevKColor(i, jColors[i]);
    }
}

void J2DPictureEx::setTevKColorSel(u8 texNum)
{
    for (u8 i = 0; i < texNum; i++) {
        if (i < 4) {
            mMaterial->getTevBlock()->setTevKColorSel(i, 31 - 4 * i);
        } else {
            mMaterial->getTevBlock()->setTevKColorSel(i, 30 - 4 * (i - 4));
        }
    }
}

void J2DPictureEx::setTevKAlphaSel(u8 texNum)
{
    for (u8 i = 0; i < texNum; i++) {
        if (i < 4) {
            mMaterial->getTevBlock()->setTevKAlphaSel(i, 29 - 4 * i);
        } else {
            mMaterial->getTevBlock()->setTevKAlphaSel(i, 28 - 4 * (i - 4));
        }
    }
}

void J2DPictureEx::shiftSetBlendRatio(u8 id, f32 blendRatio, bool isColorRatio, bool isInsert)
{
    f32* ratio   = isColorRatio != 0 ? mBlendColorRatio : mBlendAlphaRatio;
    f32* exRatio = isColorRatio != 0 ? mExBlendColorRatio : mExBlendAlphaRatio;

    if (isInsert) {
        for (int i = 7; i > id; i--) {
            if (i > 4) {
                exRatio[i - 4] = exRatio[i - 5];
            } else if (i == 4) {
                exRatio[0] = ratio[3];
            } else {
                ratio[i] = ratio[i - 1];
            }
        }
        if (id >= 4) {
            exRatio[id - 4] = blendRatio;
        } else {
            ratio[id] = blendRatio;
        }
    } else {
        for (int i = id + 1; i < 8; i++) {
            if (i > 4) {
                exRatio[i - 5] = exRatio[i - 4];
            } else if (i == 4) {
                ratio[3] = exRatio[0];
            } else {
                ratio[i - 1] = ratio[i];
            }
        }
    }
}

void J2DPictureEx::setBlendColorRatio(f32 ratio0, f32 ratio1, f32 ratio2, f32 ratio3, f32 exRatio0, f32 exRatio1, f32 exRatio2,
                                      f32 exRatio3)
{
    if (mMaterial != nullptr) {
        mBlendColorRatio[0]   = ratio0;
        mBlendColorRatio[1]   = ratio1;
        mBlendColorRatio[2]   = ratio2;
        mBlendColorRatio[3]   = ratio3;
        mExBlendColorRatio[0] = exRatio0;
        mExBlendColorRatio[1] = exRatio1;
        mExBlendColorRatio[2] = exRatio2;
        mExBlendColorRatio[3] = exRatio3;
        setTevKColor(mMaterial->mTexGenBlock.mTexGenNum);
    }
}

void J2DPictureEx::setBlendAlphaRatio(f32 ratio0, f32 ratio1, f32 ratio2, f32 ratio3, f32 exRatio0, f32 exRatio1, f32 exRatio2,
                                      f32 exRatio3)
{
    if (mMaterial) {
        mBlendAlphaRatio[0]   = ratio0;
        mBlendAlphaRatio[1]   = ratio1;
        mBlendAlphaRatio[2]   = ratio2;
        mBlendAlphaRatio[3]   = ratio3;
        mExBlendAlphaRatio[0] = exRatio0;
        mExBlendAlphaRatio[1] = exRatio1;
        mExBlendAlphaRatio[2] = exRatio2;
        mExBlendAlphaRatio[3] = exRatio3;
        setTevKColor(mMaterial->getTexGenBlock()->getTexGenNum());
    }
}

ResTIMG* J2DPictureEx::changeTexture(const ResTIMG* resource, u8 id)
{
    if (mMaterial == nullptr || resource == nullptr) {
        return nullptr;
    }
    u32 texNum = mMaterial->getTexGenBlock()->getTexGenNum();
    if (id > texNum) {
        return nullptr;
    }

    u32 maxStage = mMaterial->getTevBlock()->getMaxStage();
    maxStage    = (u8)maxStage > 8 ? 8 : maxStage;

    if (id >= (u8)maxStage) {
        return nullptr;
    }

    if (id < texNum) {
        const ResTIMG* texInfo = getTexture(id)->getTexInfo();
        u8 usableID            = 0;
        if (resource->mPaletteFormat != 0) {
            usableID = getUsableTlut(id);
        }
        getTexture(id)->storeTIMG(resource, usableID);
        return (ResTIMG*)texInfo;
    }
    append(resource, 1.0f);
    return nullptr;
}

ResTIMG* J2DPictureEx::changeTexture(const char* fileName, u8 id)
{
    return changeTexture((ResTIMG*)J2DScreen::getNameResource(fileName), id);
}

ResTIMG* J2DPictureEx::changeTexture(const ResTIMG* resource, u8 id, JUTPalette* palette)
{
    if (mMaterial == nullptr || resource == nullptr) {
        return nullptr;
    }
    u32 texNum = mMaterial->getTexGenBlock()->getTexGenNum();
    if (id > texNum) {
        return nullptr;
    }
    u32 maxStage = mMaterial->getTevBlock()->getMaxStage();
    maxStage    = (u8)maxStage > 8 ? 8 : maxStage;

    if (id >= (u8)maxStage) {
        return nullptr;
    }
    if (id < texNum) {
        ResTIMG* texInfo = getTexture(id)->getTexInfo();
        GXTlut tlut         = GX_TLUT0;
        if (resource->mPaletteFormat != 0) {
            u8 usableID = getUsableTlut(id);
            tlut        = getTlutID(resource, usableID);
        }
        getTexture(id)->storeTIMG(resource, palette, tlut);
        return texInfo;
    }
    append(resource, palette, 1.0f);
    return nullptr;
}

ResTIMG* J2DPictureEx::changeTexture(const char* name, u8 p2, JUTPalette* palette)
{
    return changeTexture((ResTIMG*)J2DScreen::getNameResource(name), p2, palette);
}

JUTTexture* J2DPictureEx::getTexture(u8 id) const
{
    if (mMaterial == nullptr) {
        return nullptr;
    }

    if (mMaterial->getTevBlock() == nullptr) {
        return nullptr;
    }

    return mMaterial->getTevBlock()->getTexture(id);
}

u8 J2DPictureEx::getTextureCount() const
{
    if (mMaterial == nullptr) {
        return 0;
    }
    return mMaterial->mTexGenBlock.mTexGenNum;
}

bool J2DPictureEx::setBlack(JUTColor black)
{
    JUTColor oldBlack, oldWhite;
    if (!getBlackWhite(&oldBlack, &oldWhite)) {
        return false;
    }
    return setBlackWhite(black, oldWhite);
}

bool J2DPictureEx::setWhite(JUTColor white)
{
    JUTColor oldBlack, oldWhite;
    if (!getBlackWhite(&oldBlack, &oldWhite)) {
        return false;
    }
    return setBlackWhite(oldBlack, white);
}

bool J2DPictureEx::setBlackWhite(JUTColor black, JUTColor white)
{
    if (mMaterial == nullptr) {
        return false;
    }

    if (mMaterial->getTevBlock() == nullptr) {
        return false;
    }

    if (!isSetBlackWhite(black, white)) {
        return false;
    }

    bool isNotDefault = true;
    u8 texGenNum      = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 tevStageNum;
    if ((black == 0) && (white == 0xFFFFFFFF)) { // i.e. if setting to default colors
        isNotDefault = false;
    }
    if (texGenNum == 1) {
        tevStageNum = (isNotDefault) ? 2 : 1;
    } else {
        tevStageNum = texGenNum + ((isNotDefault) ? 2 : 1);
    }
    mMaterial->getTevBlock()->setTevStageNum(tevStageNum);
    setTevOrder(texGenNum, tevStageNum, isNotDefault);
    setTevStage(texGenNum, tevStageNum, isNotDefault);
    setTevKColor(texGenNum);
    setTevKColorSel(texGenNum);
    setTevKAlphaSel(texGenNum);
    if (isNotDefault) {
        J2DGXColorS10 local_38;
        local_38.r = black.r;
        local_38.g = black.g;
        local_38.b = black.b;
        local_38.a = black.a;
        mMaterial->getTevBlock()->setTevColor(0, local_38);
        local_38.r = white.r;
        local_38.g = white.g;
        local_38.b = white.b;
        local_38.a = white.a;
        mMaterial->getTevBlock()->setTevColor(1, local_38);
    }
    return true;
}

bool J2DPictureEx::getBlackWhite(JUTColor* outBlack, JUTColor* outWhite) const
{
    if (mMaterial == nullptr) {
        return false;
    }
    if (mMaterial->mTevBlock == nullptr) {
        return false;
    }
    u8 texGenNum   = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 tevStageNum = mMaterial->getTevBlock()->getTevStageNum();
    u8 texCheck    = (texGenNum == 1) ? tevStageNum != 1 : tevStageNum != (texGenNum + 1);

    *outBlack = 0;
    *outWhite = 0xFFFFFFFF;
    if (texCheck) {
        J2DGXColorS10 tevBlack, tevWhite;
        tevBlack  = *mMaterial->getTevBlock()->getTevColor(0);
        tevWhite  = *mMaterial->getTevBlock()->getTevColor(1);
        *outBlack = JUTColor(((u8)tevBlack.r << 0x18) | ((u8)tevBlack.g << 0x10) | ((u8)tevBlack.b << 8) | (u8)tevBlack.a);
        *outWhite = JUTColor(((u8)tevWhite.r << 0x18) | ((u8)tevWhite.g << 0x10) | ((u8)tevWhite.b << 8) | (u8)tevWhite.a);
    }
    return true;
}

bool J2DPictureEx::isSetBlackWhite(JUTColor black, JUTColor white) const
{
    if (black == 0 && white == 0xFFFFFFFF) {
        return true;
    }

    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    mMaterial->getTevBlock()->getTevStageNum();
    int maxStage = mMaterial->getTevBlock()->getMaxStage();
    if ((u8)maxStage == 16) {
        return true;
    }

    if ((u8)maxStage == 1) {
        return false;
    }
    u8 temp = (texGenNum == 1 ? 2 : (texGenNum + 2));
    return temp <= (u8)maxStage;
}

JUTColor J2DPictureEx::getBlack() const
{
    JUTColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return TCOLOR_BLACK_U32;
    }
    return black;
}

JUTColor J2DPictureEx::getWhite() const
{
    JUTColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return TCOLOR_WHITE_U32;
    }
    return white;
}

void J2DPictureEx::setAlpha(u8 alpha)
{
    mAlpha = alpha;
    if (mMaterial == nullptr) {
        return;
    }
    if (mMaterial->getColorBlock() == nullptr) {
        return;
    }
    mMaterial->getColorBlock()->mColors[0].a = alpha;
}

void J2DPictureEx::setCullBack(_GXCullMode cullMode)
{
    mCullMode = cullMode;
    if (mMaterial != nullptr) {
        mMaterial->mColorBlock.mCullMode = cullMode;
    }
    J2DPane::setCullBack(cullMode);
}

void J2DPictureEx::rewriteAlpha()
{
    if (mMaterial == nullptr) {
        return;
    }
    mAlpha = mMaterial->mColorBlock.mColors[0].a;
}

bool J2DPictureEx::isUsed(const ResTIMG* resource)
{
    if (mMaterial != nullptr && mMaterial->mTevBlock != nullptr) {
        for (u32 i = 0; i < 8; i++) {
            JUTTexture* texture = mMaterial->mTevBlock->getTexture(i);
            if (texture != nullptr && texture->mTexInfo == resource) {
                return true;
            }
        }
    }
    return J2DPane::isUsed(resource);
}

void J2DPictureEx::setAnimation(J2DAnmColor* animation)
{
    if (mMaterial != nullptr) {
        mMaterial->setAnimation(animation);
    }
}

void J2DPictureEx::setAnimation(J2DAnmTextureSRTKey* animation)
{
    if (mMaterial != nullptr) {
        mMaterial->setAnimation(animation);
    }
}

void J2DPictureEx::setAnimation(J2DAnmTexPattern* animation)
{
    if (mMaterial != nullptr) {
        mMaterial->setAnimation(animation);
    }
}

void J2DPictureEx::setAnimation(J2DAnmTevRegKey* animation)
{
    if (mMaterial != nullptr) {
        mMaterial->setAnimation(animation);
    }
}

void J2DPictureEx::setAnimation(J2DAnmVisibilityFull* animation) { mAnmVisibility = animation; }

void J2DPictureEx::setAnimation(J2DAnmVtxColor* animation)
{
    mAnmVtxColor = animation;
    _1A4         = 0;
    if (animation != nullptr) {
        u32 tableNum = animation->getAnmTableNum(0);
        for (u8 a = 0; a < 4; a++) {

            if (_170[a] == 0xFFFF) continue;
            
            for (u16 b = 0; b < tableNum; b++) {
                J3DAnmVtxColorIndexData* data = animation->getAnmVtxColorIndexData(0, b);
                u16 *idx = &animation->getVtxColorIndexPointer(0)[(u32)data->mData];
                for (u16 c = 0; c < data->mNum; c++) {
                    if (idx[c] == _170[a]) {
                        mAnmVtxColor = animation;
                        _1A4 |= 1 << a;
                        goto skip_loop2;
                    }
                }
            }
skip_loop2:;
        }
    }

    if (_1A4 == 0) {
        mAnmVtxColor = nullptr;
    }
}

const J2DAnmTransform* J2DPictureEx::animationPane(const J2DAnmTransform* animation)
{
    if (mAnmVisibility && mMaterialNum != 0xFFFF) {
        u8 vis;
        mAnmVisibility->getVisibility(mMaterialNum, &vis);
        if (vis) {
            show();
        } else {
            hide();
        }
    }

    if (mAnmVtxColor) {
        u32 num0 = mAnmVtxColor->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if (_1A4 & (1 << i)) {
                // inline?
                for (u16 j = 0; j < num0; j++) {
                    J3DAnmVtxColorIndexData* data = mAnmVtxColor->getAnmVtxColorIndexData(0, j);
                    u16 *colorPtr = &mAnmVtxColor->getVtxColorIndexPointer(0)[(u32)data->mData];
                    for (u16 k = 0; k < data->mNum; k++) {
                        if (_170[i] == colorPtr[k]) {
                            mAnmVtxColor->getColor(0, j, &mCornerColors[i]);
                            goto next_color;
                        }
                    }
                }
            }
        next_color:;
        }
    }

    J2DPane::animationPane(animation);
}

u8 J2DPictureEx::getUsableTlut(u8 id)
{
    u8 usedTexFlags = 0;
    u32 texNum      = mMaterial->getTexGenBlock()->getTexGenNum();
    for (u8 i = 0; i < texNum; i++) {
        JUTTexture* texture = mMaterial->getTevBlock()->getTexture(i);
        if (i != id && texture != nullptr) {
            const ResTIMG* img = texture->getTexInfo();

            if (img != nullptr && img->mPaletteFormat != 0) {
                int name   = texture->getTlutName();
                int tlutID = name >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0;

                u8 diffID = name - tlutID;
                if (diffID < 8) {
                    usedTexFlags |= (1 << diffID);
                }
            }
        }
    }

    for (u8 i = 0; i < 8; i++) {
        if (!(usedTexFlags & (1 << i))) {
            return i;
        }
    }

    return 0;
}
