#ifndef GX_H
#define GX_H

#include <dolphin/mtx.h>
#include <dolphin/types.h>

// TODO: REFACTOR
// will i ever do this?

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

    typedef u8 GXBool;

#define GX_TRUE ((GXBool)1)
#define GX_FALSE ((GXBool)0)
#define GX_ENABLE ((GXBool)1)
#define GX_DISABLE ((GXBool)0)

// GX Types
// Generic struct for texture look-up table objects.
typedef struct _GXTlutObj
{
    u8 padding[0xC]; // _00
} GXTlutObj;

// GXEnum
typedef enum _GXPrimitive {
    /* 0x80 */ GX_QUADS = 0x80,
    /* 0x90 */ GX_TRIANGLES = 0x90,
    /* 0x98 */ GX_TRIANGLESTRIP = 0x98,
    /* 0xA0 */ GX_TRIANGLEFAN = 0xA0,
    /* 0xA8 */ GX_LINES = 0xA8,
    /* 0xB0 */ GX_LINESTRIP = 0xB0,
    /* 0xB8 */ GX_POINTS = 0xB8,
} GXPrimitive;

typedef enum _GXCullMode {
    /* 0x0 */ GX_CULL_NONE,
    /* 0x1 */ GX_CULL_FRONT,
    /* 0x2 */ GX_CULL_BACK,
    /* 0x3 */ GX_CULL_ALL
} GXCullMode;

typedef enum _GXTexMapID {
    /* 0x000 */ GX_TEXMAP0,
    /* 0x001 */ GX_TEXMAP1,
    /* 0x002 */ GX_TEXMAP2,
    /* 0x003 */ GX_TEXMAP3,
    /* 0x004 */ GX_TEXMAP4,
    /* 0x005 */ GX_TEXMAP5,
    /* 0x006 */ GX_TEXMAP6,
    /* 0x007 */ GX_TEXMAP7,
    /* 0x008 */ GX_MAX_TEXMAP,
    /* 0x0FF */ GX_TEXMAP_NULL = 255,
    /* 0x100 */ GX_TEXMAP_DISABLE,
} GXTexMapID;

typedef enum _GXTevStageID {
    /* 0x00 */ GX_TEVSTAGE0,
    /* 0x01 */ GX_TEVSTAGE1,
    /* 0x02 */ GX_TEVSTAGE2,
    /* 0x03 */ GX_TEVSTAGE3,
    /* 0x04 */ GX_TEVSTAGE4,
    /* 0x05 */ GX_TEVSTAGE5,
    /* 0x06 */ GX_TEVSTAGE6,
    /* 0x07 */ GX_TEVSTAGE7,
    /* 0x08 */ GX_TEVSTAGE8,
    /* 0x09 */ GX_TEVSTAGE9,
    /* 0x0A */ GX_TEVSTAGE10,
    /* 0x0B */ GX_TEVSTAGE11,
    /* 0x0C */ GX_TEVSTAGE12,
    /* 0x0D */ GX_TEVSTAGE13,
    /* 0x0E */ GX_TEVSTAGE14,
    /* 0x0F */ GX_TEVSTAGE15,
    /* 0x10 */ GX_MAX_TEVSTAGE,
} GXTevStageID;

typedef enum _GXTexCoordID {
    /* 0x00 */ GX_TEXCOORD0,
    /* 0x01 */ GX_TEXCOORD1,
    /* 0x02 */ GX_TEXCOORD2,
    /* 0x03 */ GX_TEXCOORD3,
    /* 0x04 */ GX_TEXCOORD4,
    /* 0x05 */ GX_TEXCOORD5,
    /* 0x06 */ GX_TEXCOORD6,
    /* 0x07 */ GX_TEXCOORD7,
    /* 0x08 */ GX_MAXCOORD,
    /* 0xFF */ GX_TEXCOORD_NULL = 255,
} GXTexCoordID;

typedef enum _GXChannelID {
    /* 0x00 */ GX_COLOR0,
    /* 0x01 */ GX_COLOR1,
    /* 0x02 */ GX_ALPHA0,
    /* 0x03 */ GX_ALPHA1,
    /* 0x04 */ GX_COLOR0A0,
    /* 0x05 */ GX_COLOR1A1,
    /* 0x06 */ GX_COLOR_ZERO,
    /* 0x07 */ GX_ALPHA_BUMP,
    /* 0x08 */ GX_ALPHA_BUMPN,
    /* 0xFF */ GX_COLOR_NULL = 255,
} GXChannelID;

typedef enum _GXColorSrc {
    /* 0x0 */ GX_SRC_REG,
    /* 0x1 */ GX_SRC_VTX,
} GXColorSrc;

typedef enum _GXLightID {
    /* 0x000 */ GX_LIGHT_NULL = 0,
    /* 0x001 */ GX_LIGHT0 = 1 << 0,
    /* 0x002 */ GX_LIGHT1 = 1 << 1,
    /* 0x004 */ GX_LIGHT2 = 1 << 2,
    /* 0x008 */ GX_LIGHT3 = 1 << 3,
    /* 0x010 */ GX_LIGHT4 = 1 << 4,
    /* 0x020 */ GX_LIGHT5 = 1 << 5,
    /* 0x040 */ GX_LIGHT6 = 1 << 6,
    /* 0x080 */ GX_LIGHT7 = 1 << 7,
    /* 0x100 */ GX_MAX_LIGHT = 1 << 8,
} GXLightID;

typedef enum _GXDiffuseFn {
    /* 0x0 */ GX_DF_NONE,
    /* 0x1 */ GX_DF_SIGN,
    /* 0x2 */ GX_DF_CLAMP,
} GXDiffuseFn;

typedef enum _GXAttnFn {
    /* 0x0 */ GX_AF_SPEC,
    /* 0x1 */ GX_AF_SPOT,
    /* 0x2 */ GX_AF_NONE,
} GXAttnFn;

typedef enum _GXDistAttnFn {
    /* 0x0 */ GX_DA_OFF,
    /* 0x1 */ GX_DA_GENTLE,
    /* 0x2 */ GX_DA_MEDIUM,
    /* 0x3 */ GX_DA_STEEP
} GXDistAttnFn;

typedef enum _GXSpotFn {
    /* 0x0 */ GX_SP_OFF,
    /* 0x1 */ GX_SP_FLAT,
    /* 0x2 */ GX_SP_COS,
    /* 0x3 */ GX_SP_COS2,
    /* 0x4 */ GX_SP_SHARP,
    /* 0x5 */ GX_SP_RING1,
    /* 0x6 */ GX_SP_RING2
} GXSpotFn;

typedef enum _GXTevMode {
    /* 0x0 */ GX_MODULATE,
    /* 0x1 */ GX_DECAL,
    /* 0x2 */ GX_BLEND,
    /* 0x3 */ GX_REPLACE,
    /* 0x4 */ GX_PASSCLR,
} GXTevMode;

