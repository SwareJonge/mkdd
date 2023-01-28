#ifndef JKRARAM_H
#define JKRARAM_H

//#include "dolphin/ar.h"
#include "dolphin/os.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JSupport/JSUList.h"
#include "types.h"

struct JKRAramHeap;

struct JKRAramBlock
{
    JKRAramBlock(u32, u32, u32, u8, bool);

    virtual ~JKRAramBlock(); // _08

    JKRAramBlock *allocHead(u32, u8, JKRAramHeap *);
    JKRAramBlock *allocTail(u32, u8, JKRAramHeap *);

    JSULink<JKRAramBlock> m_link; // _04
    u32 _14;                      // _14
    u32 _18;                      // _18
    u32 _1C;                      // _1C
    u8 _20;                       // _20
    bool _21;                     // _21
};

struct JKRAram : public JKRThread
{
    JKRAram(u32, u32, long);

    virtual ~JKRAram();  // _08
    virtual void *run(); // _0C

    static JKRAram *create(u32, u32, long, long, long);
    static JKRAramBlock *mainRamToAram(u8 *, u32, u32, JKRExpandSwitch, u32, JKRHeap *, s32, u32 *);
    static u8 *aramToMainRam(u32, u8 *, u32, JKRExpandSwitch, u32, JKRHeap *, s32, u32 *);
    static u8 *aramToMainRam(JKRAramBlock *, u8 *, u32, u32, JKRExpandSwitch, u32, JKRHeap *, s32, u32 *);

    u8 _7C[4];               // _7C
    u32 _80;                 // _80
    u32 _84;                 // _84
    u32 _88;                 // _88
    u8 _8C[4];               // _8C
    u32 _90;                 // _90
    JKRAramHeap *m_aramHeap; // _94
    u32 m_blockLength;       // _98
    u8 _9C[4];               // _9C

    static JKRAram *sAramObject;
    static const OSMessageQueue sMessageQueue;
};

struct JKRAramArchive : public JKRArchive
{
    JKRAramArchive(long, EMountDirection);

    virtual ~JKRAramArchive();                                       // _08
    virtual size_t getExpandedResSize(const void *) const;           // _3C
    virtual void *fetchResource(SDIFileEntry *, u32 *);              // _40
    virtual void *fetchResource(void *, u32, SDIFileEntry *, u32 *); // _44

    bool open(long);
    static u32 fetchResource_subroutine(u32, u32, u8 *, u32, int);
    static u32 fetchResource_subroutine(u32, u32, JKRHeap *, int, u8 **);

    // _00     = VTBL
    // _00-_5C = JKRArchive
    CompressionMethod mCompression;     // _5C
    EMountDirection mMountDirection; // _60
    JKRAramBlock *mBlock;            // _64
    JKRDvdFile *mDvdFile;            // _68
};

#endif