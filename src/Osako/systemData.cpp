#include "Osako/systemData.h"
#include "Dolphin/gx.h"

namespace SystemData { // might be a class

#define SCREEN_WIDTH 608
#define SCREEN_HEIGHT 448
    const int scNumStandardHeaps = 2;
    const int scDefaultFifoSize = 0x80000;
    const int scSystemHeapSize = 0xE0000;
    const int scAudioHeapSize = 0xC0000;
    const int scAudioAramSize = 0xA20000;

    const ScreenDataf sc3DViewPort = {
        0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT
    };

    const ScreenDatai sc3DScissor = {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };

    const ScreenDatai sc3DAwardScissor = {
        0, 34, SCREEN_WIDTH, 380
    };

    const ScreenDataf sc3DViewPortSub = { // these seem like split screen values
        437.0f, 312.0f, 149.0f, 112.0f
    };

    const ScreenDataf sc3DScissorSub = {
        437, 312, 149, 112
    };

    /*const _GXRenderModeObj scNtscInt448Df = {
        0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HEIGHT,
        27, 16, 666, SCREEN_HEIGHT, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2, (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2
        0, 1, 0, 0,
        {
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
        },

        {
            32, 0, 32, 0,
            0, 0, 0
        },
    };

    const _GXRenderModeObj scNtscInt448 = {
        0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HEIGHT,
        27, 16, 666, SCREEN_HEIGHT, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2, (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2
        0, 1, 0, 0,
        {
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
        },

        {
            0, 0, 21, 22,
            21, 0, 0
        },
    };

    const _GXRenderModeObj scNtscInt448Df = {
        2, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HEIGHT,
        27, 16, 666, SCREEN_HEIGHT, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2, (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2
        0, 1, 0, 0,
        {
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
        },

        {
            2, 2, 17, 22,
            17, 2, 2
        },
    };

    const _GXRenderModeObj scNtscProg448 = {
        2, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HEIGHT,
        27, 16, 666, SCREEN_HEIGHT, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2, (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2
        0, 1, 0, 0,
        {
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
            {6,6}, {6,6}, {6,6}, {6,6},
        },

        {
            0, 0, 21, 22,
            21, 0, 0
        },
    };*/

    /*const ScreenDataf sca3DViewPortDiv2[2] = { // i assume this is used for 2 player splitscreen
        {
            sc3DViewPort.X, sc3DViewPort.Y,
            sc3DViewPort.W,  sc3DViewPort.H * 0.5f - 1.0f 
        },
        {
            sc3DViewPort.X, 1.0f + sc3DViewPort.X + sc3DViewPort.W * 0.5f,
            sc3DViewPort.W * 0.5f,  sc3DViewPort.H * 0.5f - 1.0f
        }
    };


    // Aspect Ratio, this gets put at the end
    const float scAspect = sc3DViewPort.W / sc3DViewPort.H;
    const float scAspectDiv2 = sc3DViewPort.W / sca3DViewPortDiv2[0].H;
    const float scAspectSub = sc3DViewPortSub.W / sc3DViewPortSub.H;*/
}