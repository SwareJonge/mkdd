#ifndef SYSTEMDATA_H
#define SYSTEMDATA_H

#include "dolphin/gx.h"
#include "types.h"

// maybe just use 2 dimensional arrays, or TBox<TVec2<T>?
struct ScreenDataf {
    f32 X;
    f32 Y;
    f32 W;
    f32 H;
};

struct ScreenDatai {
    u32 X;
    u32 Y;
    u32 W;
    u32 H;
};

class SystemData {
public:
    static const int scNumStandardHeaps;
    static const int scDefaultFifoSize;
    static const int scSystemHeapSize;
    static const int scAudioHeapSize;
    static const int scAudioAramSize;

    static const ScreenDataf sc3DViewPort;
    static const ScreenDatai sc3DScissor;
    static const ScreenDatai sc3DAwardScissor;
    static const ScreenDataf scOrtho;
    static const ScreenDataf sc3DViewPortSub;
    static const ScreenDatai sc3DScissorSub;

    static const _GXRenderModeObj scNtscInt448Df;
    static const _GXRenderModeObj scNtscInt448;
    static const _GXRenderModeObj scNtscProg448Soft;
    static const _GXRenderModeObj scNtscProg448;
    static const _GXRenderModeObj scPalInt448Df;
    static const _GXRenderModeObj scPalInt448;
    static const _GXRenderModeObj scEuRgb60Int448Df;
    static const _GXRenderModeObj scEuRgb60Int448;

    static const ScreenDataf sca3DViewPortDiv2[2];
    static const ScreenDatai sca3DScissorDiv2[2];
    static const ScreenDataf sca3DViewPortDiv4[4];
    static const ScreenDatai sca3DScissorDiv4[4];

    static const f32 scAspect;
    static const f32 scAspectDiv2;
    static const f32 scAspectSub;
};

#endif // !SYSTEMDATA_H
