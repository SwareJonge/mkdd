#ifndef JAUDIO_JASBNKPARSER_H
#define JAUDIO_JASBNKPARSER_H

#include "JSystem/JAudio/System/JASBank.h"
#include "JSystem/JAudio/System/JASOscillator.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport.h"

#include "types.h"

// Ver0
namespace JASBNKParser
{
    const u32 MAX_INST = 128;
    const int MAX_OSC = 2;
    template <typename T>
    class TOffset
    {
    public:
        T *ptr(const void *p) const
        {
            return JSUConvertOffsetToPtr<T>(p, mOffset);
        }
    private:
        u32 mOffset;
    };

    struct TVmap
    {
        u8 _0;  // 0
        u32 _4; // 4
        f32 _8; // 8
        f32 _c; // c
    };

    struct TRand
    {
        u8 type;     // 0
        f32 floor;   // 4
        f32 ceiling; // 8
    };

    struct TSense
    {
        u8 type;      // 0
        u8 trigger;   // 1
        u8 centerkey; // 2
        f32 _4;       // 4
        f32 _8;       // 8
    };

    struct TKeymap
    {
        u8 high;                       // 0
        u32 veloRegionCount;           // 4
        TOffset<TVmap> vmapOffsets[1]; // 8
    };

    struct TOsc
    {
        u8 target;                                     // 00
        f32 _4;                                        // 04
        TOffset<JASOscillator::Point> oscPointOffset;  // 08
        TOffset<JASOscillator::Point> oscPointOffset2; // 0C
        f32 scale;                                     // 10
        f32 _14;                                       // 14
    };

    struct TInst
    {
        u32 id;                               // 00
        u8 _04[4];                            // 04
        f32 volume;                           // 08
        f32 pitch;                            // 0C
        TOffset<TOsc> oscOffsets[2];          // 10, 14
        TOffset<TRand> randOffsets[2];        // 18, 1c
        TOffset<TSense> senseOffsets[2];      // 20, 24
        u32 keyRegionCount;                   // 28
        TOffset<TKeymap> keyRegionOffsets[1]; // 2C, dynamic size
    };

    struct TPmap
    {
        f32 volume;                          // 00
        f32 pitch;                           // 04
        TOffset<TRand> randOffsets[2];       // 08
        u32 veloRegionCount;                 // 0C
        TOffset<TVmap> veloRegionOffsets[1]; // 10,dynamic size
    };

    struct TPerc
    {
        u32 magic;                        // 00
        u8 _04[0x88 - 0x4];               // 04 unk
        TOffset<TPmap> pmapOffsets[0x80]; // 88
        s8 panTable[0x80];                // 288
        u16 releaseTable[0x80];           // 308
    };

    struct TBank
    {
        u32 magic;                        // 20
        TOffset<TInst> instOffsets[0x80]; // 24
        u8 _224[0x3b4 - 0x224];           // 224, some other offsets? not used, always 0(0x64 in size)
        TOffset<TPerc> percOffsets[0xc];  // 3B4
    };

    struct THeader
    {
        u32 id;            // 00
        u32 size;          // 04
        u32 bankId;        // 08
        u8 _C[0x20 - 0xc]; // 0C padding most likely(ALIGN(0x20)?)
        TBank bank;        // 20
    };

    // Global
    JASBank *createBank(const void *p, JKRHeap *pHeap);           // 0x8009850c
    JASBasicBank *createBasicBank(const void *p, JKRHeap *pHeap); // 0x8009852c

    // Inline/Unused
    u32 getUsedHeapSize();

    extern u32 sUsedHeapSize;
}; // namespace JASBNKParser

#endif // JASBNKPARSER_H