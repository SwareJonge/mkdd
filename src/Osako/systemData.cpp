#include <dolphin/gx.h>
#include <dolphin/vi.h>
#include "Osako/systemData.h"

// TODO: rename stuff, general cleanup

#define SCREEN_WIDTH 608
#define VI_WIDTH 666
#define SCREEN_HEIGHT 448
#define VI_X_OFFSET (VI_MAX_WIDTH_NTSC - VI_WIDTH) / 2
#define VI_Y_OFFSET (VI_MAX_HEIGHT_NTSC - SCREEN_HEIGHT) / 2

const int SystemData::scNumStandardHeaps = 2;
const int SystemData::scDefaultFifoSize = 0x80000;
const int SystemData::scSystemHeapSize = 0xE0000;
const int SystemData::scAudioHeapSize = 0xC0000;
const int SystemData::scAudioAramSize = 0xA20000;

const ScreenDataf SystemData::sc3DViewPort = {
    0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT};

const ScreenDatai SystemData::sc3DScissor = {
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

const ScreenDatai SystemData::sc3DAwardScissor = {
    0, 34, SCREEN_WIDTH, 380};

const ScreenDataf SystemData::scOrtho = {
    0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT};

// these seem like split screen values
const ScreenDataf SystemData::sc3DViewPortSub = { 
    437.0f, 312.0f, 149.0f, 112.0f};

const ScreenDatai SystemData::sc3DScissorSub = {
    437, 312, 149, 112};

#ifndef REGION_EU
const _GXRenderModeObj SystemData::scNtscInt448Df = {
    VI_TVMODE_NTSC_INT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    VI_X_OFFSET,
    VI_Y_OFFSET,
    VI_WIDTH,
    SCREEN_HEIGHT,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {32, 0, 32, 0,
     0, 0, 0}};

const _GXRenderModeObj SystemData::scNtscInt448 = {
    VI_TVMODE_NTSC_INT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    VI_X_OFFSET,
    VI_Y_OFFSET,
    VI_WIDTH,
    SCREEN_HEIGHT,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {0, 0, 21, 22,
     21, 0, 0}};

const _GXRenderModeObj SystemData::scNtscProg448Soft = {
    VI_TVMODE_NTSC_PROG,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    VI_X_OFFSET,
    VI_Y_OFFSET,
    VI_WIDTH,
    SCREEN_HEIGHT,
    VI_XFBMODE_SF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {2, 2, 17, 22,
     17, 2, 2}};

const _GXRenderModeObj SystemData::scNtscProg448 = {
    VI_TVMODE_NTSC_PROG,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    VI_X_OFFSET,
    VI_Y_OFFSET,
    VI_WIDTH,
    SCREEN_HEIGHT,
    VI_XFBMODE_SF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {0, 0, 21, 22, 21, 0, 0}};
#else

#define PAL_VI_WIDTH 670
#define SCREEN_HEIGHT_PAL 538
#define VI_PAL_X_OFFSET (VI_MAX_WIDTH_PAL - PAL_VI_WIDTH) / 2
#define VI_PAL_Y_OFFSET (VI_MAX_HEIGHT_PAL - SCREEN_HEIGHT_PAL) / 2

const _GXRenderModeObj SystemData::scPalInt448Df = {
    VI_TVMODE_PAL_INT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT_PAL,
    VI_PAL_X_OFFSET,
    VI_PAL_Y_OFFSET,
    PAL_VI_WIDTH,
    SCREEN_HEIGHT_PAL,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {32, 0, 32, 0,
     0, 0, 0}};

const _GXRenderModeObj SystemData::scPalInt448 = {
    VI_TVMODE_PAL_INT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT_PAL,
    VI_PAL_X_OFFSET,
    VI_PAL_Y_OFFSET,
    PAL_VI_WIDTH,
    SCREEN_HEIGHT_PAL,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {0, 0, 21, 22,
     21, 0, 0}};

const _GXRenderModeObj SystemData::scEuRgb60Int448Df = {
    VI_TVMODE_EURGB60_INT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    VI_X_OFFSET,
    VI_Y_OFFSET,
    VI_WIDTH,
    SCREEN_HEIGHT,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {32, 0, 32, 0,
     0, 0, 0}};

const _GXRenderModeObj SystemData::scEuRgb60Int448 = {
    VI_TVMODE_EURGB60_INT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    VI_X_OFFSET,
    VI_Y_OFFSET,
    VI_WIDTH,
    SCREEN_HEIGHT,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},

    {0, 0, 21, 22,
     21, 0, 0}};