typedef enum _GXBlendMode {
    /* 0x0 */ GX_BM_NONE,
    /* 0x1 */ GX_BM_BLEND,
    /* 0x2 */ GX_BM_LOGIC,
    /* 0x3 */ GX_BM_SUBTRACT,
    /* 0x4 */ GX_MAX_BLENDMODE,
} GXBlendMode;

typedef enum _GXBlendFactor {
    /* 0x0 */ GX_BL_ZERO,
    /* 0x1 */ GX_BL_ONE,
    /* 0x2 */ GX_BL_SRC_COLOR,
    /* 0x2 */ GX_BL_DST_COLOR = 2,
    /* 0x3 */ GX_BL_INV_SRC_COLOR,
    /* 0x3 */ GX_BL_INV_DST_COLOR = 3,
    /* 0x4 */ GX_BL_SRC_ALPHA,
    /* 0x5 */ GX_BL_INV_SRC_ALPHA,
    /* 0x6 */ GX_BL_DST_ALPHA,
    /* 0x7 */ GX_BL_INV_DST_ALPHA,
} GXBlendFactor;

typedef enum _GXLogicOp {
    /* 0x0 */ GX_LO_CLEAR,
    /* 0x1 */ GX_LO_AND,
    /* 0x2 */ GX_LO_REV_AND,
    /* 0x3 */ GX_LO_COPY,
    /* 0x4 */ GX_LO_INV_AND,
    /* 0x5 */ GX_LO_NOOP,
    /* 0x6 */ GX_LO_XOR,
    /* 0x7 */ GX_LO_OR,
    /* 0x8 */ GX_LO_NOR,
    /* 0x9 */ GX_LO_EQUIV,
    /* 0xA */ GX_LO_INV,
    /* 0xB */ GX_LO_REV_OR,
    /* 0xC */ GX_LO_INV_COPY,
    /* 0xD */ GX_LO_INV_OR,
    /* 0xE */ GX_LO_NAND,
    /* 0xF */ GX_LO_SET,
} GXLogicOp;

typedef enum _GXVtxFmt {
    /* 0x0 */ GX_VTXFMT0,
    /* 0x1 */ GX_VTXFMT1,
    /* 0x2 */ GX_VTXFMT2,
    /* 0x3 */ GX_VTXFMT3,
    /* 0x4 */ GX_VTXFMT4,
    /* 0x5 */ GX_VTXFMT5,
    /* 0x6 */ GX_VTXFMT6,
    /* 0x7 */ GX_VTXFMT7,
    /* 0x8 */ GX_MAX_VTXFMT,
} GXVtxFmt;

typedef enum _GXAttr {
    /* 0x00 */ GX_VA_PNMTXIDX,
    /* 0x01 */ GX_VA_TEX0MTXIDX,
    /* 0x02 */ GX_VA_TEX1MTXIDX,
    /* 0x03 */ GX_VA_TEX2MTXIDX,
    /* 0x04 */ GX_VA_TEX3MTXIDX,
    /* 0x05 */ GX_VA_TEX4MTXIDX,
    /* 0x06 */ GX_VA_TEX5MTXIDX,
    /* 0x07 */ GX_VA_TEX6MTXIDX,
    /* 0x08 */ GX_VA_TEX7MTXIDX,
    /* 0x09 */ GX_VA_POS,
    /* 0x0A */ GX_VA_NRM,
    /* 0x0B */ GX_VA_CLR0,
    /* 0x0C */ GX_VA_CLR1,
    /* 0x0D */ GX_VA_TEX0,
    /* 0x0E */ GX_VA_TEX1,
    /* 0x0F */ GX_VA_TEX2,
    /* 0x10 */ GX_VA_TEX3,
    /* 0x11 */ GX_VA_TEX4,
    /* 0x12 */ GX_VA_TEX5,
    /* 0x13 */ GX_VA_TEX6,
    /* 0x14 */ GX_VA_TEX7,
    /* 0x15 */ GX_POS_MTX_ARRAY,
    /* 0x16 */ GX_NRM_MTX_ARRAY,
    /* 0x17 */ GX_TEX_MTX_ARRAY,
    /* 0x18 */ GX_LIGHT_ARRAY,
    /* 0x19 */ GX_VA_NBT,
    /* 0x1A */ GX_VA_MAX_ATTR,
    /* 0xFF */ GX_VA_NULL = 255,
} GXAttr;

typedef enum _GXCompCnt {
    /* 0x0 */ GX_POS_XY = 0,
    /* 0x1 */ GX_POS_XYZ = 1,
    /* 0x0 */ GX_NRM_XYZ = 0,
    /* 0x1 */ GX_NRM_NBT = 1,
    /* 0x2 */ GX_NRM_NBT3 = 2,
    /* 0x0 */ GX_CLR_RGB = 0,
    /* 0x1 */ GX_CLR_RGBA = 1,
    /* 0x0 */ GX_TEX_S = 0,
    /* 0x1 */ GX_TEX_ST = 1,
} GXCompCnt;

typedef enum _GXCompType {
    /* 0x0 */ GX_U8,
    /* 0x1 */ GX_S8,
    /* 0x2 */ GX_U16,
    /* 0x3 */ GX_S16,
    /* 0x4 */ GX_F32,
    /* 0x0 */ GX_RGB565 = 0,
    /* 0x1 */ GX_RGB8,
    /* 0x2 */ GX_RGBX8,
    /* 0x3 */ GX_RGBA4,
    /* 0x4 */ GX_RGBA6,
    /* 0x5 */ GX_RGBA8,
} GXCompType;

typedef enum _GXAttrType {
    /* 0x0 */ GX_NONE,
    /* 0x1 */ GX_DIRECT,
    /* 0x2 */ GX_INDEX8,
    /* 0x3 */ GX_INDEX16,
} GXAttrType;

typedef enum _GXTevOp {
    /* 0x0 */ GX_TEV_ADD,
    /* 0x1 */ GX_TEV_SUB,
    /* 0x8 */ GX_TEV_COMP_R8_GT = 8,
    /* 0x9 */ GX_TEV_COMP_R8_EQ,
    /* 0xA */ GX_TEV_COMP_GR16_GT,
    /* 0xB */ GX_TEV_COMP_GR16_EQ,
    /* 0xC */ GX_TEV_COMP_BGR24_GT,
    /* 0xD */ GX_TEV_COMP_BGR24_EQ,
    /* 0xE */ GX_TEV_COMP_A8_GT,
    /* 0xE */ GX_TEV_COMP_RGB8_GT = 14,
    /* 0xF */ GX_TEV_COMP_RGB8_EQ,
    /* 0xF */ GX_TEV_COMP_A8_EQ = 15,
} GXTevOp;

typedef enum _GXTevBias {
    /* 0x0 */ GX_TB_ZERO,
    /* 0x1 */ GX_TB_ADDHALF,
    /* 0x2 */ GX_TB_SUBHALF,
    /* 0x3 */ GX_MAX_TEVBIAS,
} GXTevBias;

