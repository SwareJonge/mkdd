#include "dolphin/gx.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JGeometry.h"

J2DPicture::J2DPicture()
    : mPalette(nullptr)
{
    for (int i = 0; i < 4; i++) {
        mTextures[i] = nullptr;
    }
    mUsedTextureFlags = 0;
    mTextureCount     = 0;
    initinfo();
}

J2DPicture::J2DPicture(J2DPane* parent, JSURandomInputStream* input, JKRArchive* archive)
    : mPalette(nullptr)
{
    private_readStream(parent, input, archive);
}

J2DPicture::J2DPicture(J2DPane* parent, JSURandomInputStream* input, J2DMaterial* materials)
    : mPalette(nullptr)
{
    J2DScrnBlockHeader header;
    int headerPosition = input->getPosition();
    input->read(&header, sizeof(J2DScrnBlockHeader));
    mBloBlockType      = 'PIC1';
    int paneExPosition = input->getPosition();
    J2DScrnBlockHeader paneExHeader;
    input->peek(&paneExHeader, sizeof(J2DScrnBlockHeader));
    makePaneExStream(parent, input);
    input->seek(paneExPosition + paneExHeader.mBlockLength, SEEK_SET);

    J2DScrnBlockPictureParameter trailer;
    input->read(&trailer, sizeof(J2DScrnBlockPictureParameter));
    u16 v1 = trailer.mMaterialID;

    for (int i = 0; i < 4; i++) {
        mTexCoords[i]    = trailer.mTexCoords[i];
        mCornerColors[i] = JUTColor(GXCOLOR_AS_U32(trailer.mCornerColor[i]));
    }
    input->seek(headerPosition + header.mBlockLength, SEEK_SET);

    J2DMaterial* material = nullptr;
    if (v1 != 0xFFFF) {
        material = &materials[v1];
    }

    mAlpha = 0xFF;
    if (material != nullptr) {
        mAlpha = material->getColorBlock()->getMatColor(0)->a;
    }
    mBlack        = TCOLOR_BLACK_U32;
    mWhite        = TCOLOR_WHITE_U32;
    mTextureCount = 0;
    if (material && material->getTevBlock()) {
        u8 texGenNum   = material->getTexGenBlock()->getTexGenNum();
        u8 tevStageNum = material->getTevBlock()->getTevStageNum();
        if ((texGenNum == 1 && tevStageNum != 1) || (texGenNum != 1 && tevStageNum != texGenNum + 1)) {
            J2DGXColorS10 black;
            J2DGXColorS10 white;
            black = *material->getTevBlock()->getTevColor(0);
            white = *material->getTevBlock()->getTevColor(1);

            mBlack = JUTColor(RGBA_TO_U32(black.r, black.g, black.b, black.a));
            mWhite = JUTColor(RGBA_TO_U32(white.r, white.g, white.b, white.a));
        }
        mTextureCount = (texGenNum <= 4) ? texGenNum : 4;
    }
    mUsedTextureFlags = 0;
    for (u32 i = 0; i < 4; i++) {
        mTextures[i] = nullptr;
        if (material && material->getTevBlock()) {
            JUTTexture* texture = material->mTevBlock->getTexture(i);
            if (texture != nullptr) {
                mTextures[i]      = texture;
                mUsedTextureFlags = mUsedTextureFlags | (1 << i);
            }
        }
    }
    if (material && material->getTevBlock()) {
        material->mTevBlock->setUndeleteFlag(0xF0);
    }
    mPalette = nullptr;
    JUTColor v2;
    JUTColor v3;
    if (material && material->getTevBlock()) {
        v2 = *material->mTevBlock->getTevKColor(3);
        v3 = *material->mTevBlock->getTevKColor(1);
    }
    setBlendColorRatio(v2.a, v2.b, v2.g, v2.r, 1.0f, 1.0f, 1.0f, 1.0f);
    setBlendAlphaRatio(v3.a, v3.b, v3.g, v3.r, 1.0f, 1.0f, 1.0f, 1.0f);
}

