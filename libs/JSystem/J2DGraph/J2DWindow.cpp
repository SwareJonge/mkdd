#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "JSystem/J2D/J2DMaterial.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/JUtility/JUTResource.h"
#include "types.h"

J2DWindow::J2DWindow()
    : J2DPane()
    , mFrameTextureA(nullptr)
    , mFrameTextureB(nullptr)
    , mFrameTextureC(nullptr)
    , mFrameTextureD(nullptr)
    , mContentsTexture(nullptr)
    , mPalette(nullptr)
{
    mWrapFlags = 0;
    setContentsColor(TCOLOR_WHITE_U32);
    mBlack = TCOLOR_BLACK_U32;
    mWhite = TCOLOR_WHITE_U32;
}

J2DWindow::J2DWindow(J2DPane* parent, JSURandomInputStream* input, JKRArchive* archive)
    : J2DPane()
    , mFrameTextureA(nullptr)
    , mFrameTextureB(nullptr)
    , mFrameTextureC(nullptr)
    , mFrameTextureD(nullptr)
    , mContentsTexture(nullptr)
    , mPalette(nullptr)
{
    private_readStream(parent, input, archive);
}

J2DWindow::J2DWindow(J2DPane* parent, JSURandomInputStream* input, J2DMaterial* materialList)
    : mFrameTextureA(nullptr)
    , mFrameTextureB(nullptr)
    , mFrameTextureC(nullptr)
    , mFrameTextureD(nullptr)
    , mContentsTexture(nullptr)
    , mPalette(nullptr)
{
    u32 streamData[2];
    u32 startPos = input->getPosition();
    input->read(streamData, 8);

    mBloBlockType = 'WIN1';

    u32 newPos = input->getPosition();
    int newData[2];
    input->peek(newData, 8);
    makePaneExStream(parent, input);
    input->seek(newPos + newData[1], SEEK_SET);

    J2DWindowData data;
    input->read(&data, sizeof(J2DWindowData));

    JUTColor* colors[4] = {
        &mContentsColorA,
        &mContentsColorB,
        &mContentsColorC,
        &mContentsColorD,
    };

    u16 matIDs[4];
    J2DMaterial* mats[4];
    for (int i = 0; i < 4; i++) {
        matIDs[i] = data.mContentIds[i];
        mats[i]   = nullptr;
        if (matIDs[i] != 0xFFFF) {
            mats[i] = &materialList[matIDs[i]];
        }
        *colors[i] = JUTColor(data.mContentColors[i]);
    }

    mWrapFlags = data._18;
    mWindowArea.set(data.mMinX, data.mMinY, data.mMinX + data.mOffsetX, data.mMinY + data.mOffsetY);

    u16 parentID           = data.mParentId;
    J2DMaterial* parentMat = nullptr;
    if (parentID != 0xFFFF) {
        parentMat = &materialList[parentID];
    }

    input->seek(startPos + streamData[1], SEEK_SET);
    if (mats[0]) {
        mAlpha = mats[0]->getColorBlock()->getMatColor(0)->a;
    }

    mBlack = JUTColor(0);
    mWhite = JUTColor(0xFFFFFFFF);

    if (mats[0] && mats[0]->getTevBlock()) {
        if (mats[0]->getTevBlock()->getTevStageNum() != 1) {
            J2DGXColorS10* black = mats[0]->getTevBlock()->getTevColor(0);
            s16 blackr           = black->r;
            s16 blackg           = black->g;
            s16 blackb           = black->b;
            s16 blacka           = black->a;
            J2DGXColorS10* white = mats[0]->getTevBlock()->getTevColor(1);
            s16 whiter           = white->r;
            s16 whiteg           = white->g;
            s16 whiteb           = white->b;
            s16 whitea           = white->a;
            mBlack               = JUTColor((((u8)blackr) << 24) | (((u8)blackg) << 16) | (((u8)blackb) << 8) | ((u8)blacka));
            mWhite               = JUTColor((((u8)whiter) << 24) | (((u8)whiteg) << 16) | (((u8)whiteb) << 8) | ((u8)whitea));
        }
    }

    JUTTexture** textures[4] = {
        &mFrameTextureA,
        &mFrameTextureB,
        &mFrameTextureC,
        &mFrameTextureD,
    };

    J2DMaterial* mat;
    for (u32 i = 0; i < 4; i++) {
        if (mats[i] && mats[i]->getTevBlock()) {
            JUTTexture* tempTex = mats[i]->getTevBlock()->getTexture(0);
            if (tempTex) {
                *textures[i] = tempTex;
                mats[i]->getTevBlock()->setUndeleteFlag(~0x1);
            }
        }
    }

    mContentsTexture = nullptr;
    if (parentMat && parentMat->getTevBlock()) {
        JUTTexture* tempTex = parentMat->getTevBlock()->getTexture(0);
        if (tempTex) {
            mContentsTexture = tempTex;
            parentMat->getTevBlock()->setUndeleteFlag(0xfe);
        }
    }
    initinfo2();
}

