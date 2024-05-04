#include "Kaneshige/MaskScreen.h"
#include "Osako/system.h"

MaskScreenNode::MaskScreenNode()
{
    mTex = nullptr;
    mEndX = 30;
    mEndY = 32;
    mTex = new JUTTexture(mEndX, mEndY, GX_TF_RGB565);
    ResTIMG *img = mTex->getTexInfo();
    img->mTransparency = 1;
    reset();
}

void MaskScreenNode::reset()
{
    mStartX = 0;
    mStartY = 0;
    mFlush = false;
    mActive = false;
}

void MaskScreenNode::set(f32 x, f32 y, bool p3)
{
    _0 = p3;
    if (!p3)
        mStartX = (int)x;
    else
        mStartX = (int)x - mEndX + 1;

    mStartY = (int)y;
    mStartX = (mStartX + 1) & ~1;
    mStartY = (mStartY + 1) & ~1;
    this->mFlush = false;
    this->mActive = true;
}

void MaskScreenNode::capture()
{
    if (!mActive)
        return;

    mTex->capture(mStartX, mStartY, mTex->getFormat(), false, 0);
    mFlush = true;
}

void MaskScreenNode::draw()
{
    if (mActive && mFlush)
    {
        mTex->load(GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        GXPosition3s16(mStartX, mStartY, 0);
        if (!_0)
            GXColor4u8(0xff, 0xff, 0xff, 0xff);
        else
            GXColor4u8(0, 0xff, 0xff, 0);

        GXTexCoord2u8(0, 0);

        GXPosition3s16(mStartX + mEndX, mStartY, 0);

        if (!_0)
            GXColor4u8(0, 0xff, 0xff, 0);
        else
            GXColor4u8(0xff, 0xff, 0xff, 0xff);

        GXTexCoord2u8(1, 0);

        GXPosition3s16(mStartX + mEndX, mStartY + mEndY, 0);

        if (!_0)
            GXColor4u8(0, 0xff, 0xff, 0);
        else
            GXColor4u8(0xff, 0xff, 0xff, 0xff);

        GXTexCoord2u8(1, 1);
        GXPosition3s16(mStartX, mStartY + mEndY, 0);

        if (!_0)
            GXColor4u8(0xff, 0xff, 0xff, 0xff);
        else
            GXColor4u8(0, 0xff, 0xff, 0);

        GXTexCoord2u8(0, 1);
        GXEnd();
    }    
}

MaskScreen::MaskScreen()
{
    mStartX = 0;
    mStartY = 0;
    mEndX = System::getDisplay()->getEfbWidth();
    mEndY = System::getDisplay()->getEfbHeight();

    for (int i = 0; i < NUM_NODES; i++)
        mNodes[i] = new MaskScreenNode();

    reset();
}

void MaskScreen::reset()
{
    for (int i = 0; i < NUM_NODES; i++)
        mNodes[i]->reset();
}

void MaskScreen::resetAllMask() // clap for copy paste
{
    for (int i = 0; i < NUM_NODES; i++)
        mNodes[i]->reset();
}

void MaskScreen::requestMask(f32 x, f32 y, bool p3)
{
    MaskScreenNode *req = nullptr;
    for (int i = 0; i < NUM_NODES; i++)
    {
        if (!mNodes[i]->isActive())
        {
            req = mNodes[i];
            break;
        }
    }

#line 302
    JUT_ASSERT_MSG(req, "NOT ENOUGH NODE")
    if (req)
        req->set(x, y, p3);
}

void MaskScreen::capture()
{
    for (int i = 0; i < NUM_NODES; i++)
        mNodes[i]->capture();
}

void MaskScreen::draw()
{
    J2DOrthoGraph ortho(mStartX, mStartY, mEndX, mEndY, -1.0f, 1.0f);
    ortho.setPort();

    Mtx pos;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetNumTexGens(1);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_8_8);
    JUtility::TColor tevColor(0xff, 0xff, 0xff, 0);
    GXSetTevColor(GX_TEVREG0, tevColor); // inconsistency is key
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    PSMTXIdentity(pos);
    GXLoadPosMtxImm(pos, 0);
    GXSetCurrentMtx(0);

    for (int i = 0; i < NUM_NODES; i++)
        mNodes[i]->draw();

    
}