#ifndef _JSYSTEM_JKR_JKRDVDRIPPER_H
#define _JSYSTEM_JKR_JKRDVDRIPPER_H

#include "dolphin/OS.h"
#include "dolphin/dvd.h"
#include "JSystem/JKernel/JKRDvdFile.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

enum JKRExpandSwitch
{
    Switch_0 = 0,
    Switch_1,
    Switch_2
};
class JKRDvdFile;

class JKRDMCommand {
    JKRDMCommand();
    ~JKRDMCommand();
};

namespace JKRDvdRipper { // not sure if this is a class/struct or a namespace(if it is a class, it could be inherited from JKRRipper perhaps?)
    enum EAllocDirection { // Placeholder
        DIRECTION_0,
        DIRECTION_1,
        DIRECTION_2,
    };
    // could also be u8 * return, however most functions seem to use void *
    void * loadToMainRAM(const char *, u8 *, JKRExpandSwitch, u32, JKRHeap *, EAllocDirection, u32, int *, u32 *); 
    void * loadToMainRAM(s32, u8 *, JKRExpandSwitch, u32, JKRHeap *, EAllocDirection, u32, int *, u32 *);
    void * loadToMainRAM(JKRDvdFile *, u8 *, JKRExpandSwitch, u32, JKRHeap *, EAllocDirection, u32, int *, u32 *);
    // Inline/Unused
    void loadToMainRAMAsync(const char *, u8 *, JKRExpandSwitch, u32, JKRHeap *, u32 *);
    void loadToMainRAMAsync(s32, u8 *, JKRExpandSwitch, u32, JKRHeap *, u32 *);
    void loadToMainRAMAsync(JKRDvdFile *, u8 *, JKRExpandSwitch, u32, JKRHeap *, u32* );

    void sync(JKRDMCommand *, int);
    void syncAll(int);
    void countLeftSync();

    extern JSUList<JKRDMCommand> sDvdAsyncList;

    // these both exist too JKRAramRipper? copy paste or is something else going on? i'm guessing copy paste
    extern bool errorRetry; 
    extern s32 sSZSBufferSize; // 0x400
    // Weak
    bool isErrorRetry() { return errorRetry; } // this is unused but for whatever reason it gets linked
}

void JKRDecompressFromDVD(JKRDvdFile *, void *, u32, u32, u32, u32, u32);

#endif