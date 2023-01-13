#ifndef GX_H
#define GX_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

    typedef u8 GXBool;

#define GX_TRUE ((GXBool)1)
#define GX_FALSE ((GXBool)0)
#define GX_ENABLE ((GXBool)1)
#define GX_DISABLE ((GXBool)0)

    typedef enum _GXTexFmt
    {
        GX_TF_I4,     // 4-bit intensity
        GX_TF_I8,     // 8-bit intensity
        GX_TF_IA4,    // 8-bit intensity + alpha (4+4).
        GX_TF_IA8,    // 16-bit intensity + alpha (8+8).
        GX_TF_RGB565, // 16-bit RGB.
        GX_TF_RGB5A3, // When MSB=1, RGB555 format (opaque), and when MSB=0, RGBA4443 format (transparent).
        GX_TF_RGBA8,  // 32-bit RGB.
        GX_TF_CMPR,   // Compressed 4-bit texel.
        GX_TF_Z8,     // Unsigned 8-bit Z. For texture copies, specify the upper 8 bits of Z.
        GX_TF_Z16,    // Unsigned 16-bit Z. For texture copies, specify the upper 16 bits of Z.
        GX_TF_Z24X8,  // Unsigned 24-bit (32-bit texture) Z. For texture copies, copy the 24-bit Z and 0xff.
        GX_CTF_R4,    // 4-bit red. For copying 4 bits from red.
        GX_CTF_RA4,   // 4-bit red + 4-bit alpha. For copying 4 bits from red, 4 bits from alpha.
        GX_CTF_RA8,   // 8-bit red + 8-bit alpha. For copying 8 bits from red, 8 bits from alpha.
        GX_CTF_A8,    // 8-bit alpha. For copying 8 bits from alpha.
        GX_CTF_R8,    // 8-bit red. For copying 8 bits from red.
        GX_CTF_G8,    // 8-bit green. For copying 8 bits from green.
        GX_CTF_B8,    // 8-bit blue. For copying 8 bits from blue.
        GX_CTF_RG8,   // 8-bit red +8-bit green. For copying 8 bits from red, 8 bits from green.
        GX_CTF_GB8,   // 8-bit green +8-bit blue. For copying 8 bits from green, 8 bits from blue.
        GX_CTF_Z4,    // 4-bit Z. For copying the 4 upper bits from Z.
        GX_CTF_Z8M,   // 8-bit Z (median byte). For copying the middle 8 bits of Z.
        GX_CTF_Z8L,   // 8-bit Z (lower byte). For copying the lower 8 bits of Z.
        GX_CTF_Z16L   // 16-bit Z (lower portion). For copying the lower 16 bits of Z.
    } GXTexFmt;

    typedef enum _GXTexCoordID
    {
        GX_TEXCOORD0,
        GX_TEXCOORD1,
        GX_TEXCOORD2,
        GX_TEXCOORD3,
        GX_TEXCOORD4,
        GX_TEXCOORD5,
        GX_TEXCOORD6,
        GX_TEXCOORD7,
        GX_MAX_TEXCOORD = 8,
        GX_TEXCOORD_NULL = 0xff
    } GXTexCoordID;

    typedef enum _GXTexMapID
    {
        GX_TEXMAP0, // Texture map ID 0.
        GX_TEXMAP1, // Texture map ID 1.
        GX_TEXMAP2, // Texture map ID 2.
        GX_TEXMAP3, // Texture map ID 3.
        GX_TEXMAP4, // Texture map ID 4.
        GX_TEXMAP5, // Texture map ID 5.
        GX_TEXMAP6, // Texture map ID 6.
        GX_TEXMAP7, // Texture map ID 7.
        GX_MAX_TEXMAP,
        GX_TEXMAP_NULL = 0xff, // No textures used.
        GX_TEX_DISABLE = 0x100 // No texture map look-up.
    } GXTexMapID;

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

    //void GXSetDispCopyFrame2Field(GXCopyMode mode);
    //void GXSetCopyClamp(GXFBClamp clamp);
    //u32 GXSetDispCopyYScale(f32 vscale);
    //void GXSetCopyClear(GXColor clear_clr, u32 clear_z);
    //void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]);
    //void GXSetDispCopyGamma(GXGamma gamma);

    void GXCopyDisp(void *dest, GXBool clear);
    void GXCopyTex(void *dest, GXBool clear);

    f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
    u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale);

    void GXClearBoundingBox(void);
    void GXReadBoundingBox(u16 *left, u16 *top, u16 *right, u16 *bottom);
    void GXDrawDone(void);
    void GXPixModeSync(void);

    typedef struct GXTexObj
    {
        u32 _00;
        u32 _04;
        u32 _08;
        u32 _0c;
        u8 _10;
        u8 _11;
        u8 _12;
        u8 _13;
        u32 format_14;
        u32 tlut_name_18;
        u16 _1c;
        s8 _1e;
        s8 mipmap_1f;
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

#ifdef __cplusplus
};
#endif // ifdef __

#endif // !GX_H
