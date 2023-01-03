#ifndef GX_H
#define GX_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

    typedef u8 GXBool;

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

    u16 GXGetNumXfbLines(float, u16);
    float GXGetYScaleFactor(u16, u16);

#ifdef __cplusplus
};
#endif // ifdef __

#endif // !GX_H
