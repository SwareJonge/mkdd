#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/gx.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "JSystem/J2D/J2DManage.h"
#include "JSystem/J2D/J2DMaterial.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J3D/J3DFileBlock.h"
#include "JSystem/J3D/J3DTypes.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRFileLoader.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUStream.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DMaterialfactory.h"
#include "JSystem/J3D/J3DTypes.h"
#include "types.h"

J2DDataManage* J2DScreen::mDataManage;

J2DScreen::J2DScreen()
    : J2DPane(nullptr, true, 'root', JGeometry::TBox2f(JGeometry::TVec2f(0, 0), JGeometry::TVec2f(640, 480)))
{
    // Non matching, has to do with TBox
    mAnimPaneIndex = 0xFFFF;
    mIsScissor     = false;
    mMaterialCount = 0;
    mMaterials     = nullptr;
    mTexRes        = nullptr;
    mFontRes       = nullptr;
    mNameTab       = nullptr;
}
J2DScreen::~J2DScreen() { clean(); }

void J2DScreen::clean()
{
    delete[] mMaterials;
    mMaterialCount = 0;
    mMaterials     = nullptr;
    delete[] mTexRes;
    mTexRes = nullptr;
    delete[] mFontRes;
    mFontRes = nullptr;
    if (mNameTab) {
        mNameTab->clearResNameTable();
        delete mNameTab;
        mNameTab = nullptr;
    }
}

bool J2DScreen::set(const char* name, u32 flags, JKRArchive* archive)
{
    void* resource = JKRFileLoader::getGlbResource(name, archive);
    if (resource) {
        size_t resSize = archive->getExpandedResSize(resource);
        JSUMemoryInputStream input(resource, resSize);
        return set(&input, flags);
    }
    return false;
}

bool J2DScreen::set(JSURandomInputStream* stream, u32 flags) { private_set(stream, flags, nullptr); }

bool J2DScreen::setPriority(const char *path, u32 flags, JKRArchive *archive) {
    if (archive == nullptr) {
        return false;
    }

    const void *res = JKRGetNameResource(path, archive);
    if (res) {
        JSUMemoryInputStream stream(res, archive->getExpandedResSize(res));
        return setPriority(&stream, flags, archive);
    }
    return false;
}

bool J2DScreen::setPriority(JSURandomInputStream* stream, u32 flags, JKRArchive* archive) {
    if (archive == nullptr) {
        return false;
    }
    return private_set(stream, flags, archive);
}

bool J2DScreen::private_set(JSURandomInputStream* stream, u32 flags, JKRArchive* archive)
{
    if (!checkSignature(stream)) {
        return false;
    }
    if (!getScreenInformation(stream)) {
        return false;
    }
    bool result = makeHierarchyPanes(this, stream, flags, archive) != 2;
    if ((flags & 0x1F0000) == 0) {
        clean();
    }
    return (result != false) ? stream->mState == 0 : false;
}

bool J2DScreen::checkSignature(JSURandomInputStream* stream)
{
    J3DFileHeader header;
    stream->read(&header, sizeof(header));
    if (header.mJ3dVersion != 'SCRN' || !(header.mFileVersion == 'blo1' || header.mFileVersion == 'blo2')) {
#line 257
        JUT_WARNING_F2("%s", "SCRN resource is broken.\n");
        return false;
    }    
    return true;
}

bool J2DScreen::getScreenInformation(JSURandomInputStream* input)
{
    J2DScreenInfoBlock info;
    input->read(&info, sizeof(info));
    if (info.mBloBlockType != 'INF1') {
#line 282
        JUT_WARNING_F2("%s", "SCRN resource is broken.\n");
        return false;
    }
    JGeometry::TBox2f box;
    box.i.x = 0.0f;
    box.i.y = 0.0f;
    box.f.x = info.mWidth;
    box.f.y = info.mHeight;
    place(box);
    mColor = info.mColor;
    if (sizeof(info) < info.mBlockLength) {
        input->skip(info.mBlockLength - sizeof(info));
    }
    return true;
}

inline u32 J2DScreen::makeHierarchyPanes(J2DPane* parent, JSURandomInputStream* input, u32 flags, JKRArchive* archive)
{
    // only matches when marked as inline, probably inline size limit
    J2DPane* currentPane = parent;
    while (true) {
        J2DScrnBlockHeader header;
        input->peek(&header, 8);
        switch (header.mBloBlockType) {
        case 'EXT1':
            input->seek(header.mBlockLength, SEEK_CUR);
            return 1;

        case 'BGN1': {
            input->seek(header.mBlockLength, SEEK_CUR);
            s32 result = makeHierarchyPanes(currentPane, input, flags, archive);
            if (result) {
                return result;
            }
            break;
        }
        case 'END1':
            input->seek(header.mBlockLength, SEEK_CUR);
            return 0;

        case 'TEX1': {
            J2DResReference* ref = getResReference(input, flags);
            mTexRes = ref;
            if (!ref) {
                return 2;
            }
            break;
        }
        case 'FNT1': {
            J2DResReference* ref = getResReference(input, flags);
            mFontRes = ref;
            if (!ref) {
                return 2;
            }
            break;
        }
        case 'MAT1':
            if (!createMaterial(input, flags, archive)) {
                return 2;
            }
            break;
        default:

            if (archive == nullptr) {
                currentPane = createPane(header, input, parent, flags);
            } else {
                currentPane = createPane(header, input, parent, flags, archive);
            }
            if (!currentPane) {
                return 2;
            }
            break;
        }
    }
}