typedef enum _GXTevColorArg {
    /* 0x0 */ GX_CC_CPREV,
    /* 0x1 */ GX_CC_APREV,
    /* 0x2 */ GX_CC_C0,
    /* 0x3 */ GX_CC_A0,
    /* 0x4 */ GX_CC_C1,
    /* 0x5 */ GX_CC_A1,
    /* 0x6 */ GX_CC_C2,
    /* 0x7 */ GX_CC_A2,
    /* 0x8 */ GX_CC_TEXC,
    /* 0x9 */ GX_CC_TEXA,
    /* 0xA */ GX_CC_RASC,
    /* 0xB */ GX_CC_RASA,
    /* 0xC */ GX_CC_ONE,
    /* 0xD */ GX_CC_HALF,
    /* 0xE */ GX_CC_KONST,
    /* 0xF */ GX_CC_ZERO,
} GXTevColorArg;

typedef enum _GXTevColor {
    /* 0x0 */ GX_CH_RED,
    /* 0x1 */ GX_CH_GREEN,
    /* 0x2 */ GX_CH_BLUE,
    /* 0x3 */ GX_CH_ALPHA,
} GXTevColor;

typedef enum _GXTevScale {
    /* 0x0 */ GX_CS_SCALE_1,
    /* 0x1 */ GX_CS_SCALE_2,
    /* 0x2 */ GX_CS_SCALE_4,
    /* 0x3 */ GX_CS_DIVIDE_2,
    /* 0x4 */ GX_MAX_TEVSCALE
} GXTevScale;

typedef enum _GXTevRegID {
    /* 0x0 */ GX_TEVPREV,
    /* 0x1 */ GX_TEVREG0,
    /* 0x2 */ GX_TEVREG1,
    /* 0x3 */ GX_TEVREG2,
    /* 0x4 */ GX_MAX_TEVREG
} GXTevRegID;

typedef enum _GXTevAlphaArg {
    /* 0x0 */ GX_CA_APREV,
    /* 0x1 */ GX_CA_A0,
    /* 0x2 */ GX_CA_A1,
    /* 0x3 */ GX_CA_A2,
    /* 0x4 */ GX_CA_TEXA,
    /* 0x5 */ GX_CA_RASA,
    /* 0x6 */ GX_CA_KONST,
    /* 0x7 */ GX_CA_ZERO,
} GXTevAlphaArg;

typedef enum _GXTexFmt {
    /* 0x0 */ GX_TF_I4,
    /* 0x1 */ GX_TF_I8,
    /* 0x2 */ GX_TF_IA4,
    /* 0x3 */ GX_TF_IA8,
    /* 0x4 */ GX_TF_RGB565,
    /* 0x5 */ GX_TF_TGB5A3,
    /* 0x6 */ GX_TF_RGBA8,
    /* 0x7 */ GX_TF_CI4,
    /* 0x8 */ GX_TF_CI8,
    /* 0x9 */ GX_TF_CI14,
    /* 0xE */ GX_TF_CMPR = 14,
    /* 0x10 */ _GX_TF_ZTF = 0x10,
    /* 0x11 */ GX_TF_Z8 = (0x1 | _GX_TF_ZTF),
    /* 0x13 */ GX_TF_Z16 = (0x3 | _GX_TF_ZTF),
    /* 0x16 */ GX_TF_Z24X8 = (0x6 | _GX_TF_ZTF),
    /* 0x20 */ _GX_TF_CTF = 0x20,
    /* 0x28 */ _GX_CTF_R8 = 0x28,
    /* 0x30 */ GX_CTF_Z4 = (0x0 | _GX_TF_ZTF | _GX_TF_CTF),
    /* 0x39 */ GX_CTF_Z8M = (0x9 | _GX_TF_ZTF | _GX_TF_CTF),
    /* 0x3A */ GX_CTF_Z8L = (0xA | _GX_TF_ZTF | _GX_TF_CTF),
    /* 0x3C */ GX_CTF_Z16L = (0xC | _GX_TF_ZTF | _GX_TF_CTF),
} GXTexFmt;

typedef enum _GXGamma {
    /* 0x0 */ GX_GM_1_0,
    /* 0x0 */ GX_GM_1_7,
    /* 0x0 */ GX_GM_2_2,
} GXGamma;

typedef enum _GXTlutFmt {
    /* 0x0 */ GX_TL_IA8,
    /* 0x1 */ GX_TL_RGB565,
    /* 0x2 */ GX_TL_RGB5A3,
} GXTlutFmt;

typedef enum _GXTlut {
    /* 0x00 */ GX_TLUT0,
    /* 0x01 */ GX_TLUT1,
    /* 0x02 */ GX_TLUT2,
    /* 0x03 */ GX_TLUT3,
    /* 0x04 */ GX_TLUT4,
    /* 0x05 */ GX_TLUT5,
    /* 0x06 */ GX_TLUT6,
    /* 0x07 */ GX_TLUT7,
    /* 0x08 */ GX_TLUT8,
    /* 0x09 */ GX_TLUT9,
    /* 0x0A */ GX_TLUT10,
    /* 0x0B */ GX_TLUT11,
    /* 0x0C */ GX_TLUT12,
    /* 0x0D */ GX_TLUT13,
    /* 0x0E */ GX_TLUT14,
    /* 0x0F */ GX_TLUT15,
    /* 0x10 */ GX_BIGTLUT0,
    /* 0x11 */ GX_BIGTLUT1,
    /* 0x12 */ GX_BIGTLUT2,
    /* 0x13 */ GX_BIGTLUT3,
} GXTlut;

/* 8-bit version of GXTexFmt for use in structures */
typedef enum _GXTexFmt8 {
    /* 0x20 */ GX_CTF_R4 = (0x0 | _GX_TF_CTF),
    /* 0x22 */ GX_CTF_RA4 = (0x2 | _GX_TF_CTF),
    /* 0x23 */ GX_CTF_RA8 = (0x3 | _GX_TF_CTF),
    /* 0x26 */ GX_CTF_YUVA8 = (0x6 | _GX_TF_CTF),
    /* 0x27 */ GX_CTF_A8 = (0x7 | _GX_TF_CTF),
    /* 0x28 */ GX_CTF_R8 = (0x8 | _GX_TF_CTF),
    /* 0x29 */ GX_CTF_G8 = (0x9 | _GX_TF_CTF),
    /* 0x2A */ GX_CTF_B8 = (0xA | _GX_TF_CTF),
    /* 0x2B */ GX_CTF_RG8 = (0xB | _GX_TF_CTF),
    /* 0x2C */ GX_CTF_GB8 = (0xC | _GX_TF_CTF),
} GXTexFmt8;

typedef enum _GXTexWrapMode {
    /* 0x0 */ GX_CLAMP,
    /* 0x1 */ GX_REPEAT,
    /* 0x2 */ GX_MIRROR,
    /* 0x3 */ GX_MAX_TEXWRAP_MODE,
} GXTexWrapMode;