void J2DWindow::private_readStream(J2DPane* parent, JSURandomInputStream* input, JKRArchive* arc)
{
    int startPos = input->getPosition();
    u32 startData[2];
    input->read(startData, 8);
    mBloBlockType = startData[0];
    makePaneStream(parent, input);
    JUTResReference resRef;
    u8 counter = input->readU8();
    f32 x0     = input->read16b();
    f32 y0     = input->read16b();
    f32 x1     = x0 + input->read16b();
    f32 y1     = y0 + input->read16b();
    mWindowArea.set(x0, y0, x1, y1);
    ResTIMG* timg = (ResTIMG*)getPointer(input, 'TIMG', arc);
    if (timg) {
        mFrameTextureA = new JUTTexture(timg, 0);
    }
    timg = (ResTIMG*)getPointer(input, 'TIMG', arc);
    if (timg) {
        mFrameTextureB = new JUTTexture(timg, 0);
    }
    timg = (ResTIMG*)getPointer(input, 'TIMG', arc);
    if (timg) {
        mFrameTextureC = new JUTTexture(timg, 0);
    }
    timg = (ResTIMG*)getPointer(input, 'TIMG', arc);
    if (timg) {
        mFrameTextureD = new JUTTexture(timg, 0);
    }
    ResTLUT* tlut = (ResTLUT*)getPointer(input, 'TLUT', arc);
    if (tlut) {
        mPalette = new JUTPalette(GX_TLUT0, tlut);
    }
    mWrapFlags = input->read8b();
    mContentsColorA.set(input->read32b());
    mContentsColorB.set(input->read32b());
    mContentsColorC.set(input->read32b());
    mContentsColorD.set(input->read32b());
    counter -= 14;
    mContentsTexture = nullptr;
    if (counter != 0) {
        timg = (ResTIMG*)getPointer(input, 'TIMG', arc);
        if (timg) {
            mContentsTexture = new JUTTexture(timg, 0);
        }
        counter--;
    }
    mBlack = JUTColor(0);
    mWhite = JUTColor(0xFFFFFFFF);
    if (counter != 0) {
        mBlack = JUTColor(input->readU32());
        counter--;
    }
    if (counter != 0) {
        mWhite = JUTColor(input->readU32());
    }
    input->seek(startPos + startData[1], SEEK_SET);
    initinfo2();
}

void J2DWindow::initinfo2()
{
    if (mFrameTextureA && mFrameTextureB && mFrameTextureC && mFrameTextureD) {
        mMinWidth  = mFrameTextureA->getWidth() + mFrameTextureB->getWidth();
        mMinHeight = mFrameTextureA->getHeight() + mFrameTextureC->getHeight();

    } else {
        mMinWidth  = 1;
        mMinHeight = 1;
        return;
    }

    _145            = 0;
    JUTTexture* tex = mFrameTextureA;
    if (*mFrameTextureB != *tex) {
        _145 |= 1;
        tex = mFrameTextureB;
    }
    if (*mFrameTextureD != *tex) {
        _145 |= 2;
        tex = mFrameTextureD;
    }
    if (*mFrameTextureC != *tex) {
        _145 |= 4;
    }
}

J2DWindow::~J2DWindow()
{
    delete mFrameTextureA;
    delete mFrameTextureB;
    delete mFrameTextureC;
    delete mFrameTextureD;
    delete mPalette;
    delete mContentsTexture;
}