J2DPicture::J2DPicture(const ResTIMG* img)
{
    for (int i = 0; i < 4; i++) {
        mTextures[i] = nullptr;
    }
    mUsedTextureFlags = 0;
    mTextureCount     = 0;
    if (img != nullptr) {
        append(img, 1.0f);
    }
    mPalette = nullptr;
    initinfo();
}

J2DPicture::J2DPicture(const char* fileName)
{
    for (int i = 0; i < 4; i++) {
        mTextures[i] = 0;
    }
    mUsedTextureFlags = 0;
    mTextureCount     = 0;
    if (fileName) {
        append(fileName, 1.0f);
    }
    mPalette = nullptr;
    initinfo();
}

J2DPicture::J2DPicture(JUTTexture* texture)
{
    for (int i = 0; i < 4; i++) {
        mTextures[i] = 0;
    }
    mUsedTextureFlags = 0;
    mTextureCount     = 0;
    if (texture) {
        append(texture, 1.0f);
    }
    mPalette = nullptr;
    initinfo();
}

J2DPicture::J2DPicture(u64 id, const JGeometry::TBox2f& bounds)
    : J2DPane(id, bounds)
    , mPalette(nullptr)
{
    initinfo();
    for (int i = 0; i < 4; i++) {
        mTextures[i] = 0;
    }
    mUsedTextureFlags = 0;
    mTextureCount     = 0;
    setBlendRatio(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void J2DPicture::private_readStream(J2DPane* parent, JSURandomInputStream* input, JKRArchive* archive)
{
    J2DScrnBlockHeader header;
    int headerPosition = input->getPosition();
    input->read(&header, sizeof(J2DScrnBlockHeader));
    mBloBlockType = header.mBloBlockType;
    makePaneStream(parent, input);

    ResTIMG* img;
    ResTLUT* lut;
    u32 v1 = 0;
    u8 v2  = input->readU8();

    img = (ResTIMG*)getPointer(input, 'TIMG', archive);
    lut = (ResTLUT*)getPointer(input, 'TLUT', archive);

    u8 v3 = input->read8b();
    v2 -= 3;
    if (v2 != 0) {
        v1 = input->read8b();
        v2--;
    }

    if (v2 != 0) {
        input->read8b();
        v2--;
    }

    mBlack = TCOLOR_BLACK_U32;
    mWhite = TCOLOR_WHITE_U32;

    if (v2 != 0) {
        mBlack = input->readU32();
        v2--;
    }

    if (v2 != 0) {
        mWhite = input->readU32();
        v2--;
    }

    setCornerColor(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    for (int i = 0; v2 != 0 && i < 4; i++) {
        mCornerColors[i] = input->readU32();
        v2--;
    }

    input->seek(headerPosition + header.mBlockLength, SEEK_SET);

    for (int i = 0; i < 4; i++) {
        mTextures[i] = nullptr;
    }
    mTextureCount     = 0;
    mUsedTextureFlags = 1;

    if (img) {
        mTextures[0] = new JUTTexture(img);
        mTextureCount++;
    }
    if (lut) {
        mPalette = new JUTPalette(GX_TLUT0, lut);
        mTextures[0]->attachPalette(mPalette);
    }

    bool check = (v1 >> 2) & 1;
    setTexCoord(mTextures[0], (J2DBinding)(u8)v3, (J2DMirror)(v1 & 3), check);

    setBlendRatio(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void J2DPicture::initiate(const ResTIMG* img, const ResTLUT* lut)
{
    private_initiate(img, lut);
    if (mTextures[0] == nullptr) {
        return;
    }
    place(JGeometry::TBox2f(0.0f, 0.0f, mTextures[0]->getWidth(), mTextures[0]->getHeight()));
}

void J2DPicture::private_initiate(const ResTIMG* timg, const ResTLUT* tlut)
{
    if (mTextureCount) {
        return;
    }

    if (timg) {
        if (!mTextures[0]) {
            mTextures[0] = new JUTTexture(timg, 0);
            if (mTextures[0]) {
                mTextureCount++;
                mUsedTextureFlags = (mUsedTextureFlags & 0xe) | 1;
            }
        } else {
            mTextures[0]->storeTIMG(timg, (u8)0);
            mTextureCount++;
            mUsedTextureFlags = (mUsedTextureFlags & 0xe) | 1;
        }
    }
    mPalette = nullptr;
    if (tlut && !mPalette) {
        mPalette = new JUTPalette(GX_TLUT0, const_cast<ResTLUT*>(tlut));
        if (mTextures[0]) {
            mTextures[0]->attachPalette(mPalette);
        }
    }
}

void J2DPicture::initinfo()
{
    mBloBlockType = 'PIC1';
    setTexCoord(nullptr, J2DBIND_Unk15, J2DMIRROR_Unk0, false);
    setBlendRatio(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    mBlack = TCOLOR_BLACK_U32;
    mWhite = TCOLOR_WHITE_U32;
    setCornerColor(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
}

J2DPicture::~J2DPicture()
{
    for (int i = 0; i < 4; i++) {
        if ((mUsedTextureFlags & 1 << i) != 0) {
            delete mTextures[i];
        }
    }
    delete mPalette;
}

bool J2DPicture::prepareTexture(u8 num)
{
    for (u8 i = 0; i < num; i++) {
        if (i >= 4) {
            return false;
        }
        if (!mTextures[i]) {
            mTextures[i] = new JUTTexture;
            if (!mTextures[i]) {
                return false;
            }
            mUsedTextureFlags |= 1 << i;
        }
    }
    return true;
}

bool J2DPicture::insert(const ResTIMG* timg, JUTPalette* palette, u8 id, f32 blendRatio)
{
    if (!timg || mTextureCount >= 4 || id >= 4 || id > mTextureCount) {
        return false;
    }

    u8 thing = 0;
    if (timg->mPaletteFormat && !palette) {
        thing = getUsableTlut(mTextureCount);
    }

    JUTTexture* texture;
    if (!mTextures[mTextureCount]) {
        texture = new JUTTexture(timg, thing);
        if (palette) {
            texture->storeTIMG(timg, palette);
        }
        for (u8 i = 3; i > id; i--) {
            mTextures[i]        = mTextures[i - 1];
            mBlendColorRatio[i] = mBlendColorRatio[i - 1];
            mBlendAlphaRatio[i] = mBlendAlphaRatio[i - 1];
        }

        mUsedTextureFlags = (mUsedTextureFlags & ((1 << id) - 1)) | ((mUsedTextureFlags & ~((1 << id) - 1)) * 2);
        mUsedTextureFlags |= (1 << id);
    } else {
        texture = mTextures[mTextureCount];
        if (palette == nullptr) {
            mTextures[mTextureCount]->storeTIMG(timg, thing);
        } else {
            mTextures[mTextureCount]->storeTIMG(timg, palette);
        }

        u8 tempFlags[4];
        for (u8 i = 0; i < 4; i++) {
            tempFlags[i] = (mUsedTextureFlags & (1 << i)) != 0;
        }

        for (u8 i = mTextureCount; i > id; i--) {
            mTextures[i]        = mTextures[i - 1];
            tempFlags[i]        = tempFlags[i - 1];
            mBlendColorRatio[i] = mBlendColorRatio[i - 1];
            mBlendAlphaRatio[i] = mBlendAlphaRatio[i - 1];
        }

        mUsedTextureFlags = 0;

        for (u8 i = 0; i < 4; i++) {
            if (tempFlags[i] != 0) {
                mUsedTextureFlags |= (1 << i);
            }
        }

        mUsedTextureFlags |= (1 << id);
    }

    mTextures[id]        = texture;
    mBlendColorRatio[id] = blendRatio;
    mBlendAlphaRatio[id] = blendRatio;

    if (mTextureCount == 0 && mTextures[0] != nullptr) {
        place(JGeometry::TBox2f(0.0f, 0.0f, mTextures[0]->getWidth(), mTextures[0]->getHeight()));
        setTexCoord(nullptr, J2DBIND_Unk15, J2DMIRROR_Unk0, false);
    }

    mTextureCount++;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}

bool J2DPicture::insert(const char* fileName, JUTPalette* palette, u8 id, f32 blendRatio)
{
    return insert((ResTIMG*)J2DScreen::getNameResource(fileName), palette, id, blendRatio);
}

bool J2DPicture::insert(JUTTexture* texture, u8 id, f32 blendRatio)
{
    if (!texture || mTextureCount >= 4 || id >= 4 || id > mTextureCount) {
        return false;
    }

    if (mTextures[3] != nullptr && mUsedTextureFlags & 8) {
        delete mTextures[3];
        mUsedTextureFlags &= 0x7;
    }

    for (u8 i = 3; i > id; i--) {
        mTextures[i]        = mTextures[i - 1];
        mBlendColorRatio[i] = mBlendColorRatio[i - 1];
        mBlendAlphaRatio[i] = mBlendAlphaRatio[i - 1];
    }

    mUsedTextureFlags = (mUsedTextureFlags & ((1 << id) - 1)) | ((mUsedTextureFlags & ~((1 << id) - 1)) * 2);
    mTextures[id]     = texture;
    mUsedTextureFlags &= ~(1 << id);

    mBlendColorRatio[id] = blendRatio;
    mBlendAlphaRatio[id] = blendRatio;

    if (mTextureCount == 0 && &mTextures[0] != nullptr) {
        place(JGeometry::TBox2<f32>(0.0f, 0.0f, mTextures[0]->getWidth(), mTextures[0]->getHeight()));
        setTexCoord(nullptr, J2DBIND_Unk15, J2DMIRROR_Unk0, false);
    }

    mTextureCount++;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}

int J2DPicture::remove(u8 id)
{
    if (mTextureCount <= id || mTextureCount == 1) {
        return false;
    }
    if ((mUsedTextureFlags & 1 << id) != 0) {
        delete mTextures[id];
    }
    for (u8 i = id; mTextureCount - 1 > i; i++) {
        mTextures[i]        = mTextures[i + 1];
        mBlendColorRatio[i] = mBlendColorRatio[i + 1];
        mBlendAlphaRatio[i] = mBlendAlphaRatio[i + 1];
    }
    mTextures[mTextureCount - 1] = nullptr;

    mUsedTextureFlags = (mUsedTextureFlags & ((1 << id) - 1)) | ((mUsedTextureFlags & ~((1 << (id + 1)) - 1)) >> 1);

    mTextureCount--;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}

int J2DPicture::remove(JUTTexture* texture)
{
    u8 i;
    for (i = 0; i < mTextureCount; i++) {
        if (mTextures[i] == texture) {
            break;
        }
    }
    return remove(i);
}

ResTIMG* J2DPicture::changeTexture(const ResTIMG* img, u8 id)
{
    if (id > mTextureCount || 4 <= id || img == nullptr) {
        return nullptr;
    }
    ResTIMG* result;
    if (id < mTextureCount) {
        JUTTexture* texture = getTexture(id);
        result              = texture->mTexInfo;
        u8 v1               = 0;
        if (img->mPaletteFormat != 0) {
            v1 = getUsableTlut(id);
        }
        getTexture(id)->storeTIMG(img, v1);
        return result;
    }
    append(img, 1.0f);
    return nullptr;
}

ResTIMG* J2DPicture::changeTexture(const char* fileName, u8 id)
{
    return changeTexture((ResTIMG*)J2DScreen::getNameResource(fileName), id);
}

ResTIMG* J2DPicture::changeTexture(const ResTIMG* timg, u8 id, JUTPalette* palette)
{
    if (id > mTextureCount || 4 <= id || timg == nullptr) {
        return nullptr;
    }

    ResTIMG* img = nullptr;
    if (id < mTextureCount) {
        JUTTexture* tex = getTexture(id);
        GXTlut tlut     = GX_TLUT0;
        img             = tex->mTexInfo;
        if (timg->mPaletteFormat != 0) {
            u8 tlutID = getUsableTlut(id);
            tlut      = getTlutID(timg, tlutID);
        }
        getTexture(id)->storeTIMG(timg, palette, tlut);
        return img;
    } else {
        append(timg, palette, 1.0f);
    }
    return img;
}

ResTIMG* J2DPicture::changeTexture(const char* fileName, u8 id, JUTPalette* palette)
{
    return changeTexture((ResTIMG*)J2DScreen::getNameResource(fileName), id, palette);
}

void J2DPicture::drawSelf(f32 x, f32 y)
{
    Mtx texMtx;
    PSMTXIdentity(texMtx);
    drawSelf(x, y, &texMtx);
}

void J2DPicture::drawSelf(f32 x, f32 y, Mtx* texMtx)
{
    if (mTextures[0] != nullptr && mTextureCount != 0) {
        drawFullSet(mGlobalBounds.i.x + x, mGlobalBounds.i.y + y, mBounds.getWidth(), mBounds.getHeight(), texMtx);
    }
}

void J2DPicture::drawFullSet(f32 x, f32 y, f32 width, f32 height, Mtx* texMtx)
{
    if (mTextures[0] != nullptr && mTextureCount != 0) {
        drawTexCoord(mBounds.i.x, mBounds.i.y, width, height, mTexCoords[0].x, mTexCoords[0].y, mTexCoords[1].x, mTexCoords[1].y,
                     mTexCoords[2].x, mTexCoords[2].y, mTexCoords[3].x, mTexCoords[3].y, texMtx);
    }
}

void J2DPicture::draw(f32 x, f32 y, f32 width, f32 height, bool a1, bool a2, bool a3)
{
    if (isVisible() && mTextureCount != 0 && mTextures[0]) {

        for (u8 i = 0; i < mTextureCount; i++) {
            load(i);
        }
        GXSetNumTexGens(mTextureCount);
        mColorAlpha = mAlpha;
        JUTColor cornerColors[4];
        getNewColor(cornerColors);
        setTevMode();
        makeMatrix(x, y, 0.0f, 0.0f);
        GXLoadPosMtxImm(mPositionMtx, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

        JGeometry::TVec2<s16> coords[4];
        setTexCoord(coords, (mTextures[0]), J2DBIND_Unk15, J2DMirror(a1 << 1 | a2), a3);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S16, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        f32 zero = 0.0f;

        // bottom left
        GXPosition3f32(zero, zero, zero);
        GXColor1u32(cornerColors[0]);
        GXPosition2u16(coords[0].x, coords[0].y);

        // bottom right
        GXPosition3f32(width, zero, zero);
        GXColor1u32(cornerColors[1]);
        GXPosition2u16(coords[1].x, coords[1].y);

        // top right
        GXPosition3f32(width, height, zero);
        GXColor1u32(cornerColors[3]);
        GXPosition2u16(coords[3].x, coords[3].y);

        // top left
        GXPosition3f32(zero, height, zero);
        GXColor1u32(cornerColors[2]);
        GXPosition2u16(coords[2].x, coords[2].y);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U16, 0xf);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        Mtx mtx;
        PSMTXIdentity(mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    }
}

void J2DPicture::drawOut(const JGeometry::TBox2f& posBox, const JGeometry::TBox2f& texRect)
{
    if (isVisible() && mTextureCount != 0 && mTextures[0]) {

        for (u8 i = 0; i < mTextureCount; i++) {
            load(i);
        }
        GXSetNumTexGens(mTextureCount);

        f32 s0 = (posBox.i.x - texRect.i.x) / texRect.getWidth();
        f32 s1 = (posBox.f.x - texRect.f.x) / texRect.getWidth() + 1.0f;

        f32 t0 = (posBox.i.y - texRect.i.y) / texRect.getHeight();
        f32 t1 = (posBox.f.y - texRect.f.y) / texRect.getHeight() + 1.0f;

        mColorAlpha = mAlpha;
        JUTColor cornerColors[4];
        getNewColor(cornerColors);
        setTevMode();

        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

        f32 zero = 0.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        // bottom left
        GXPosition3f32((s16)posBox.i.x, (s16)posBox.i.y, zero);
        GXColor1u32(cornerColors[0]);
        GXTexCoord2f32(s0, t0);

        // bottom right
        GXPosition3f32((s16)posBox.f.x, (s16)posBox.i.y, zero);
        GXColor1u32(cornerColors[1]);
        GXTexCoord2f32(s1, t0);

        // top right
        GXPosition3f32((s16)posBox.f.x, (s16)posBox.f.y, zero);
        GXColor1u32(cornerColors[3]);
        GXTexCoord2f32(s1, t1);

        // top left
        GXPosition3f32((s16)posBox.i.x, (s16)posBox.f.y, zero);
        GXColor1u32(cornerColors[2]);
        GXTexCoord2f32(s0, t1);
        GXEnd();

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 15);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    }
}

void J2DPicture::drawTexCoord(f32 x, f32 y, f32 width, f32 height, s16 btmLeftS, s16 btmLeftT, s16 btmRightS, s16 btmRightT, s16 topLeftS,
                              s16 topLeftT, s16 topRightS, s16 topRightT, Mtx* texMtx)
{
    if (mTextureCount != 0) {
        for (u8 i = 0; i < mTextureCount; i++) {
            load(i);
        }

        f32 maxX = x + width;
        f32 maxY = y + height;

        GXSetNumTexGens(mTextureCount);

        JUTColor cornerColors[4];

        Mtx outputMtx;

        getNewColor(cornerColors);
        setTevMode();
        PSMTXConcat(*texMtx, mGlobalMtx, outputMtx);

        GXLoadPosMtxImm(outputMtx, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        f32 zero = 0.0f;
        // bottom left
        GXPosition3f32(x, y, zero);
        GXColor1u32(cornerColors[0]);
        GXTexCoord2s16(btmLeftS, btmLeftT);

        // bottom right
        GXPosition3f32(maxX, y, zero);
        GXColor1u32(cornerColors[1]);
        GXTexCoord2s16(btmRightS, btmRightT);

        // top right
        GXPosition3f32(maxX, maxY, zero);
        GXColor1u32(cornerColors[3]);
        GXTexCoord2s16(topRightS, topRightT);

        // top left
        GXPosition3f32(x, maxY, zero);
        GXColor1u32(cornerColors[2]);
        GXTexCoord2s16(topLeftS, topLeftT);

        GXEnd();

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    }
}

void J2DPicture::setTevMode()
{
    u8 i;
    for (i = 0; i < mTextureCount; i++) {
        GXSetTevOrder(GXTevStageID(i), GXTexCoordID(i), GXTexMapID(i), GX_COLOR_NULL);
    }

    GXSetTevColor(GX_TEVREG2, JUTColor(0xFFFFFFFF));
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);

    if (mTextures[0] == nullptr) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    } else if (mTextures[0]->getTransparency()) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    } else {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    }

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKColor(GX_KCOLOR0, mBlendColor);
    GXSetTevKColor(GX_KCOLOR2, mBlendAlpha);

    for (i = 1; i < mTextureCount; i++) {
        GXSetTevKColorSel(GXTevStageID(i), GXTevKColorSel(0x20 - i * 4));
        GXSetTevKAlphaSel(GXTevStageID(i), GXTevKAlphaSel(0x22 - i * 4));
        GXSetTevColorIn(GXTevStageID(i), GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);

        if (mTextures[i] == nullptr) {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_A2, GX_CA_KONST, GX_CA_ZERO);
        } else if (mTextures[i]->getTransparency()) {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
        } else {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_A2, GX_CA_KONST, GX_CA_ZERO);
        }

        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }

    if (mBlack != 0 || mWhite != 0xFFFFFFFF) {
        GXSetTevOrder(GXTevStageID(i), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, mBlack);
        GXSetTevColor(GX_TEVREG1, mWhite);
        GXSetTevColorIn(GXTevStageID(i), GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GXTevStageID(i), GX_CA_A0, GX_CA_A1, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        i++;
    }

    if (mColorAlpha != 0xFF || mCornerColors[0] != 0xFFFFFFFF || mCornerColors[1] != 0xFFFFFFFF || mCornerColors[2] != 0xFFFFFFFF
        || mCornerColors[3] != 0xFFFFFFFF) {
        GXSetTevOrder(GXTevStageID(i), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GXTevStageID(i), GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GXTevStageID(i), GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        i++;
    }

    GXSetNumTevStages(i);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);

    for (i = 0; i < mTextureCount; i++) {
        GXSetTexCoordGen2(GXTexCoordID(i), GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    }

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumIndStages(0);

    for (int i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
}

void J2DPicture::setBlendColorRatio(f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6, f32 p7, f32 p8)
{
    mBlendColorRatio[0] = p1;
    mBlendColorRatio[1] = p2;
    mBlendColorRatio[2] = p3;
    mBlendColorRatio[3] = p4;
    setBlendKonstColor();
}

void J2DPicture::setBlendAlphaRatio(f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6, f32 p7, f32 p8)
{
    mBlendAlphaRatio[0] = p1;
    mBlendAlphaRatio[1] = p2;
    mBlendAlphaRatio[2] = p3;
    mBlendAlphaRatio[3] = p4;
    setBlendKonstAlpha();
}

void J2DPicture::setBlendKonstColor()
{
    int color = 0;

    for (u8 i = 1; i < mTextureCount; i++) {
        f32 prevRatios = 0.0f;
        for (u8 j = 0; j < i; j++) {
            prevRatios += mBlendColorRatio[j];
        }
        f32 currRatios = prevRatios + mBlendColorRatio[i];
        if (currRatios != 0.0f) {
            color |= (u8)(255.0f * (1.0f - prevRatios / currRatios)) << (i - 1) * 8;
        }
    }
    mBlendColor = color;
}

void J2DPicture::setBlendKonstAlpha()
{
    int alpha = 0;

    for (u8 i = 1; i < mTextureCount; i++) {
        f32 prevRatios = 0.0f;
        for (u8 j = 0; j < i; j++) {
            prevRatios += mBlendAlphaRatio[j];
        }
        f32 currRatios = prevRatios + mBlendAlphaRatio[i];
        if (currRatios != 0.0f) {
            alpha |= (u8)(255.0f * (1.0f - prevRatios / currRatios)) << (i - 1) * 8;
        }
    }
    mBlendAlpha = alpha;
}

void J2DPicture::getNewColor(JUTColor* newColor)
{
    newColor[0] = mCornerColors[0];
    newColor[1] = mCornerColors[1];
    newColor[2] = mCornerColors[2];
    newColor[3] = mCornerColors[3];

    if (mColorAlpha != 255) {
        newColor[0].a = newColor[0].a * mColorAlpha / 255;
        newColor[1].a = newColor[1].a * mColorAlpha / 255;
        newColor[2].a = newColor[2].a * mColorAlpha / 255;
        newColor[3].a = newColor[3].a * mColorAlpha / 255;
    }
}

void J2DPicture::setTexCoord(const JGeometry::TVec2<s16>* coords)
{
    for (int i = 0; i < 4; i++) {
        mTexCoords[i].x = coords[i].x;
        mTexCoords[i].y = coords[i].y;
    }
}

void J2DPicture::setTexCoord(const JUTTexture* texture, J2DBinding binding, J2DMirror mirror, bool doRotate90)
{
    setTexCoord(mTexCoords, texture, binding, mirror, doRotate90);
}

void J2DPicture::setTexCoord(JGeometry::TVec2s* texCoords, const JUTTexture* texture, J2DBinding binding, J2DMirror mirror, bool doRotate90)
{
    bool bindLeft;
    bool bindRight;
    bool bindTop;
    bool bindBottom;

    if (!doRotate90) {
        bindLeft   = (mirror & J2DMIRROR_X) ? (bool)(binding & J2DBIND_Right) : (bool)(binding & J2DBIND_Left);
        bindRight  = (mirror & J2DMIRROR_X) ? (bool)(binding & J2DBIND_Left) : (bool)(binding & J2DBIND_Right);
        bindTop    = (mirror & J2DMIRROR_Y) ? (bool)(binding & J2DBIND_Bottom) : (bool)(binding & J2DBIND_Top);
        bindBottom = (mirror & J2DMIRROR_Y) ? (bool)(binding & J2DBIND_Top) : (bool)(binding & J2DBIND_Bottom);
    } else {
        bindLeft   = (mirror & J2DMIRROR_X) ? (bool)(binding & J2DBIND_Bottom) : (bool)(binding & J2DBIND_Top);
        bindRight  = (mirror & J2DMIRROR_X) ? (bool)(binding & J2DBIND_Top) : (bool)(binding & J2DBIND_Bottom);
        bindTop    = (mirror & J2DMIRROR_Y) ? (bool)(binding & J2DBIND_Left) : (bool)(binding & J2DBIND_Right);
        bindBottom = (mirror & J2DMIRROR_Y) ? (bool)(binding & J2DBIND_Right) : (bool)(binding & J2DBIND_Left);
    }

    f32 rectWidth;
    f32 rectHeight;

    rectWidth  = !doRotate90 ? getWidth() : getHeight();
    rectHeight = !doRotate90 ? getHeight() : getWidth();

    f32 texWidth;
    f32 texHeight;
    if (!texture) {
        texWidth  = rectWidth;
        texHeight = rectHeight;
    } else {
        texWidth  = texture->getWidth();
        texHeight = texture->getHeight();
    }

    f32 s0, t0, s1, t1;
    if (bindLeft) {
        s0 = 0.0f;
        s1 = bindRight ? 1.0f : (rectWidth / texWidth);
    } else if (bindRight) {
        s0 = 1.0f - (rectWidth / texWidth);
        s1 = 1.0f;
    } else {
        s0 = 0.5f - (rectWidth / texWidth) / 2.0f;
        s1 = 0.5f + (rectWidth / texWidth) / 2.0f;
    }

    if (bindTop) {
        t0 = 0.0f;
        t1 = bindBottom ? 1.0f : (rectHeight / texHeight);
    } else if (bindBottom) {
        t0 = 1.0f - (rectHeight / texHeight);
        t1 = 1.0f;
    } else {
        t0 = 0.5f - (rectHeight / texHeight) / 2.0f;
        t1 = 0.5f + (rectHeight / texHeight) / 2.0f;
    }

    if (mirror & J2DMIRROR_X) {
        swap(s0, s1);
    }
    if (mirror & J2DMIRROR_Y) {
        swap(t0, t1);
    }

    s16 temp_r27 = J2DCast_F32_to_S16(s0, 8);
    s16 temp_r28 = J2DCast_F32_to_S16(s1, 8);
    s16 temp_r30 = J2DCast_F32_to_S16(t0, 8);
    s16 temp_r31 = J2DCast_F32_to_S16(t1, 8);

    if (!doRotate90) {
        texCoords[0].set(temp_r27, temp_r30);
        texCoords[1].set(temp_r28, temp_r30);
        texCoords[2].set(temp_r27, temp_r31);
        texCoords[3].set(temp_r28, temp_r31);
    } else {
        texCoords[0].set(temp_r27, temp_r31);
        texCoords[1].set(temp_r27, temp_r30);
        texCoords[2].set(temp_r28, temp_r31);
        texCoords[3].set(temp_r28, temp_r30);
    }
}

bool J2DPicture::isUsed(const ResTIMG* resource)
{
    for (u8 i = 0; i < mTextureCount; i++) {
        if (mTextures[i] != nullptr && mTextures[i]->mTexInfo == resource) {
            return true;
        }
    }
    return J2DPane::isUsed(resource);
}

u8 J2DPicture::getUsableTlut(u8 id)
{
    u8 usedTexFlags = 0;

    for (u8 i = 0; i < mTextureCount; i++) {
        if (i != id && mTextures[i] != nullptr) {
            const ResTIMG* img = mTextures[i]->getTexInfo();

            if (img != nullptr && img->mPaletteFormat != 0) {
                int name   = mTextures[i]->getTlutName();
                int tlutID = name >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0;

                u8 diffID = name - tlutID;
                if (diffID < 4) {
                    usedTexFlags |= (1 << diffID);
                }
            }
        }
    }

    u8 usableID = 0;
    for (u8 i = 0; i < 4; i++) {
        if (!(usedTexFlags & (1 << i))) {
            usableID = i;
            break;
        }
    }

    return usableID;
}

GXTlut J2DPicture::getTlutID(const ResTIMG* img, u8 id)
{
    if (img->mPaletteEntryCount > 256) {
        return GXTlut((id % 4) + GX_BIGTLUT0);
    }
    return GXTlut(id);
}
