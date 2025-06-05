#ifndef JAUDIO_JASWAVEARCLOADER_H
#define JAUDIO_JASWAVEARCLOADER_H

#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASDisposer.h"

#include "dolphin/os/OSMutex.h"


#define DIR_MAX 64

namespace JASWaveArcLoader
{
    JASHeap *getRootHeap();
    void setRootHeap(JASHeap *);
    void setCurrentDir(const char *);
    char *getCurrentDir();

    extern JASHeap *sAramHeap;
}

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
    bool isLoading() const;

    virtual ~JASWaveArc();        // 08
    virtual void onDispose();     // 0C
    virtual void onLoadDone() {}  // 10
    virtual void onEraseDone() {} // 14

    // Inline/Unused
    void execLoad();
    void activate();
    void loadBlock(JASHeap *heap);
    
    struct loadToAramCallbackParams // not official struct name
    {
        JASWaveArc *mWavArc; // 0
        s32 mEntryNum;       // 4
        u32 mBase;           // 8
        u32 _c;              // C
    };

    JASHeap mHeap;    // 04
    u32 _48;          // 4C
    volatile s32 _4c; // 50
    s32 mEntryNum;    // 54
    u32 mFileLength;  // 58
    u16 _58;          // 5C
    u16 _5a;          // 5E
    OSMutex mMutex;   // 60
};

#endif