void J2DWindow::draw(const JGeometry::TBox2f& inBox)
{
    JGeometry::TBox2f newBox;
    if (mFrameTextureA && mFrameTextureB && mFrameTextureC && mFrameTextureD) {
        newBox.set(mFrameTextureA->getWidth(), mFrameTextureA->getHeight(), inBox.getWidth() - mFrameTextureB->getWidth(),
                   inBox.getHeight() - mFrameTextureC->getHeight());
    } else {
        newBox.set(0.0f, 0.0f, inBox.getWidth(), inBox.getHeight());
    }
    draw(inBox, newBox);
}

void J2DWindow::draw_private(const JGeometry::TBox2f& box0, const JGeometry::TBox2f& box1)
{
    JGeometry::TBox2f newBox(box1);
    newBox.addPos(box0.i);
    drawContents(newBox);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetNumTexGens(1);

    if (mFrameTextureA && mFrameTextureB && mFrameTextureC && mFrameTextureD) {
        f32 startX = box0.i.x;
        f32 startY = box0.i.y;

        f32 x1 = box0.f.x - mFrameTextureD->getWidth();
        f32 y1 = box0.f.y - mFrameTextureD->getHeight();

        f32 x2 = startX + mFrameTextureA->getWidth();
        f32 y2 = startY + mFrameTextureA->getHeight();

        drawFrameTexture(mFrameTextureA, startX, startY, mWrapFlags & 0x80, mWrapFlags & 0x40, true);

        drawFrameTexture(mFrameTextureB, x1, startY, mWrapFlags & 0x20, mWrapFlags & 0x10, _145 & 1);

        u16 u0;
        if (mWrapFlags & 0x20) {
            u0 = 0x8000;
        } else {
            u0 = 0;
        }
        u16 v0;
        if (mWrapFlags & 0x10) {
            v0 = 0;
        } else {
            v0 = 0x8000;
        }
        u16 v1 = v0 ^ 0x8000;

        drawFrameTexture(mFrameTextureB, x2, startY, x1 - x2, mFrameTextureB->getHeight(), u0, v0, u0, v1, false);

        drawFrameTexture(mFrameTextureD, x1, y1, mWrapFlags & 2, mWrapFlags & 1, _145 & 2);

        u16 u1;
        if (mWrapFlags & 2) {
            u1 = 0x8000;
        } else {
            u1 = 0;
        }
        u16 v2;
        if (mWrapFlags & 1) {
            v2 = 0;
        } else {
            v2 = 0x8000;
        }
        v1 = v2 ^ 0x8000;
        drawFrameTexture(mFrameTextureD, x2, y1, x1 - x2, mFrameTextureD->getHeight(), u1, v2, u1, v1, false);

        u16 u2;
        if (mWrapFlags & 2) {
            u2 = 0;
        } else {
            u2 = 0x8000;
        }
        u16 u3 = u2 ^ 0x8000;
        u16 v3;
        if (mWrapFlags & 1) {
            v3 = 0x8000;
        } else {
            v3 = 0;
        }
        drawFrameTexture(mFrameTextureD, x1, y2, mFrameTextureD->getWidth(), y1 - y2, u2, v3, u3, v3, false);

        drawFrameTexture(mFrameTextureC, startX, y1, mWrapFlags & 8, mWrapFlags & 4, _145 & 4);

        u16 u4;
        if (mWrapFlags & 8) {
            u4 = 0;
        } else {
            u4 = 0x8000;
        }
        u16 u5 = u4 ^ 0x8000;
        u16 v4;
        if (mWrapFlags & 4) {
            v4 = 0x8000;
        } else {
            v4 = 0;
        }
        drawFrameTexture(mFrameTextureC, startX, y2, mFrameTextureC->getWidth(), y1 - y2, u4, v4, u5, v4, false);
    }
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetNumTexGens(0);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DWindow::draw(const JGeometry::TBox2f& box0, const JGeometry::TBox2f& box1)
{
    if (box0.getWidth() >= mMinWidth && box0.getHeight() >= mMinHeight && isVisible()) {
        makeMatrix(box0.i.x, box0.i.y, 0.0f, 0.0f);
        GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
        GXSetCurrentMtx(0);
        mColorAlpha = mAlpha;
        JGeometry::TBox2f newBox(0.0f, 0.0f, box0.getWidth(), box0.getHeight());
        draw_private(newBox, box1);
        Mtx mtx;
        PSMTXIdentity(mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
    }
}

void J2DWindow::resize(f32 maxX, f32 maxY)
{
    f32 width  = getWidth();
    f32 height = getHeight();
    J2DPane::resize(maxX, maxY);
    mWindowArea.f.x += maxX - width;
    mWindowArea.f.y += maxY - height;
    for (JSUTreeIterator<J2DPane> it(getFirstChild()); it != getEndChild(); it++) {
        if (it->getTypeID() == J2DPaneType_TextBox && it->isConnectParent()) {
            f32 newX = maxX - width + it->getWidth();
            f32 newY = maxY - height + it->getHeight();
            it->J2DPane::resize(newX, newY);
        }
    }
}

void J2DWindow::setContentsColor(JUTColor colorA, JUTColor colorB, JUTColor colorC, JUTColor colorD)
{
    mContentsColorA.set(colorA);
    mContentsColorB.set(colorB);
    mContentsColorC.set(colorC);
    mContentsColorD.set(colorD);
}

void J2DWindow::drawSelf(f32 offsetX, f32 offsetY)
{
    Mtx mtx;
    PSMTXIdentity(mtx);
    drawSelf(offsetX, offsetY, &mtx);
}

void J2DWindow::drawSelf(f32 offsetX, f32 offsetY, Mtx* mtx)
{
    JGeometry::TBox2f box(mBounds);
    box.addPos(JGeometry::TVec2<f32>(offsetX, offsetY));
    if (box.getWidth() >= mMinWidth && box.getHeight() >= mMinHeight) {
        Mtx posMtx;
        PSMTXConcat(*mtx, mGlobalMtx, posMtx);
        GXLoadPosMtxImm(posMtx, 0);
        draw_private(box, mWindowArea);
    }
    clip(mWindowArea);
}

void J2DWindow::drawContents(const JGeometry::TBox2f& box)
{
    if (box.isValid()) {
        GXSetNumChans(1);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        if ((mContentsColorA & 0xFF) == 0xFF && (mContentsColorB & 0xFF) == 0xFF && (mContentsColorC & 0xFF) == 0xFF
            && (mContentsColorD & 0xFF) == 0xFF && mColorAlpha == 0xFF) {
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
        } else {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        }

        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumIndStages(0);
        for (int i = 0; i < 16; i++) {
            GXSetTevDirect(GXTevStageID(i));
        }

        if (!mContentsTexture) {
            JUTColor color1(mContentsColorA);
            JUTColor color2(mContentsColorC);
            JUTColor color3(mContentsColorB);
            JUTColor color4(mContentsColorD);
            if (mColorAlpha != 0xFF) {
                color1.a = color1.a * mColorAlpha / 0xFF;
                color2.a = color2.a * mColorAlpha / 0xFF;
                color3.a = color3.a * mColorAlpha / 0xFF;
                color4.a = color4.a * mColorAlpha / 0xFF;
            }
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            f32 zero = 0.0f;
            GXPosition3f32(box.i.x, box.i.y, zero);
            GXColor1u32(color1);
            GXPosition3f32(box.f.x, box.i.y, zero);
            GXColor1u32(color3);
            GXPosition3f32(box.f.x, box.f.y, zero);
            GXColor1u32(color4);
            GXPosition3f32(box.i.x, box.f.y, zero);
            GXColor1u32(color2);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        } else {
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetNumTexGens(1);
            drawContentsTexture(box.i.x, box.i.y, box.getWidth(), box.getHeight());
        }
    }
}

void J2DWindow::drawFrameTexture(JUTTexture* texture, f32 x0, f32 y0, f32 width, f32 height, u16 maxU, u16 maxV, u16 minU, u16 minV, bool doLoad)
{
    f32 x1 = x0 + width;
    f32 y1 = y0 + height;
    if (doLoad) {
        texture->load(GX_TEXMAP0);
        setTevMode(texture, mBlack, mWhite);
    }

    JUTColor color(mColorAlpha | 0xFFFFFF00);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);


    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, y0, 0.0f);
    GXColor1u32(color);
    GXTexCoord2u16(minU, minV);

    GXPosition3f32(x1, y0, 0.0f);
    GXColor1u32(color);
    GXTexCoord2u16(maxU, minV);

    GXPosition3f32(x1, y1, 0.0f);
    GXColor1u32(color);
    GXTexCoord2u16(maxU, maxV);

    GXPosition3f32(x0, y1, 0.0f);
    GXColor1u32(color);
    GXTexCoord2u16(minU, maxV);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

// Incorrect Scheduling
void J2DWindow::drawFrameTexture(JUTTexture* texture, f32 x0, f32 y0, bool doInvertU, bool doInvertV, bool doLoad)
{
    drawFrameTexture(texture, x0, y0, texture->getWidth(), texture->getHeight(), doInvertU ? 0 : 0x8000, doInvertV ? 0 : 0x8000,
                     doInvertU ? 0x8000 : 0, doInvertV ? 0x8000 : 0, doLoad);
}

void J2DWindow::drawContentsTexture(f32 x0, f32 y0, f32 xOffset, f32 yOffset)
{
    f32 x1     = x0 + xOffset;
    f32 y1     = y0 + yOffset;
    f32 width  = mContentsTexture->getWidth();
    f32 height = mContentsTexture->getHeight();
    f32 xTex0  = -(xOffset / width - 1.0f) / 2.0f;
    f32 yTex0  = -(yOffset / height - 1.0f) / 2.0f;
    f32 xTex1  = xTex0 + xOffset / width;
    f32 yTex1  = yTex0 + yOffset / height;
    TContentsColor contentsColor;
    getContentsColor(contentsColor);
    if (mColorAlpha != 0xff) {
        contentsColor.mColorA.a = contentsColor.mColorA.a * mColorAlpha / 0xff;
        contentsColor.mColorB.a = contentsColor.mColorB.a * mColorAlpha / 0xff;
        contentsColor.mColorC.a = contentsColor.mColorC.a * mColorAlpha / 0xff;
        contentsColor.mColorD.a = contentsColor.mColorD.a * mColorAlpha / 0xff;
    }
    mContentsTexture->load(GX_TEXMAP0);
    setTevMode(mContentsTexture, 0, 0xffffffff);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

    f32 z = 0.0f;

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, y0, z);
    GXColor1u32(contentsColor.mColorA);
    GXTexCoord2f32(xTex0, yTex0);

    GXPosition3f32(x1, y0, z);
    GXColor1u32(contentsColor.mColorB);
    GXTexCoord2f32(xTex1, yTex0);

    GXPosition3f32(x1, y1, z);
    GXColor1u32(contentsColor.mColorD);
    GXTexCoord2f32(xTex1, yTex1);

    GXPosition3f32(x0, y1, z);
    GXColor1u32(contentsColor.mColorC);
    GXTexCoord2f32(xTex0, yTex1);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 15);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

