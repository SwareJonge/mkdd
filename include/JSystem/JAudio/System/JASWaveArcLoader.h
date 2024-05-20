#ifndef JAUDIO_JASWAVEARCLOADER_H
#define JAUDIO_JASWAVEARCLOADER_H

#include "JSystem/JAudio/System/JASHeap.h"
#include "dolphin/os/OSMutex.h"

struct JASDisposer
{
    JASDisposer() {}
    virtual ~JASDisposer() {}
    virtual void onDispose();
};

#define DIR_MAX 64

struct JASWaveArcLoader
{
    static JASHeap *getRootHeap();
    static void setCurrentDir(const char *);
    static char *getCurrentDir();

    static char sCurrentDir[DIR_MAX];
    static JASHeap *sAramHeap;
};

struct JASWaveArc : JASDisposer
{
    JASWaveArc();
    bool loadSetup(u32);
    bool eraseSetup();
    static void loadToAramCallback(void *);
    bool sendLoadCmd();
    bool load(JASHeap *);
    bool loadTail(JASHeap *);
    void erase();
    void setEntryNum(s32);
    void setFileName(const char *);

    virtual ~JASWaveArc();
    virtual void onDispose();
    virtual void onLoadDone();
    virtual void onEraseDone();

    struct loadToAramCallbackParams // not official struct name
    {

        JASWaveArc *mWavArc; // 0
        long mEntryNum;      // 4
        u32 mBase;           // 8
        u32 _c;              // C
    };

    JASHeap mHeap;    // 04
    u32 _48;          // 48
    volatile s32 _4c; // 4C
    int mEntryNum;    // 50
    u32 mFileLength;  // 54
    u16 _58;          // 58
    u16 _5a;          // 5A
    OSMutex mMutex;   // 5C
};

#endif