typedef enum _GXTexFilter {
    /* 0x0 */ GX_NEAR,
    /* 0x1 */ GX_LINEAR,
    /* 0x2 */ GX_NEAR_MIP_NEAR,
    /* 0x3 */ GX_LIN_MIP_NEAR,
    /* 0x4 */ GX_NEAR_MIP_LIN,
    /* 0x5 */ GX_LIN_MIP_LIN,
} GXTexFilter;

typedef enum _GXAnisotropy {
    /* 0x0 */ GX_ANISO_1,
    /* 0x1 */ GX_ANISO_2,
    /* 0x2 */ GX_ANISO_4,
    /* 0x3 */ GX_MAX_ANISOTROPY,
} GXAnisotropy;

typedef enum _GXCITexFmt {
    /* 0x8 */ GX_TF_C4 = 8,
    /* 0x8 */ GX_TF_C8,
    /* 0xA */ GX_TF_C14X2
} GXCITexFmt;

typedef enum _GXTexMtxType {
    /* 0x0 */ GX_MTX3x4,
    /* 0x1 */ GX_MTX2x4
} GXTexMtxType;

typedef enum _GXCompare {
    /* 0x0 */ GX_NEVER,
    /* 0x1 */ GX_LESS,
    /* 0x2 */ GX_EQUAL,
    /* 0x3 */ GX_LEQUAL,
    /* 0x4 */ GX_GREATER,
    /* 0x5 */ GX_NEQUAL,
    /* 0x6 */ GX_GEQUAL,
    /* 0x7 */ GX_ALWAYS,
} GXCompare;

typedef enum _GXAlphaOp {
    /* 0x0 */ GX_AOP_AND,
    /* 0x1 */ GX_AOP_OR,
    /* 0x2 */ GX_AOP_XOR,
    /* 0x3 */ GX_AOP_XNOR,
    /* 0x4 */ GX_MAX_ALPHAOP
} GXAlphaOp;

typedef enum _GXFogType {
    /* 0x0 */ GX_FOG_NONE,
    /* 0x2 */ GX_FOG_LIN = 2,
    /* 0x2 */ GX_FOG_PERSP_LIN = 2,
    /* 0x4 */ GX_FOG_EXP = 4,
    /* 0x4 */ GX_FOG_PERSP_EXP = 4,
    /* 0x5 */ GX_FOG_EXP2,
    /* 0x5 */ GX_FOG_PERSP_EXP2 = 5,
    /* 0x6 */ GX_FOG_REVEXP,
    /* 0x6 */ GX_FOG_PERSP_REVEXP = 6,
    /* 0x7 */ GX_FOG_REVEXP2,
    /* 0x7 */ GX_FOG_PERSP_REVEXP2 = 7,
    /* 0xA */ GX_FOG_ORTHO_LIN = 10,
    /* 0xC */ GX_FOG_ORTHO_EXP = 12,
    /* 0xD */ GX_FOG_ORTHO_EXP2,
    /* 0xE */ GX_FOG_ORTHO_REVEXP,
    /* 0xF */ GX_FOG_ORTHO_REVEXP2,
} GXFogType;

typedef enum _GXProjectionType {
    /* 0x0 */ GX_PERSPECTIVE,
    /* 0x1 */ GX_ORTHOGRAPHIC
} GXProjectionType;

typedef enum _GXTevKAlphaSel {
    /* 0x00 */ GX_TEV_KASEL_1,
    /* 0x01 */ GX_TEV_KASEL_7_8,
    /* 0x02 */ GX_TEV_KASEL_3_4,
    /* 0x03 */ GX_TEV_KASEL_5_8,
    /* 0x04 */ GX_TEV_KASEL_1_2,
    /* 0x05 */ GX_TEV_KASEL_3_8,
    /* 0x06 */ GX_TEV_KASEL_1_4,
    /* 0x07 */ GX_TEV_KASEL_1_8,

    /* 0x10 */ GX_TEV_KASEL_K0_R = 0x10,
    /* 0x11 */ GX_TEV_KASEL_K1_R,
    /* 0x12 */ GX_TEV_KASEL_K2_R,
    /* 0x13 */ GX_TEV_KASEL_K3_R,
    /* 0x14 */ GX_TEV_KASEL_K0_G,
    /* 0x15 */ GX_TEV_KASEL_K1_G,
    /* 0x16 */ GX_TEV_KASEL_K2_G,
    /* 0x17 */ GX_TEV_KASEL_K3_G,
    /* 0x18 */ GX_TEV_KASEL_K0_B,
    /* 0x19 */ GX_TEV_KASEL_K1_B,
    /* 0x1A */ GX_TEV_KASEL_K2_B,
    /* 0x1B */ GX_TEV_KASEL_K3_B,
    /* 0x1C */ GX_TEV_KASEL_K0_A,
    /* 0x1D */ GX_TEV_KASEL_K1_A,
    /* 0x1E */ GX_TEV_KASEL_K2_A,
    /* 0x1F */ GX_TEV_KASEL_K3_A,
} GXTevKAlphaSel;

typedef enum _GXTevKColorSel {
    /* 0x00 */ GX_TEV_KCSEL_1,
    /* 0x01 */ GX_TEV_KCSEL_7_8,
    /* 0x02 */ GX_TEV_KCSEL_3_4,
    /* 0x03 */ GX_TEV_KCSEL_5_8,
    /* 0x04 */ GX_TEV_KCSEL_1_2,
    /* 0x05 */ GX_TEV_KCSEL_3_8,
    /* 0x06 */ GX_TEV_KCSEL_1_4,
    /* 0x07 */ GX_TEV_KCSEL_1_8,
    /* 0x0C */ GX_TEV_KCSEL_K0 = 0xC,
    /* 0x0D */ GX_TEV_KCSEL_K1,
    /* 0x0E */ GX_TEV_KCSEL_K2,
    /* 0x0F */ GX_TEV_KCSEL_K3,
    /* 0x10 */ GX_TEV_KCSEL_K0_R,
    /* 0x11 */ GX_TEV_KCSEL_K1_R,
    /* 0x12 */ GX_TEV_KCSEL_K2_R,
    /* 0x13 */ GX_TEV_KCSEL_K3_R,
    /* 0x14 */ GX_TEV_KCSEL_K0_G,
    /* 0x15 */ GX_TEV_KCSEL_K1_G,
    /* 0x16 */ GX_TEV_KCSEL_K2_G,
    /* 0x17 */ GX_TEV_KCSEL_K3_G,
    /* 0x18 */ GX_TEV_KCSEL_K0_B,
    /* 0x19 */ GX_TEV_KCSEL_K1_B,
    /* 0x1A */ GX_TEV_KCSEL_K2_B,
    /* 0x1B */ GX_TEV_KCSEL_K3_B,
    /* 0x1C */ GX_TEV_KCSEL_K0_A,
    /* 0x1D */ GX_TEV_KCSEL_K1_A,
    /* 0x1E */ GX_TEV_KCSEL_K2_A,
    /* 0x1F */ GX_TEV_KCSEL_K3_A,
} GXTevKColorSel;

