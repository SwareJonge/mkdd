#include "JSystem/JKernel/JKRDvdRipper.h"

// WIP, missing classes

namespace JKRDvdRipper {

    void *loadToMainRAM(const char *fileName, u8 *ptr, JKRExpandSwitch expSwitch, u32 p4, JKRHeap *heap, EAllocDirection allocDirection, u32 p7, int *p8, u32 *p9) {
        void * file;
        JKRDvdFile dvdFile;
        if (dvdFile.open(fileName)) {
            file = loadToMainRAM(&dvdFile, ptr, expSwitch, p4, heap, allocDirection, p7, p8, p9);
            delete &dvdFile;
        }
        else {
            delete &dvdFile;
            file = nullptr;
        }
        return file;
    }

    void *loadToMainRAM(s32 entryNum, u8 *ptr, JKRExpandSwitch expSwitch, u32 p4, JKRHeap *heap, EAllocDirection allocDirection, u32 p7, int *p8, u32 *p9) {
        void * file;
        JKRDvdFile dvdFile;
        if (dvdFile.open(entryNum)) {
            file = loadToMainRAM(&dvdFile, ptr, expSwitch, p4, heap, allocDirection, p7, p8, p9);
            delete &dvdFile;
        }
        else {
            delete &dvdFile;
            file = nullptr;
        }
        return file;
    }

    void *loadToMainRAM(JKRDvdFile * jkrDvdFile, u8 * file, JKRExpandSwitch expandSwitch, u32 p4, JKRHeap * heap, EAllocDirection allocDirection, u32 p7, int * p8, u32 *p9) {
        if (expandSwitch == Switch_1) {
            DVDReadPrio(&jkrDvdFile->mDvdFileInfo, , 0x20, 0, 2);
        }

            return file;
    }
}