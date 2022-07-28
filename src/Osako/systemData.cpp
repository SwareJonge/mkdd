#include "Osako/systemData.h"
#include "Dolphin/gx.h"

#define SCREEN_WIDTH 608
#define SCREEN_HEIGHT 448

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

const ScreenDataf SystemData::sc3DViewPortSub = { // these seem like split screen values
    437.0f, 312.0f, 149.0f, 112.0f};

const ScreenDatai SystemData::sc3DScissorSub = {
    437, 312, 149, 112};

const _GXRenderModeObj SystemData::scNtscInt448Df = {
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    27, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2
    16, // (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2 0
    666,
    SCREEN_HEIGHT,
    1,
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
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    27, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2
    16, // (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2 0
    666,
    SCREEN_HEIGHT,
    1,
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
    2,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    27, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2
    16, // (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2 0
    666,
    SCREEN_HEIGHT,
    0,
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
    2,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_HEIGHT,
    27, // probably (MAX_NTSC_WIDTH - SCREEN_WIDTH) / 2
    16, // (MAX_NTSC_HEIGTH - SCREEN_HEIGHT) / 2 0
    666,
    SCREEN_HEIGHT,
    0,
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

const float SystemData::scAspect = (float)(sc3DViewPort.W / sc3DViewPort.H);

#define getHalfSize(size) size / 2

const ScreenDataf SystemData::sca3DViewPortDiv2[2] = { // i assume this is used for 2 player splitscreen
    {
        sc3DViewPort.X,                    // 0
        sc3DViewPort.Y,                    // 0
        sc3DViewPort.W,                    // 608
        getHalfSize(sc3DViewPort.H) - 1.0f // 223
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

const ScreenDataf SystemData::sca3DViewPortDiv4[4] = {
    {sc3DViewPort.X,
     sc3DViewPort.Y,
     getHalfSize(sc3DViewPort.W) - 1.0f,
     getHalfSize(sc3DViewPort.H) - 1.0f},
    {
        sc3DViewPort.X + getHalfSize(sc3DViewPort.W) + 1.0f,
        sc3DViewPort.Y,
        getHalfSize(sc3DViewPort.W) - 1.0f,
        getHalfSize(sc3DViewPort.H) - 1.0f // 223
    },
    {
        sc3DViewPort.X,
        sc3DViewPort.Y + getHalfSize(sc3DViewPort.H) + 1.0f, // 225
        getHalfSize(sc3DViewPort.W) - 1.0f,
        getHalfSize(sc3DViewPort.H) - 1.0f // 223
    },
    {
        sc3DViewPort.X + getHalfSize(sc3DViewPort.W) + 1.0f,
        sc3DViewPort.Y + getHalfSize(sc3DViewPort.H) + 1.0f, // 225
        getHalfSize(sc3DViewPort.W) - 1.0f,
        getHalfSize(sc3DViewPort.H) - 1.0f // 223
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