J2DPane* J2DScreen::createPane(const J2DScrnBlockHeader& header, JSURandomInputStream* input, J2DPane* parent, u32 flags,
                               JKRArchive* archive)
{
    // can't just do direct returns here...
    J2DPane* pane;
    switch (header.mBloBlockType) {
    case 'PAN1':
        pane = new J2DPane(parent, input, 0);
        break;
    case 'WIN1':
        pane = new J2DWindow(parent, input, archive);
        break;
    case 'PIC1':
        pane = new J2DPicture(parent, input, archive);
        break;
    case 'TBX1':
        pane = new J2DTextBox(parent, input, archive);
        break;
    case 'PAN2':
        pane = new J2DPane(parent, input, 1);
        break;
    case 'WIN2':
        if ((flags & 0x1F0000) != 0) {
            pane = new J2DWindowEx(parent, input, flags, mMaterials);
            break;
        }
        pane = new J2DWindow(parent, input, mMaterials);
        break;
    case 'PIC2':
        if ((flags & 0x1F0000) != 0) {
            pane = new J2DPictureEx(parent, input, flags, mMaterials);
            break;
        }
        pane = new J2DPicture(parent, input, mMaterials);
        break;
    case 'TBX2':
        if ((flags & 0x1F0000) != 0) {
            pane = new J2DTextBoxEx(parent, input, flags, mMaterials);
            break;
        }
        pane = new J2DTextBox(parent, input, flags, mMaterials);
        break;
    default: {
#line 446
        JUT_WARNING_F2("%s", "unknown pane");
        int targetPosition = input->getPosition() + header.mBlockLength;
        pane               = new J2DPane(parent, input, 0);
        input->seek(targetPosition, SEEK_SET);
        break;
    }
    }
    return pane;
}