typedef enum _GXTevSwapSel {
    /* 0x0 */ GX_TEV_SWAP0,
    /* 0x1 */ GX_TEV_SWAP1,
    /* 0x2 */ GX_TEV_SWAP2,
    /* 0x3 */ GX_TEV_SWAP3,
    /* 0x4 */ GX_MAX_TEVSWAP,
} GXTevSwapSel;

typedef enum _GXTexGenType {
    /* 0x0 */ GX_TG_MTX3x4,
    /* 0x1 */ GX_TG_MTX2x4,
    /* 0x2 */ GX_TG_BUMP0,
    /* 0x3 */ GX_TG_BUMP1,
    /* 0x4 */ GX_TG_BUMP2,
    /* 0x5 */ GX_TG_BUMP3,
    /* 0x6 */ GX_TG_BUMP4,
    /* 0x7 */ GX_TG_BUMP5,
    /* 0x8 */ GX_TG_BUMP6,
    /* 0x9 */ GX_TG_BUMP7,
    /* 0xA */ GX_TG_SRTG,
} GXTexGenType;

typedef enum _GXTexGenSrc {
    /* 0x00 */ GX_TG_POS,
    /* 0x01 */ GX_TG_NRM,
    /* 0x02 */ GX_TG_BINRM,
    /* 0x03 */ GX_TG_TANGENT,
    /* 0x04 */ GX_TG_TEX0,
    /* 0x05 */ GX_TG_TEX1,
    /* 0x06 */ GX_TG_TEX2,
    /* 0x07 */ GX_TG_TEX3,
    /* 0x08 */ GX_TG_TEX4,
    /* 0x09 */ GX_TG_TEX5,
    /* 0x0A */ GX_TG_TEX6,
    /* 0x0B */ GX_TG_TEX7,
    /* 0x0C */ GX_TG_TEXCOORD0,
    /* 0x0D */ GX_TG_TEXCOORD1,
    /* 0x0E */ GX_TG_TEXCOORD2,
    /* 0x0F */ GX_TG_TEXCOORD3,
    /* 0x10 */ GX_TG_TEXCOORD4,
    /* 0x11 */ GX_TG_TEXCOORD5,
    /* 0x12 */ GX_TG_TEXCOORD6,
} GXTexGenSrc;

typedef enum _GXZTexOp {
    /* 0x0 */ GX_ZT_DISABLE,
    /* 0x1 */ GX_ZT_ADD,
    /* 0x2 */ GX_ZT_REPLACE,
    /* 0x3 */ GX_MAX_ZTEXOP,
} GXZTexOp;

typedef enum _GXIndTexStageID {
    /* 0x0 */ GX_INDTEXSTAGE0,
    /* 0x1 */ GX_INDTEXSTAGE1,
    /* 0x2 */ GX_INDTEXSTAGE2,
    /* 0x3 */ GX_INDTEXSTAGE3,
    /* 0x4 */ GX_MAX_INDTEXSTAGE,
} GXIndTexStageID;

typedef enum _GXIndTexScale {
    /* 0x0 */ GX_ITS_1,
    /* 0x1 */ GX_ITS_2,
    /* 0x2 */ GX_ITS_4,
    /* 0x3 */ GX_ITS_8,
    /* 0x4 */ GX_ITS_16,
    /* 0x5 */ GX_ITS_32,
    /* 0x6 */ GX_ITS_64,
    /* 0x7 */ GX_ITS_128,
    /* 0x8 */ GX_ITS_256,
    /* 0x9 */ GX_MAX_ITSCALE,
} GXIndTexScale;

typedef enum _GXIndTexMtxID {
    /* 0x0 */ GX_ITM_OFF,
    /* 0x1 */ GX_ITM_0,
    /* 0x2 */ GX_ITM_1,
    /* 0x3 */ GX_ITM_2,
    /* 0x4 */ GX_ITM_3,
    /* 0x5 */ GX_ITM_S0 = 5,
    /* 0x6 */ GX_ITM_S1,
    /* 0x7 */ GX_ITM_S2,
    /* 0x8 */ GX_ITM_S3,
    /* 0x9 */ GX_ITM_T0 = 9,
    /* 0xA */ GX_ITM_T1,
    /* 0xB */ GX_ITM_T2,
} GXIndTexMtxID;

typedef enum _GXIndTexFormat {
    /* 0x0 */ GX_ITF_8,
    /* 0x1 */ GX_ITF_5,
    /* 0x2 */ GX_ITF_4,
    /* 0x3 */ GX_ITF_3,
} GXIndTexFormat;

typedef enum _GXIndTexBiasSel {
    /* 0x0 */ GX_ITB_NONE,
    /* 0x1 */ GX_ITB_S,
    /* 0x2 */ GX_ITB_T,
    /* 0x3 */ GX_ITB_ST,
    /* 0x4 */ GX_ITB_U,
    /* 0x5 */ GX_ITB_SU,
    /* 0x6 */ GX_ITB_TU,
    /* 0x7 */ GX_ITB_STU,
} GXIndTexBiasSel;

typedef enum _GXIndTexAlphaSel {
    /* 0x0 */ GX_ITBA_OFF,
    /* 0x1 */ GX_ITBA_S,
    /* 0x2 */ GX_ITBA_T,
    /* 0x3 */ GX_ITBA_U,
} GXIndTexAlphaSel;

typedef enum _GXIndTexWrap {
    /* 0x0 */ GX_ITW_OFF,
    /* 0x1 */ GX_ITW_256,
    /* 0x2 */ GX_ITW_128,
    /* 0x3 */ GX_ITW_64,
    /* 0x4 */ GX_ITW_32,
    /* 0x5 */ GX_ITW_16,
    /* 0x6 */ GX_ITW_0,
} GXIndTexWrap;

typedef enum _GXTexOffset {
    /* 0x0 */ GX_TO_ZERO,
    /* 0x1 */ GX_TO_SIXTEENTH,
    /* 0x2 */ GX_TO_EIGHTH,
    /* 0x3 */ GX_TO_FOURTH,
    /* 0x4 */ GX_TO_HALF,
    /* 0x5 */ GX_TO_ONE,
    /* 0x6 */ GX_MAX_TEXOFFSET,
} GXTexOffset;

typedef enum _GXTevKColorID {
    /* 0x0 */ GX_KCOLOR0,
    /* 0x1 */ GX_KCOLOR1,
    /* 0x2 */ GX_KCOLOR2,
    /* 0x3 */ GX_KCOLOR3,
} GXTevKColorID;

typedef enum _GXTexCacheSize {
    /* 0x0 */ GX_TEXCACHE_32K,
    /* 0x1 */ GX_TEXCACHE_128K,
    /* 0x2 */ GX_TEXCACHE_512K,
    /* 0x3 */ GX_TEXCACHE_NONE,
} GXTexCacheSize;

typedef enum _GXPosNrmMtx {
    GX_PNMTX0 = 3 * 0,
    GX_PNMTX1 = 3 * 1,
    GX_PNMTX2 = 3 * 2,
    GX_PNMTX3 = 3 * 3,
    GX_PNMTX4 = 3 * 4,
    GX_PNMTX5 = 3 * 5,
    GX_PNMTX6 = 3 * 6,
    GX_PNMTX7 = 3 * 7,
    GX_PNMTX8 = 3 * 8,
    GX_PNMTX9 = 3 * 9,
} GXPosNrmMtx;

