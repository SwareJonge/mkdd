#ifndef GX_H
#define GX_H

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

#endif // !GX_H