void J2DScreen::draw(f32 x, f32 y, const J2DGrafContext* graf)
{
    u32 x1, y1, wd, ht;
    if (mIsScissor) {
        GXGetScissor(&x1, &y1, &wd, &ht);
    }

    if (graf) {
        J2DPane::draw(x, y, graf, mIsScissor, true);
    } else {
        J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
        ortho.setPort();
        J2DPane::draw(x, y, &ortho, mIsScissor, true);
    }

    if (mIsScissor) {
        GXSetScissor(x1, y1, wd, ht);
    }

    GXSetNumIndStages(0);
    for (int i = 0; i < 16; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumTexGens(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
}

J2DPane* J2DScreen::search(u64 id) { return (id == 0) ? nullptr : J2DPane::search(id); }

u32 J2DScreen::gather(J2DPane** p1, u64 p2, u64 p3, int p4)
{
    int v1 = 0;
    J2DPane::gather(p1, p2, p3, p4, v1);
    return v1;
}

J2DPane* J2DScreen::searchUserInfo(u64 p1) { return (p1 == 0) ? nullptr : J2DPane::searchUserInfo(p1); }

void J2DScreen::drawSelf(f32 x, f32 y, Mtx* mtx)
{
    JUTColor color(mColor);
    u8 alpha = color.a;
    alpha = ((alpha * mAlpha) / 255);
    if (!(u8)alpha) { 
        return;
    }
    color = (color & 0xFFFFFF00) | alpha;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(0.0f, 0.0f, 0.0);
    GXColor1u32(color);
    GXPosition3f32(mBounds.getWidth(), 0.0f, 0.0f);
    GXColor1u32(color);
    GXPosition3f32(mBounds.getWidth(), mBounds.getHeight(), 0.0f);
    GXColor1u32(color);
    GXPosition3f32(0.0f, mBounds.getHeight(), 0.0f);
    GXColor1u32(color);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

J2DResReference* J2DScreen::getResReference(JSURandomInputStream* input, u32 flags)
{
    s32 initialPosition = input->getPosition();
    input->skip(4);
    s32 size1, size2;
    size1 = input->readS32();
    input->skip(4);
    size2 = input->readS32();
    input->seek(initialPosition + size2, SEEK_SET);
    size1 = size1 - size2;
    char* data;
    if (flags & (J3DMLF_TevNumShift | J3DMLF_18 | J3DMLF_UseSingleSharedDL | J3DMLF_20 | J3DMLF_21)) {
        data = new char[size1];
    } else {
        data = new (-4) char[size1];
    }
    if (data) {
        input->read(data, size1);
    }
    return (J2DResReference*)data;
}

bool J2DScreen::createMaterial(JSURandomInputStream* input, u32 flags, JKRArchive* archive)
{
    int initialPosition = input->getPosition();
    u32 blank[2];
    input->read(blank, 8);
    mMaterialCount = input->readU16();
    input->skip(2);

    if (flags & 0x1f0000) {
        mMaterials = new J2DMaterial[mMaterialCount];
    } else {
        mMaterials = new (-4) J2DMaterial[mMaterialCount];
    }

    u8* blockbuf = new (-4) u8[blank[1]];
    if (mMaterials && blockbuf) {
        J2DMaterialBlock* blocks = (J2DMaterialBlock*)blockbuf;
        input->seek(initialPosition, SEEK_SET);
        input->read(blockbuf, blank[1]);

        J2DMaterialFactory factory(blocks[0]);
        for (u16 i = 0; i < mMaterialCount; i++) {
            factory.create(&mMaterials[i], i, flags, mTexRes, mFontRes, archive);
        }
        if (flags & 0x1f0000) {
            u32 nameTabIdx = READU32_BE(blockbuf, 0x14);
            ResNTAB* nameTab = (ResNTAB*)(blockbuf + nameTabIdx);
            u16 entry = nameTab->mEntryNum;
            u16 offset = nameTab->mEntries[entry-1].mOffs;
            const char *strTable = (const char *)nameTab;
            u16 strSize = offset;
            for (; strTable[strSize] != 0; strSize++) {}
            
            strSize++;
            u8* tab = new u8[strSize];
            if (!tab)
                goto ret_fail;
            
            for (u16 i = 0; i < strSize; i++) {
                tab[i] = blockbuf[nameTabIdx+i];
            }
            
            mNameTab = new JUTNameTab((ResNTAB*)tab);
            if (!mNameTab) {
                delete[] tab;
                goto ret_fail; 
            }
        }

        delete[] blockbuf;
        return true;
    }

ret_fail: 
    delete[] blockbuf;
    clean();
    return false;
}

J2DMaterial* J2DScreen::getMaterial(u16 index)
{
    if (index >= mMaterialCount) {
        return nullptr;
    }
    if (mMaterials == nullptr) {
        return nullptr;
    }
    return mMaterials + index;
}

bool J2DScreen::isUsed(const ResTIMG* resource) { return J2DPane::isUsed(resource); }

bool J2DScreen::isUsed(const ResFONT* resource) { return J2DPane::isUsed(resource); }

void* J2DScreen::getNameResource(const char* fileName)
{
    void* resource = JKRFileLoader::getGlbResource(fileName, nullptr);
    if (resource == nullptr && mDataManage != nullptr) {
        resource = mDataManage->get(fileName);
    }
    return resource;
}

void J2DScreen::animation()
{
    animationPane(mTransform);
    for (u16 i = 0; i < mMaterialCount; i++) {
        mMaterials[i].animation();
    }
}

void J2DScreen::setAnimation(J2DAnmColor* animation)
{
    animation->searchUpdateMaterialID(this);
    u16 count = animation->getUpdateMaterialNum();
    for (u16 i = 0; i < count; i++) {
        if (animation->getUpdateMaterialID(i) < mMaterialCount) {
            mMaterials[animation->getUpdateMaterialID(i)].setAnimation(animation);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmTextureSRTKey* animation)
{
    animation->searchUpdateMaterialID(this);
    u16 count = animation->getUpdateMaterialNum();
    for (u16 i = 0; i < count; i++) {
        if (animation->getUpdateMaterialID(i) < mMaterialCount) {
            mMaterials[animation->getUpdateMaterialID(i)].setAnimation(animation);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmTexPattern* animation)
{
    animation->searchUpdateMaterialID(this);
    u16 count = animation->getUpdateMaterialNum();
    for (u16 i = 0; i < count; i++) {
        if (animation->getUpdateMaterialID(i) < mMaterialCount) {
            mMaterials[animation->getUpdateMaterialID(i)].setAnimation(animation);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmTevRegKey* animation)
{
    animation->searchUpdateMaterialID(this);
    u16 count = animation->getCRegUpdateMaterialNum();
    for (u16 i = 0; i < count; i++) {
        if (animation->getCRegUpdateMaterialID(i) < mMaterialCount) {
            mMaterials[animation->getCRegUpdateMaterialID(i)].setAnimation(animation);
        }
    }
    count = animation->getKRegUpdateMaterialNum();
    for (u16 i = 0; i < count; i++) {
        if (animation->getKRegUpdateMaterialID(i) < mMaterialCount) {
            mMaterials[animation->getKRegUpdateMaterialID(i)].setAnimation(animation);
        }
    }
}


void J2DScreen::setAnimation(J2DAnmVtxColor* animation) {
    J2DPane::setVtxColorAnimation(animation);
}

void J2DScreen::setAnimation(J2DAnmVisibilityFull* animation) {
    J2DPane::setVisibileAnimation(animation);
}