typedef enum _GXTexMtx {
    GX_TEXMTX0 = 30 + 0 * 3,
    GX_TEXMTX1 = 30 + 1 * 3,
    GX_TEXMTX2 = 30 + 2 * 3,
    GX_TEXMTX3 = 30 + 3 * 3,
    GX_TEXMTX4 = 30 + 4 * 3,
    GX_TEXMTX5 = 30 + 5 * 3,
    GX_TEXMTX6 = 30 + 6 * 3,
    GX_TEXMTX7 = 30 + 7 * 3,
    GX_TEXMTX8 = 30 + 8 * 3,
    GX_TEXMTX9 = 30 + 9 * 3,
    GX_IDENTITY = 60,
} GXTexMtx;

typedef enum _GXPTTexMtx {
    GX_PTTEXMTX0 = 64 + 0 * 3,
    GX_PTTEXMTX1 = 64 + 1 * 3,
    GX_PTTEXMTX2 = 64 + 2 * 3,
    GX_PTTEXMTX3 = 64 + 3 * 3,
    GX_PTTEXMTX4 = 64 + 4 * 3,
    GX_PTTEXMTX5 = 64 + 5 * 3,
    GX_PTTEXMTX6 = 64 + 6 * 3,
    GX_PTTEXMTX7 = 64 + 7 * 3,
    GX_PTTEXMTX8 = 64 + 8 * 3,
    GX_PTTEXMTX9 = 64 + 9 * 3,
    GX_PTTEXMTX10 = 64 + 10 * 3,
    GX_PTTEXMTX11 = 64 + 11 * 3,
    GX_PTTEXMTX12 = 64 + 12 * 3,
    GX_PTTEXMTX13 = 64 + 13 * 3,
    GX_PTTEXMTX14 = 64 + 14 * 3,
    GX_PTTEXMTX15 = 64 + 15 * 3,
    GX_PTTEXMTX16 = 64 + 16 * 3,
    GX_PTTEXMTX17 = 64 + 17 * 3,
    GX_PTTEXMTX18 = 64 + 18 * 3,
    GX_PTTEXMTX19 = 64 + 19 * 3,
    GX_PTIDENTITY = 125,
} GXPTTexMtx;

typedef enum _GXFBClamp {
    /* 0x0 */ GX_CLAMP_NONE,
    /* 0x1 */ GX_CLAMP_TOP,
    /* 0x2 */ GX_CLAMP_BOTTOM,
} GXFBClamp;

typedef enum _GXPixelFmt {
    /* 0x0 */ GX_PF_RGB8_Z24,
    /* 0x1 */ GX_PF_RGBA6_Z24,
    /* 0x2 */ GX_PF_RGB565_Z16,
    /* 0x3 */ GX_PF_Z24,
    /* 0x4 */ GX_PF_Y8,
    /* 0x5 */ GX_PF_U8,
    /* 0x6 */ GX_PF_V8,
    /* 0x7 */ GX_PF_YUV420,
} GXPixelFmt;

typedef enum _GXZFmt16 {
    /* 0x0 */ GX_ZC_LINEAR,
    /* 0x1 */ GX_ZC_NEAR,
    /* 0x2 */ GX_ZC_MID,
    /* 0x3 */ GX_ZC_FAR,
} GXZFmt16;

typedef enum _GXCommand {
    GX_CMD_LOAD_INDX_A = 0x20,
    GX_CMD_LOAD_INDX_B = 0x28,
    GX_CMD_LOAD_INDX_C = 0x30,
    GX_CMD_LOAD_INDX_D = 0x38,

    GX_CMD_LOAD_CP_REG = 0x08,
    GX_CMD_LOAD_XF_REG = 0x10,
} GXCommand;

typedef enum _GXClipMode {
    /* 0x0 */ GX_CLIP_ENABLE,
    /* 0x1 */ GX_CLIP_DISABLE,
} GXClipMode;

typedef enum _GXCopyMode {
    /* 0x0 */ GX_COPY_PROGRESSIVE,
    /* 0x1 */ GX_COPY_INTLC_EVEN,
    /* 0x2 */ GX_COPY_INTLC_ODD,
} GXCopyMode;

typedef enum _GXAlphaReadMode {
    /* 0x0 */ GX_READ_00,
    /* 0x1 */ GX_READ_FF,
    /* 0x2 */ GX_READ_NONE,
} GXAlphaReadMode;

