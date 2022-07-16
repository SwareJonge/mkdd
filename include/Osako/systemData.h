#ifndef SYSTEMDATA_H
#define SYSTEMDATA_H

#include "types.h"
#include "Dolphin/gx.h"

struct ScreenDataf {
    float X;
    float Y;
    float W;
    float H;
};

struct ScreenDatai {
    int X;
    int Y;
    int W;
    int H;
};

namespace SystemData {

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

    static const ScreenDataf sca3DViewPortDiv2[2];
    static const ScreenDatai sca3DScissorDiv2[2];
    static const ScreenDatai sca3DViewPortDiv4[4];

    static const float scAspect;
    static const float scAspectDiv2;
    static const float scAspectSub;

    // Add PAL stuff in the future
    static const _GXRenderModeObj scNtscInt448Df;
    static const _GXRenderModeObj scNtscInt448;
    static const _GXRenderModeObj scNtscInt448Soft;
    static const _GXRenderModeObj scNtscProg448;
}

#endif // !SYSTEMDATA_H