void J2DWindow::setTevMode(JUTTexture* texture, JUTColor color1, JUTColor color2)
{
    if (color1 == 0 && color2 == 0xFFFFFFFF) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        if (texture->getTransparency()) {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        } else {
            GXSetTevColor(GX_TEVREG2, JUTColor(0xFFFFFFFF));
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A2, GX_CA_RASA, GX_CA_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetNumTevStages(1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    } else {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, color1);
        GXSetTevColor(GX_TEVREG1, color2);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
        if (texture->getTransparency()) {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
        } else {
            GXSetTevColor(GX_TEVREG2, JUTColor(0xFFFFFFFF));
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_A2, GX_CA_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetNumTevStages(2);
    }
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

JUTTexture* J2DWindow::getFrameTexture(u8 frameMaterialIndex, u8 textureIndex) const
{
    JUTTexture* textures[4] = { mFrameTextureA, mFrameTextureB, mFrameTextureC, mFrameTextureD };
    if (frameMaterialIndex >= 4 || textureIndex != 0) {
        return nullptr;
    }
    return textures[frameMaterialIndex];
}

bool J2DWindow::isUsed(const ResTIMG* resource)
{
    JUTTexture* textures[5] = { mFrameTextureA, mFrameTextureB, mFrameTextureC, mFrameTextureD, mContentsTexture };
    for (u8 i = 0; i < 5; i++) {
        if (textures[i] != nullptr && textures[i]->mTexInfo == resource) {
            return true;
        }
    }
    return J2DPane::isUsed(resource);
}