typedef enum _GXTlutSize {
    /* 0x001 */ GX_TLUT_16 = 1,
    /* 0x002 */ GX_TLUT_32 = 2,
    /* 0x004 */ GX_TLUT_64 = 4,
    /* 0x008 */ GX_TLUT_128 = 8,
    /* 0x010 */ GX_TLUT_256 = 16,
    /* 0x020 */ GX_TLUT_512 = 32,
    /* 0x040 */ GX_TLUT_1K = 64,
    /* 0x080 */ GX_TLUT_2K = 128,
    /* 0x100 */ GX_TLUT_4K = 256,
    /* 0x200 */ GX_TLUT_8K = 512,
    /* 0x400 */ GX_TLUT_16K = 1024,
} GXTlutSize;

    typedef struct _GXColor
    {
        u8 r, g, b, a;
    } GXColor;

    typedef struct _SDK_GXColorS10
    {
        s16 r, g, b, a;
    } GXColorS10;
    typedef GXColorS10 _GXColorS10;

    typedef struct _GXRenderModeObj
    {
        s32 tvMode;
        u16 fbWidth;
        u16 efbHeight;
        u16 xfbHeight;
        u16 viXOrigin;
        u16 viYOrigin;
        u16 viWidth;
        u16 viHeight;
        s32 xfbMode;
        u8 field_rendering;
        u8 aa;
        u8 sample_pattern[12][2];
        u8 vfilter[7];
    } GXRenderModeObj;

    extern GXRenderModeObj GXNtsc240Ds;
    extern GXRenderModeObj GXNtsc240DsAa;
    extern GXRenderModeObj GXNtsc240Int;
    extern GXRenderModeObj GXNtsc240IntAa;
    extern GXRenderModeObj GXNtsc480IntDf;
    extern GXRenderModeObj GXNtsc480Int;
    extern GXRenderModeObj GXNtsc480IntAa;
    extern GXRenderModeObj GXNtsc480Prog;
    extern GXRenderModeObj GXNtsc480ProgSoft;
    extern GXRenderModeObj GXNtsc480ProgAa;

    extern GXRenderModeObj GXMpal240Ds;
    extern GXRenderModeObj GXMpal240DsAa;
    extern GXRenderModeObj GXMpal240Int;
    extern GXRenderModeObj GXMpal240IntAa;
    extern GXRenderModeObj GXMpal480IntDf;
    extern GXRenderModeObj GXMpal480Int;
    extern GXRenderModeObj GXMpal480IntAa;

    extern GXRenderModeObj GXPal264Ds;
    extern GXRenderModeObj GXPal264DsAa;
    extern GXRenderModeObj GXPal264Int;
    extern GXRenderModeObj GXPal264IntAa;
    extern GXRenderModeObj GXPal528IntDf;
    extern GXRenderModeObj GXPal528Int;
    extern GXRenderModeObj GXPal524IntAa;

    extern GXRenderModeObj GXEurgb60Hz240Ds;
    extern GXRenderModeObj GXEurgb60Hz240DsAa;
    extern GXRenderModeObj GXEurgb60Hz240Int;
    extern GXRenderModeObj GXEurgb60Hz240IntAa;
    extern GXRenderModeObj GXEurgb60Hz480IntDf;
    extern GXRenderModeObj GXEurgb60Hz480Int;
    extern GXRenderModeObj GXEurgb60Hz480IntAa;
    extern GXRenderModeObj GXRmHW;

    void GXAdjustForOverscan(const GXRenderModeObj *rmin, GXRenderModeObj *rmout,
                             u16 hor, u16 ver);
    void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht);
    void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
    void GXSetDispCopyDst(u16 wd, u16 ht);
    void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);

    void GXSetDispCopyFrame2Field(GXCopyMode mode);
    void GXSetCopyClamp(GXFBClamp clamp);
    u32 GXSetDispCopyYScale(f32 vscale);
    void GXSetCopyClear(GXColor clear_clr, u32 clear_z);
    void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]);
    void GXSetDispCopyGamma(GXGamma gamma);

    void GXCopyDisp(void *dest, GXBool clear);
    void GXCopyTex(void *dest, GXBool clear);

    f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
    u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale);

    void GXClearBoundingBox(void);
    void GXReadBoundingBox(u16 *left, u16 *top, u16 *right, u16 *bottom);
    void GXDrawDone(void);
    void GXPixModeSync(void);

    typedef struct _GXTexObj
    {
        u32 dummy[8];
    } GXTexObj;

    typedef struct
    {
        void *base;        // _00
        void *end;         // _04
        u32 size;          // _08
        u32 highWatermark; // _0C
        u32 lowWatermark;  // _10
        void *readPtr;     // _14
        void *writePtr;    // _18
        s32 rwDistance;    // _1C
        u8 _20[0x60];      // _20
    } GXFifoObj;

    //u16 GXGetNumXfbLines(float, u16);
    float GXGetYScaleFactor(u16, u16);

    void GXSetFog(GXFogType type, f32 startZ, f32 endZ, f32 nearZ, f32 farZ, GXColor color);
    //void GXSetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable *table);
    void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op);
    void GXSetColorUpdate(GXBool enable_update);
    void GXSetAlphaUpdate(GXBool enable_update);
    void GXSetZMode(GXBool enable_compare, GXCompare comp, GXBool enable_update);
    void GXSetZCompLoc(GXBool z_buf_before_tex);
    void GXSetPixelFmt(GXPixelFmt pixel_fmt, GXZFmt16 z_fmt);
    void GXSetDither(GXBool enable_dither);
    void GXSetDstAlpha(GXBool enable, u8 alpha);
    void GXSetFieldMask(GXBool odd_mask, GXBool even_mask);
    void GXSetFieldMode(GXBool field_mode, GXBool half_aspect_ratio);

    // GXMisc
    void GXFlush();
    void GXAbortFrame();
    void GXSetDrawDone();
    typedef void (*GXDrawDoneCallback)(void);
    void GXSetDrawDoneCallback(GXDrawDoneCallback callback);

    // GXFifo
    extern GXFifoObj *GXInit(void *base, u32 size);
    extern GXFifoObj *GXGetCPUFifo();
    extern void GXSetCPUFifo(GXFifoObj *obj);
    extern void GXSetGPFifo(GXFifoObj *obj);
    extern void GXSaveCPUFifo(GXFifoObj *obj);
    extern void GXInitFifoBase(GXFifoObj *obj, void *base, u32 size);
    extern void GXInitFifoPtrs(GXFifoObj *obj, void *readPtr, void *writePtr);
    extern void GXInitFifoLimits(GXFifoObj *obj, u32 hiWaterMark, u32 loWaterMark);
    inline void *GXGetFifoBase(GXFifoObj *fifo) {
        return fifo->base;
    }
    inline u32 GXGetFifoSize(GXFifoObj* fifo) {
        return fifo->size;
    }

    // GXTexture
    u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 max_lod);
    void __GetImageTileCount();
    void GXInitTexObj(GXTexObj *obj, void *image, u16 width, u16 height, GXTexFmt fmt,
                      GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXBool mipmap);
    void GXInitTexObjCI(GXTexObj *obj, void *image, u16 width, u16 height, GXCITexFmt format,
                        GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXBool mipmap, u32 tlut_name);
    void GXInitTexObjLOD(GXTexObj *obj, GXTexFilter min_filter, GXTexFilter max_filter, f32 min_lod,
                         f32 max_lod, f32 lod_bias, GXBool bias_clamp, GXBool edge_lod,
                         GXAnisotropy aniso);
    u16 GXGetTexObjWidth(GXTexObj *obj);
    u16 GXGetTexObjHeight(GXTexObj *obj);
    GXTexFmt GXGetTexObjFmt(GXTexObj *obj);
    GXTexWrapMode GXGetTexObjWrapS(GXTexObj *obj);
    GXTexWrapMode GXGetTexObjWrapT(GXTexObj *obj);
    GXBool GXGetTexObjMipMap(GXTexObj *obj);
    u32 GXGetTexObjTlut(GXTexObj *obj);
    void GXLoadTexObj(GXTexObj *obj, GXTexMapID id);
    void GXInitTlutObj(GXTlutObj *obj, void *lut, GXTlutFmt fmt, u16 entry_num);
    void GXLoadTlut(GXTlutObj *obj, u32 tlut_name);
    //void GXInitTexCacheRegion(GXTexRegion *region, GXBool is_32b_mipmap, u32 tmem_even,
                              //GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd);
    //void GXInitTlutRegion(GXTlutRegion *region, u32 tmem_addr, GXTlutSize tlut_size);
    void GXInvalidateTexAll(void);

    //GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback callback);
    //GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback callback);

    void GXSetTexCoordScaleManually(GXTexCoordID coord, GXBool enable, u16 s_scale, u16 t_scale);

    void __SetSURegs();
    void __GXSetSUTexRegs();
    void __GXSetTmemConfig();

    // GXAttr
    typedef struct _GXVtxDescList
    {
        /* 0x0 */ GXAttr attr;
        /* 0x4 */ GXAttrType type;
    } GXVtxDescList; // Size: 0x08

    typedef struct _GXVtxAttrFmtList
    {
        /* 0x00 */ GXAttr mAttrib;
        /* 0x04 */ GXCompCnt mCompCnt;
        /* 0x08 */ GXCompType mCompType;
        /* 0x0C */ u8 mCompShift;
    } GXVtxAttrFmtList; // Size: 0x10

    void GXSetVtxDesc(GXAttr attr, GXAttrType type);
    void GXSetVtxDescv(GXVtxDescList *list);
    void __GXSetVCD(void);
    void __GXCalculateVLim(void);
    void GXClearVtxDesc(void);
    void GXSetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u32);
    void GXSetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList *list);
    void __GXSetVAT(void);
    void GXSetArray(GXAttr attr, void *base, u8);
    void GXInvalidateVtxCache(void);
    void GXSetTexCoordGen2(GXTexCoordID dst, GXTexGenType type, GXTexGenSrc src, u32 mtx, GXBool renormalize, u32 pt_mtx);
    void GXSetNumTexGens(u8 numTexGens);

    // GXTransform
    void GXProject(f32 model_x, f32 model_y, f32 model_z, Mtx model_mtx, f32 *proj_mtx, f32 *viewpoint,
                   f32 *screen_x, f32 *screen_y, f32 *screen_z);
    void GXSetProjection(Mtx44 mtx, GXProjectionType type);
    void GXSetProjectionv(f32 *p);
    void GXGetProjectionv(f32 *p);
    void GXLoadPosMtxImm(Mtx mtx, u32 id);
    void GXLoadNrmMtxImm(Mtx mtx, u32 id);
    void GXSetCurrentMtx(u32 id);
    void GXLoadTexMtxImm(Mtx mtx, u32 id, GXTexMtxType type);
    void GXSetViewport(f32 x_orig, f32 y_orig, f32 width, f32 height, f32 near_z, f32 far_z);
    void GXGetViewportv(f32 *p);
    void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
    void GXGetScissor(u32 *left, u32 *top, u32 *width, u32 *height);
    void GXSetScissorBoxOffset(u32 x_offset, u32 y_offset);
    void GXSetClipMode(GXClipMode mode);

    // GXLight
    typedef struct _GXLightObj
    {
        /* 0x00 */ u8 field_0x0[0xc];
        /* 0x0C */ GXColor color;
        /* 0x10 */ f32 a0;
        /* 0x14 */ f32 a1;
        /* 0x18 */ f32 a2;
        /* 0x1C */ f32 k0;
        /* 0x20 */ f32 k1;
        /* 0x24 */ f32 k2;
        /* 0x28 */ f32 posX;
        /* 0x2C */ f32 posY;
        /* 0x30 */ f32 posZ;
        /* 0x34 */ f32 dirX;
        /* 0x38 */ f32 dirY;
        /* 0x3C */ f32 dirZ;
    } GXLightObj;

    void GXInitLightAttn(GXLightObj *obj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
    void GXInitLightSpot(GXLightObj *obj, f32 cutoff, GXSpotFn spot_fn);
    void GXInitLightDistAttn(GXLightObj *obj, f32 dist, f32 brightness, GXDistAttnFn dist_fn);
    void GXInitLightPos(GXLightObj *obj, f32 x, f32 y, f32 z);
    void GXInitLightDir(GXLightObj *obj, f32 x, f32 y, f32 z);
    void GXInitLightColor(GXLightObj *obj, GXColor color);
    void GXLoadLightObjImm(GXLightObj *obj, GXLightID light);
    void GXSetChanAmbColor(GXChannelID channel, GXColor color);
    void GXSetChanMatColor(GXChannelID channel, GXColor color);
    void GXSetNumChans(u8 chan_num);
    void GXSetChanCtrl(GXChannelID channel, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn);

// GXTev
    extern void GXSetTevOp(GXTevStageID stage, GXTevMode mode);
    void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c,
                         GXTevColorArg d);
    void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp,
                         GXTevRegID out_reg);
    void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c,
                         GXTevAlphaArg d);
    void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp,
                         GXTevRegID out_reg);
    void GXSetTevColor(GXTevRegID id, GXColor color);
    void GXSetTevColorS10(GXTevRegID id, GXColorS10 color);

    void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1);
    void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias);
    void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color);
    void GXSetNumTevStages(u8 num_stages);

