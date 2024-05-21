#ifndef JAUDIO_JASWSPARSER_H
#define JAUDIO_JASWSPARSER_H

#include "JSystem/JAudio/System/JASWaveBank.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport.h"

#include "types.h"

namespace JASWSParser
{
    template <typename T>
    class TOffset
    {
    public:
        T *ptr(const void *p) const
        {
            return JSUConvertOffsetToPtr<T>(p, mOffset);
        }
        u32 mOffset;
    };

    struct TCtrlWave
    {
        u32 mTableSize; // _00
    };
    struct TWave
    {
        u8 _00;      // _00
        u8 _01;      // _01
        u8 _02;      // _02
        f32 _04;     // _04
        u32 mOffset; // _08
        u32 _0C;     // _0C
        u32 _10;     // _10
        u32 _14;     // _14
        u32 _18;     // _18
        u32 _1C;     // _1C
        s16 _20;     // _20
        s16 _22;     // _22
    };
    struct TWaveArchive
    {
        char mFileName[0x74];           // _00 - unknown length
        TOffset<TWave> mWaveOffsets[1]; // _74 - dynamic length
    };
    struct TWaveArchiveBank
    {
        u8 _00[8];                                // _00 - unknown/padding
        TOffset<TWaveArchive> mArchiveOffsets[1]; // _08 - dynamic length
    };
    struct TCtrl
    {
        u8 _00[4];                              // _00 - unknown/padding
        u32 mWaveCount;                         // _04
        TOffset<TCtrlWave> mCtrlWaveOffsets[1]; // _08 - dynamic length
    };
    struct TCtrlScene
    {
        u8 _00[0xc];                // _00 - unknown/padding
        TOffset<TCtrl> mCtrlOffset; // _0C
    };
    struct TCtrlGroup
    {
        u8 _00[8];                                // _00 - unknown/padding
        u32 mCtrlGroupCount;                      // _08
        TOffset<TCtrlScene> mCtrlSceneOffsets[1]; // _0C - dynamic length
    };

    // fabricated
    struct THeader
    {
        u8 _00[0xC];                                  // _00 - unknown/padding
        u32 mWaveTableSize;                           // _0C
        TOffset<TWaveArchiveBank> mArchiveBankOffset; // _10
        TOffset<TCtrlGroup> mCtrlGroupOffset;         // _14
    };

    // Global
    JASWaveBank *createWaveBank(const void *p, JKRHeap *pHeap);             // 0x8009682c
    JASBasicWaveBank *createBasicWaveBank(const void *p, JKRHeap *pHeap);   // 0x80096890
    JASSimpleWaveBank *createSimpleWaveBank(const void *p, JKRHeap *pHeap); // 0x80096ab8
    // Inline/Unused
    u32 getGroupCount(const void *p);
    u32 getUsedHeapSize();

    extern u32 sUsedHeapSize;
}; // namespace JASWSParser

#endif