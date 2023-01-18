#ifndef _JSYSTEM_JKR_JKRDVDRIPPER_H
#define _JSYSTEM_JKR_JKRDVDRIPPER_H
#include "types.h"
#include "dolphin/OS.h"
#include "dolphin/dvd.h"
#include "JSystem/JKernel/JKRHeap.h"

enum JKRExpandSwitch
{
    Switch_0 = 0,
    Switch_1,
    Switch_2
};
class JKRDvdFile;

class JKRDMCommand {

};

namespace JKRDvdRipper {
    enum EAllocDirection { // Placeholder
        DIRECTION_0,
        DIRECTION_1,
        DIRECTION_2,
    };
    // could also be u8 * return, however most functions seem to use void *
    static void * loadToMainRAM(const char *, u8 *, JKRExpandSwitch, u32, JKRHeap *, EAllocDirection, u32, int *, u32 *); 
    static void * loadToMainRAM(s32, u8 *, JKRExpandSwitch, u32, JKRHeap *, EAllocDirection, u32, int *, u32 *);
    static void * loadToMainRAM(JKRDvdFile *, u8 *, JKRExpandSwitch, u32, JKRHeap *, EAllocDirection, u32, int *, u32 *);

    static void loadToMainRAMAsync(const char *, u8 *, JKRExpandSwitch, u32, JKRHeap *, u32 *);
    static void loadToMainRAMAsync(s32, u8 *, JKRExpandSwitch, u32, JKRHeap *, u32 *);
    static void loadToMainRAMAsync(JKRDvdFile *, u8 *, JKRExpandSwitch, u32, JKRHeap *, u32* );

    bool doneProcess(s32, DVDFileInfo *);
    void sync(JKRDMCommand *, int);
    void syncAll(int);
    void countLeftSync();
    bool isErrorRetry() {
        return errorRetry;
    }

    extern bool errorRetry;

}

#endif