// GXGeometry
    void __GXSetDirtyState(void);
    void GXBegin(GXPrimitive type, GXVtxFmt fmt, u16 vert_num);
    static void GXEnd(void) {}
    void __GXSendFlushPrim(void);
    void GXSetLineWidth(u8 width, GXTexOffset offsets);
    void GXSetPointSize(u8 size, GXTexOffset offsets);
    void GXEnableTexOffsets(GXTexCoordID coord, GXBool line, GXBool point);
    void GXSetCullMode(GXCullMode mode);
    void GXSetCoPlanar(GXBool enable);
    void __GXSetGenMode(void);

    // GXPerf
    void GXReadXfRasMetric(u32 *, u32 *, u32 *, u32 *);
    void GXGetGPStatus(GXBool *overhi, GXBool *underlo, GXBool *read_idle, GXBool *cmd_idle, GXBool *breakpoint);

    // INLINES
    //volatile PPCWGPipe GXFIFO : 0xCC008000;

#define GFX_FIFO(T) (*(volatile T *)0xCC008000)

    inline void GXPosition3f32(f32 x, f32 y, f32 z)
    {
        GFX_FIFO(f32) = x;
        GFX_FIFO(f32) = y;
        GFX_FIFO(f32) = z;
    }

    inline void GXColor1u32(u32 c)
    {
        GFX_FIFO(u32) = c;
    }

    inline void GXTexCoord2f32(f32 s, f32 t)
    {
        GFX_FIFO(f32) = s;
        GFX_FIFO(f32) = t;
    }

    inline void GXTexCoord2u8(u8 s, u8 t)
    {
        GFX_FIFO(u8) = s;
        GFX_FIFO(u8) = t;
    }

    inline void GXPosition2u16(u16 x, u16 y)
    {
        GFX_FIFO(u16) = x;
        GFX_FIFO(u16) = y;
    }

    inline void GXPosition1x16(u16 x)
    {
        GFX_FIFO(u16) = x;
    }

    inline void GXPosition1x8(u8 x)
    {
        GFX_FIFO(u8) = x;
    }

    inline void GXPosition3s8(s8 x, s8 y, s8 z)
    {
        GFX_FIFO(s8) = x;
        GFX_FIFO(s8) = y;
        GFX_FIFO(s8) = z;
    }

    inline void GXPosition2s8(s8 x, s8 y)
    {
        GFX_FIFO(s8) = x;
        GFX_FIFO(s8) = y;
    }

    inline void i_GXPosition3s16(s16 x, s16 y, s16 z)
    {
        GFX_FIFO(s16) = x;
        GFX_FIFO(s16) = y;
        GFX_FIFO(s16) = z;
    }

    inline void GXTexCoord2s8(s8 x, s8 y)
    {
        GFX_FIFO(s8) = x;
        GFX_FIFO(s8) = y;
    }

    inline void i_GXEnd() {}

#ifdef __cplusplus
};
#endif // ifdef __

#endif // !GX_H