#endif

const float SystemData::scAspect = (float)(sc3DViewPort.W / sc3DViewPort.H);

#define getHalfSize(size) size / 2 // this shoudn't be needed, instead make a proper macro that already has this calculated?

// 2 Player Splitscreen screen coordinates/size
const ScreenDataf SystemData::sca3DViewPortDiv2[2] = {
    {
        sc3DViewPort.X,                                      // 0
        sc3DViewPort.Y,                                      // 0
        sc3DViewPort.W,                                      // 608
        getHalfSize(sc3DViewPort.H) - 1.0f                   // 223
    },
    {
        sc3DViewPort.X,                                      // 0
        sc3DViewPort.Y + getHalfSize(sc3DViewPort.H) + 1.0f, // 225
        sc3DViewPort.W,                                      // 608
        getHalfSize(sc3DViewPort.H) - 1.0f                   // 223
    }};

const ScreenDatai SystemData::sca3DScissorDiv2[2] = {
    {sc3DScissor.X,
     sc3DScissor.Y,
     sc3DScissor.W,
     getHalfSize(sc3DScissor.H) - 1},
    {sc3DScissor.X,
     sc3DScissor.Y + getHalfSize(sc3DScissor.H) + 1,
     sc3DScissor.W,
     getHalfSize(sc3DScissor.H) - 1}};
const float SystemData::scAspectDiv2 = SystemData::sc3DViewPort.W / (getHalfSize(sc3DViewPort.H) - 1.0f); // 223);

// 4 Player Splitscreen screen coordinates/size
const ScreenDataf SystemData::sca3DViewPortDiv4[4] = {
    {
        sc3DViewPort.X,                                      // 0
        sc3DViewPort.Y,                                      // 0
        getHalfSize(sc3DViewPort.W) - 1.0f,                  // 303
        getHalfSize(sc3DViewPort.H) - 1.0f},                 // 223
    {
        sc3DViewPort.X + getHalfSize(sc3DViewPort.W) + 1.0f, // 305
        sc3DViewPort.Y,                                      // 0
        getHalfSize(sc3DViewPort.W) - 1.0f,                  // 303
        getHalfSize(sc3DViewPort.H) - 1.0f                   // 223
    },
    {
        sc3DViewPort.X,                                      // 0
        sc3DViewPort.Y + getHalfSize(sc3DViewPort.H) + 1.0f, // 225
        getHalfSize(sc3DViewPort.W) - 1.0f,                  // 303
        getHalfSize(sc3DViewPort.H) - 1.0f                   // 223
    },
    {
        sc3DViewPort.X + getHalfSize(sc3DViewPort.W) + 1.0f, // 305
        sc3DViewPort.Y + getHalfSize(sc3DViewPort.H) + 1.0f, // 225
        getHalfSize(sc3DViewPort.W) - 1.0f,                  // 303
        getHalfSize(sc3DViewPort.H) - 1.0f                   // 223
    }};

const ScreenDatai SystemData::sca3DScissorDiv4[4] = {
    {sc3DScissor.X,
     sc3DScissor.Y,
     getHalfSize(sc3DScissor.W) - 1,
     getHalfSize(sc3DScissor.H) - 1},
    {sc3DScissor.X + sc3DScissor.W / 2 + 1,
     sc3DScissor.Y,
     getHalfSize(sc3DScissor.W) - 1,
     getHalfSize(sc3DScissor.H) - 1},
    {sc3DScissor.X,
     sc3DScissor.Y + getHalfSize(sc3DScissor.H) + 1,
     getHalfSize(sc3DScissor.W) - 1,
     getHalfSize(sc3DScissor.H) - 1},
    {sc3DScissor.X + sc3DScissor.W / 2 + 1,
     sc3DScissor.Y + getHalfSize(sc3DScissor.H) + 1,
     getHalfSize(sc3DScissor.W) - 1,
     getHalfSize(sc3DScissor.H) - 1}};

const float SystemData::scAspectSub = SystemData::sc3DViewPortSub.W / SystemData::sc3DViewPortSub.H;
