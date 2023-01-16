#ifndef _JSYSTEM_JFW_JFWSYSTEM_H
#define _JSYSTEM_JFW_JFWSYSTEM_H

#include "dolphin/gx.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTFont.h"
#include "types.h"

// Maybe these are namespaces?
struct JFWSystem
{
    struct CSetUpParam
    {
        static u32 maxStdHeaps;
        static u32 sysHeapSize;
        static u32 fifoBufSize;
        static u32 aramAudioBufSize;
        static u32 aramGraphBufSize;
        static s32 streamPriority;
        static s32 decompPriority;
        static s32 aPiecePriority;
        static ResFONT *systemFontRes;
        static GXRenderModeObj *renderMode;
        static u32 exConsoleBufferSize;
    };

    static void firstInit();
    static void init();

    static JKRHeap *rootHeap;
    static JKRHeap *systemHeap;
    static JKRThread *mainThread;
    static JUTDbPrint *debugPrint;
    static JUTFont *systemFont;
    static JUTConsoleManager *systemConsoleManager;
    static JUTConsole *systemConsole;
    static bool sInitCalled;


    
    // No idea how they formatted this lol
    static void setMaxStdHeaps(u32 stdHeaps) { 
        JUT_ASSERT(47, sInitCalled == 0);
        CSetUpParam::maxStdHeaps = stdHeaps; }
    static void setSysHeapSize(u32 heapSize) {
        JUT_ASSERT(50, sInitCalled == 0);
        CSetUpParam::sysHeapSize = heapSize; }
    static void setFifoBufSize(u32 bufSize) {
        JUT_ASSERT(53, sInitCalled == 0);
        CSetUpParam::fifoBufSize = bufSize; }
    
    // Inlines for Aram
    static void setAramAudioBufSize(u32 bufSize) {
        JUT_ASSERT(58, sInitCalled == 0);
        CSetUpParam::aramAudioBufSize = bufSize; }
    static void setAramGraphBufSize(u32 bufSize) {
        JUT_ASSERT(61, sInitCalled == 0);
        CSetUpParam::aramGraphBufSize = bufSize; }
    // probably some more inlines for other variables, not used by MKDD
    static void setRenderMode(GXRenderModeObj * rmode) {
        JUT_ASSERT(80, sInitCalled == 0);
        CSetUpParam::renderMode = rmode; }
};

#endif