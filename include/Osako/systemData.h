#ifndef SYSTEMDATA_H
#define SYSTEMDATA_H

#include <dolphin/gx.h>
#include <dolphin/vi.h>
#include "types.h"

/*
    MKDD doesn't seem to follow any standard when it comes to the resolution and aspect ratio

    This is the standard for DTV(NTSC)
    SAR 22:15
    PAR 10:11
    DAR 4:3
    704x480 -> 640x480

    I guess they used the VI width to calculate the xfb/efb width which would result in ~605.455 rounded to nearest number that's mod4 which is 608
    But even then the active area wouldn't be 4:3
*/

#define CALC_VI_OFFSET(max, actual) ((max - actual)/2)
#define CALC_AR(w,h) (w/h)

#define GAME_VI_WIDTH 666
#define GAME_VI_HEIGHT 448
#define GAME_VI_WIDTH_PAL 670
#define GAME_VI_HEIGHT_PAL 538

#define GAME_SCREEN_WIDTH 608
#define GAME_SCREEN_HEIGHT 448
#define GAME_SCREEN_HEIGHT_PAL 538

#define GAME_VI_X_OFFSET CALC_VI_OFFSET(VI_MAX_WIDTH_NTSC, GAME_VI_WIDTH)
#define GAME_VI_Y_OFFSET CALC_VI_OFFSET(VI_MAX_HEIGHT_NTSC, GAME_VI_HEIGHT)
#define GAME_VI_X_OFFSET_PAL CALC_VI_OFFSET(VI_MAX_WIDTH_PAL, GAME_VI_WIDTH_PAL)
#define GAME_VI_Y_OFFSET_PAL CALC_VI_OFFSET(VI_MAX_HEIGHT_PAL, GAME_VI_HEIGHT)

class SystemData {
public:
    // maybe just use 2 dimensional arrays, or TBox<TVec2<T>?
    struct Scissor {
        u32 X;
        u32 Y;
        u32 W;
        u32 H;
    };
    struct ViewPort {
        f32 X;
        f32 Y;
        f32 W;
        f32 H;
    };

    static const int scNumStandardHeaps;
    static const int scDefaultFifoSize;
    static const int scSystemHeapSize;
    static const int scAudioHeapSize;
    static const int scAudioAramSize;

    static const ViewPort sc3DViewPort;
    static const Scissor sc3DScissor;
    static const Scissor sc3DAwardScissor;
    static const ViewPort scOrtho;
    static const ViewPort sc3DViewPortSub;
    static const Scissor sc3DScissorSub;

    static const GXRenderModeObj scNtscInt448Df;
    static const GXRenderModeObj scNtscInt448;
    static const GXRenderModeObj scNtscProg448Soft;
    static const GXRenderModeObj scNtscProg448;
    static const GXRenderModeObj scPalInt448Df;
    static const GXRenderModeObj scPalInt448;
    static const GXRenderModeObj scEuRgb60Int448Df;
    static const GXRenderModeObj scEuRgb60Int448;

    static const ViewPort sca3DViewPortDiv2[2];
    static const Scissor sca3DScissorDiv2[2];
    static const ViewPort sca3DViewPortDiv4[4];
    static const Scissor sca3DScissorDiv4[4];

    static const f32 scAspect;
    static const f32 scAspectDiv2;
    static const f32 scAspectSub;
};

#endif // !SYSTEMDATA_H
