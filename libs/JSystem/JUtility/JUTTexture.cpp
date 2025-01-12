#include <dolphin/gx.h>
#include <dolphin/os.h>
#include "types.h"
#include "JSystem/ResTIMG.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTTexture.h"

JUTTexture::JUTTexture(int sizeX, int sizeY, GXTexFmt textureFormat)
{
    mFlags = (mFlags & TEXFLAG_Unk2) | TEXFLAG_Unk1;
    u32 texBufferSize = GXGetTexBufferSize(sizeX, sizeY, textureFormat, GX_FALSE, 1);
    mImage = reinterpret_cast<ResTIMG *>(new (0x20) u8[texBufferSize + 0x20]);
    ResTIMG *img = mImage;
    img->mTextureFormat = textureFormat;
    img->mTransparency = UNK0;
    img->mSizeX = sizeX;
    img->mSizeY = sizeY;
    img->mWrapS = GX_CLAMP;
    img->mWrapT = GX_CLAMP;
    img->mPaletteFormat = 0;
    img->mLutFormat = GX_TL_IA8;
    img->mPaletteEntryCount = 0;
    img->mPaletteOffset = 0;
    img->mIsMIPmapEnabled = GX_FALSE;
    img->mDoEdgeLOD = GX_FALSE;
    img->mIsBiasClamp = GX_FALSE;
    img->mIsMaxAnisotropy = GX_FALSE;
    img->mMinFilterType = 1;
    img->mMagFilterType = 1;
    img->mMinLOD = 0;
    img->mMaxLOD = 0;
    img->mTotalImageCount = 1;
    img->mLODBias = 0;
    img->mImageDataOffset = 0x20;
    mEmbPalette = nullptr;
    storeTIMG(img);
    DCFlushRange(mTexData, texBufferSize);
}

JUTTexture::~JUTTexture()
{
    if (mFlags & TEXFLAG_Unk1)
    {
        delete[] mImage;
    }
    if (mFlags & TEXFLAG_Unk2)
    {
        delete mEmbPalette;
    }
}

void JUTTexture::storeTIMG(const ResTIMG *img, u8 lutID)
{
    if (img && lutID < 16)
    {
        mTexInfo = const_cast<ResTIMG *>(img);
        mTexData = (void *)((u32)mTexInfo + mTexInfo->mImageDataOffset);
        if (mTexInfo->mImageDataOffset == 0)
        {
            mTexData = (void *)((u32)mTexInfo + 0x20);
        }

        mActivePalette = nullptr;
        mTlut = GX_TLUT0;
        mWrapS = mTexInfo->mWrapS;
        mWrapT = mTexInfo->mWrapT;
        mMinFilter = mTexInfo->mMinFilterType;
        mMagFilter = mTexInfo->mMagFilterType;
        mMinLOD = mTexInfo->mMinLOD;
        mMaxLOD = mTexInfo->mMaxLOD;
        mLODBias = mTexInfo->mLODBias;

        u16 paletteCount = mTexInfo->mPaletteEntryCount;
        if (paletteCount == 0)
        {
            initTexObj();
            return;
        }

        GXTlut tlut;
        if (paletteCount > 0x100)
        {
            tlut = (GXTlut)(lutID % 4 + 0x10);
        }
        else
        {
            tlut = (GXTlut)(lutID);
        }

        if (!mEmbPalette || !(mFlags & TEXFLAG_Unk2))
        {
            mEmbPalette = new JUTPalette(tlut, (GXTlutFmt)mTexInfo->mLutFormat, (JUTTransparency)mTexInfo->mTransparency,
                                         mTexInfo->mPaletteEntryCount, (void *)((u32)mTexInfo + mTexInfo->mPaletteOffset));
            mFlags = (mFlags & TEXFLAG_Unk1) | TEXFLAG_Unk2;
        }
        else
        {
            mEmbPalette->storeTLUT(tlut, (GXTlutFmt)mTexInfo->mLutFormat, (JUTTransparency)mTexInfo->mTransparency,
                                   mTexInfo->mPaletteEntryCount, (void *)((u32)mTexInfo + mTexInfo->mPaletteOffset));
        }

        attachPalette(mEmbPalette);
    }
}

void JUTTexture::storeTIMG(const ResTIMG *img, JUTPalette *palette)
{
    storeTIMG(img, palette, palette ? palette->getTlutName() : GX_TLUT0);
}

