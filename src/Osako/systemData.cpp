#include "Osako/systemData.h"

const int SystemData::scNumStandardHeaps = 2;
const int SystemData::scDefaultFifoSize = 0x80000;
const int SystemData::scSystemHeapSize = 0xE0000;
const int SystemData::scAudioHeapSize = 0xC0000;
const int SystemData::scAudioAramSize = 0xA20000;

const SystemData::ViewPort SystemData::sc3DViewPort = {
    0.0f, 0.0f, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT};

const SystemData::Scissor SystemData::sc3DScissor = {
    0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT};

const SystemData::Scissor SystemData::sc3DAwardScissor = {
    0, 34, GAME_SCREEN_WIDTH, 380};

const SystemData::ViewPort SystemData::scOrtho = {
    0.0f, 0.0f, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT};

const SystemData::ViewPort SystemData::sc3DViewPortSub = { 
    437.0f, 312.0f, 149.0f, 112.0f};

const SystemData::Scissor SystemData::sc3DScissorSub = {
    437, 312, 149, 112};

#ifndef VIDEO_PAL
const _GXRenderModeObj SystemData::scNtscInt448Df = {
    VI_TVMODE_NTSC_INT,
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT,
    GAME_VI_X_OFFSET,
    GAME_VI_Y_OFFSET,
    GAME_VI_WIDTH,
    GAME_VI_HEIGHT,
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
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT,
    GAME_VI_X_OFFSET,
    GAME_VI_Y_OFFSET,
    GAME_VI_WIDTH,
    GAME_VI_HEIGHT,
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
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT,
    GAME_VI_X_OFFSET,
    GAME_VI_Y_OFFSET,
    GAME_VI_WIDTH,
    GAME_VI_HEIGHT,
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
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT,
    GAME_VI_X_OFFSET,
    GAME_VI_Y_OFFSET,
    GAME_VI_WIDTH,
    GAME_VI_HEIGHT,
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

const _GXRenderModeObj SystemData::scPalInt448Df = {
    VI_TVMODE_PAL_INT,
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT_PAL,
    GAME_VI_X_OFFSET_PAL,
    GAME_VI_Y_OFFSET_PAL,
    GAME_VI_WIDTH_PAL,
    GAME_VI_HEIGHT_PAL,
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
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT_PAL,
    GAME_VI_X_OFFSET_PAL,
    GAME_VI_Y_OFFSET_PAL,
    GAME_VI_WIDTH_PAL,
    GAME_VI_HEIGHT_PAL,
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
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT,
    GAME_VI_X_OFFSET,
    GAME_VI_Y_OFFSET,
    GAME_VI_WIDTH,
    GAME_VI_HEIGHT,
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
    GAME_SCREEN_WIDTH,
    GAME_SCREEN_HEIGHT,
    GAME_SCREEN_HEIGHT,
    GAME_VI_X_OFFSET,
    GAME_VI_Y_OFFSET,
    GAME_VI_WIDTH,
    GAME_VI_HEIGHT,
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

const f32 SystemData::scAspect = (f32)(sc3DViewPort.W / sc3DViewPort.H);

// 2 Player Splitscreen screen coordinates/size
const SystemData::ViewPort SystemData::sca3DViewPortDiv2[2] = {
    {
        sc3DViewPort.X,                               // 0
        sc3DViewPort.Y,                               // 0
        sc3DViewPort.W,                               // 608
        (sc3DViewPort.H / 2) - 1.0f                   // 223
    },
    {
        sc3DViewPort.X,                               // 0
        sc3DViewPort.Y + (sc3DViewPort.H / 2) + 1.0f, // 225
        sc3DViewPort.W,                               // 608
        (sc3DViewPort.H / 2) - 1.0f                   // 223
    }};

const SystemData::Scissor SystemData::sca3DScissorDiv2[2] = {
    {sc3DScissor.X,
     sc3DScissor.Y,
     sc3DScissor.W,
     (sc3DScissor.H / 2) - 1},
    {sc3DScissor.X,
     sc3DScissor.Y + (sc3DScissor.H / 2) + 1,
     sc3DScissor.W,
     (sc3DScissor.H / 2) - 1}};

const f32 SystemData::scAspectDiv2 = SystemData::sc3DViewPort.W / ((sc3DViewPort.H / 2) - 1.0f); // 223);

// 4 Player Splitscreen screen coordinates/size
const SystemData::ViewPort SystemData::sca3DViewPortDiv4[4] = {
    {
        sc3DViewPort.X,                               // 0
        sc3DViewPort.Y,                               // 0
        (sc3DViewPort.W / 2) - 1.0f,                  // 303
        (sc3DViewPort.H / 2) - 1.0f},                 // 223
    {
        sc3DViewPort.X + (sc3DViewPort.W / 2) + 1.0f, // 305
        sc3DViewPort.Y,                               // 0
        (sc3DViewPort.W / 2) - 1.0f,                  // 303
        (sc3DViewPort.H / 2) - 1.0f                   // 223
    },
    {
        sc3DViewPort.X,                               // 0
        sc3DViewPort.Y + (sc3DViewPort.H / 2) + 1.0f, // 225
        (sc3DViewPort.W / 2) - 1.0f,                  // 303
        (sc3DViewPort.H / 2) - 1.0f                   // 223
    },
    {
        sc3DViewPort.X + (sc3DViewPort.W / 2) + 1.0f, // 305
        sc3DViewPort.Y + (sc3DViewPort.H / 2) + 1.0f, // 225
        (sc3DViewPort.W / 2) - 1.0f,                  // 303
        (sc3DViewPort.H / 2) - 1.0f                   // 223
    }};

const SystemData::Scissor SystemData::sca3DScissorDiv4[4] = {
    {sc3DScissor.X,
     sc3DScissor.Y,
     (sc3DScissor.W / 2) - 1,
     (sc3DScissor.H / 2) - 1},
    {sc3DScissor.X + sc3DScissor.W / 2 + 1,
     sc3DScissor.Y,
     (sc3DScissor.W / 2) - 1,
     (sc3DScissor.H / 2) - 1},
    {sc3DScissor.X,
     sc3DScissor.Y + (sc3DScissor.H / 2) + 1,
     (sc3DScissor.W / 2) - 1,
     (sc3DScissor.H / 2) - 1},
    {sc3DScissor.X + sc3DScissor.W / 2 + 1,
     sc3DScissor.Y + (sc3DScissor.H / 2) + 1,
     (sc3DScissor.W / 2) - 1,
     (sc3DScissor.H / 2) - 1}};

const f32 SystemData::scAspectSub = SystemData::sc3DViewPortSub.W / SystemData::sc3DViewPortSub.H;