void JUTTexture::storeTIMG(const ResTIMG *img, JUTPalette *palette, GXTlut tlut)
{
    if (img == nullptr)
        return;
    
    mTexInfo = const_cast<ResTIMG *>(img);
    mTexData = ((u8 *)mTexInfo + mTexInfo->mImageDataOffset);
    if (mTexInfo->mImageDataOffset == 0)
    {
        mTexData = ((u8 *)mTexInfo + 0x20);
    }

    if (getEmbPaletteDelFlag())
    {
        delete mEmbPalette;
    }
    mEmbPalette = palette;
    setEmbPaletteDelFlag(false);
    mActivePalette = nullptr;

    if (palette)
    {
        mTlut = (u8)tlut;
        if (tlut != palette->getTlutName())
        {
            GXTlutFmt fmt = palette->getFormat();
            JUTTransparency trans = palette->getTransparency();
            u16 colors = palette->getNumColors();
            ResTLUT *table = palette->getColorTable();
            palette->storeTLUT(tlut, fmt, trans, colors, table);
        }
    }

    mWrapS = mTexInfo->mWrapS;
    mWrapT = mTexInfo->mWrapT;
    mMinFilter = mTexInfo->mMinFilterType;
    mMagFilter = mTexInfo->mMagFilterType;
    mMinLOD = mTexInfo->mMinLOD;
    mMaxLOD = mTexInfo->mMaxLOD;
    mLODBias = mTexInfo->mLODBias;
    init();
    
}

void JUTTexture::attachPalette(JUTPalette *palette)
{
    if (mTexInfo->mPaletteFormat == 0)
    {
        return;
    }
    if (!palette && mEmbPalette)
    {
        mActivePalette = mEmbPalette;
    }
    else
    {
        mActivePalette = palette;
    }
    initTexObj(mActivePalette->getTlutName());
}

void JUTTexture::init()
{
    if (mTexInfo->mPaletteEntryCount == 0)
    {
        initTexObj();
    }
    else if (mEmbPalette)
    {
        mActivePalette = mEmbPalette;
        initTexObj(mActivePalette->getTlutName());
    }
#ifdef DEBUG
    else
    {
        OSReport("This texture is CI-Format, but EmbPalette is NULL.\n");
    }
#endif
}

// DUMB: u16 casts are needed for TP debug
void JUTTexture::initTexObj() 
{
    GXBool hasMipMap = mTexInfo->mIsMIPmapEnabled ? GX_TRUE : GX_FALSE;
    u8 *image = (u8 *)mTexInfo;

    image += ((mTexInfo->mImageDataOffset) ? mTexInfo->mImageDataOffset : 0x20);

    GXInitTexObj(this, image, /*(u16) */ mTexInfo->mSizeX,
                 /*(u16) */ mTexInfo->mSizeY, (GXTexFmt)mTexInfo->mTextureFormat, (GXTexWrapMode)mWrapS, (GXTexWrapMode)mWrapT,
                 hasMipMap);

    GXInitTexObjLOD(this, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter, (f32)mMinLOD / 8, (f32)mMaxLOD / 8, mLODBias / 100.0f,
                    /*(u16) */ mTexInfo->mIsBiasClamp, /*(u16) */ mTexInfo->mDoEdgeLOD, (GXAnisotropy)mTexInfo->mIsMaxAnisotropy);
}

void JUTTexture::initTexObj(GXTlut tlut)
{
    GXBool hasMipMap = mTexInfo->mIsMIPmapEnabled ? GX_TRUE : GX_FALSE;
    mTlut = tlut;
    u8 *image = (u8 *)mTexInfo;

    image += ((mTexInfo->mImageDataOffset) ? mTexInfo->mImageDataOffset : 0x20);
    
    GXInitTexObjCI(this, image, /*(u16) */ mTexInfo->mSizeX,
                   /*(u16) */ mTexInfo->mSizeY, (GXCITexFmt)mTexInfo->mTextureFormat, (GXTexWrapMode)mWrapS, (GXTexWrapMode)mWrapT,
                   hasMipMap, tlut);

    GXInitTexObjLOD(this, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter, (f32)mMinLOD / 8, (f32)mMaxLOD / 8, mLODBias / 100.0f,
                    /*(u16) */ mTexInfo->mIsBiasClamp, /*(u16) */ mTexInfo->mDoEdgeLOD, (GXAnisotropy)mTexInfo->mIsMaxAnisotropy);
}

void JUTTexture::load(GXTexMapID id)
{
    if (mActivePalette)
    {
        mActivePalette->load();
    }
    GXLoadTexObj(this, id);
}

void JUTTexture::capture(int x0, int y0, GXTexFmt textureFormat, bool useMIPmap, u8 doClear)
{
    if (!(mFlags & 1))
    {
        return;
    }

    if (useMIPmap)
    {
        GXSetTexCopySrc(x0, y0, mTexInfo->mSizeX << 1, mTexInfo->mSizeY << 1);
    }
    else
    {
        GXSetTexCopySrc(x0, y0, mTexInfo->mSizeX, mTexInfo->mSizeY);
    }

    GXSetTexCopyDst(mTexInfo->mSizeX, mTexInfo->mSizeY, textureFormat, useMIPmap);
    GXCopyTex(mTexData, doClear);
    GXPixModeSync();
}
