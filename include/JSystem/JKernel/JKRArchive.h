#ifndef JKRARCHIVE_H_
#define JKRARCHIVE_H_

#include "JSystem/JKernel/JKRFileLoader.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class JKRDvdFile;
class SArcDataInfo;
class SDirEntry;
class SDIFileEntry;

inline u16 read_big_endian_u16(void *ptr) {
    u8 *uptr = (u8 *)ptr;
    return ((u16)uptr[0] << 8) | ((u16)uptr[1]);
}

inline u32 read_big_endian_u32(void *ptr) {
    u8 *uptr = (u8 *)ptr;
    return ((u32)uptr[0] << 0x18) | ((u32)uptr[1] << 0x10) | ((u32)uptr[2] << 8) | (u32)uptr[3];
}

class JKRArchive : public JKRFileLoader {
public:
    enum EMountMode {
        UNKNOWN_MOUNT_MODE = 0,
        MOUNT_MEM = 1,
        MOUNT_ARAM = 2,
        MOUNT_DVD = 3,
        MOUNT_COMP = 4,
    };

    enum EMountDirection {
        UNKNOWN_MOUNT_DIRECTION = 0,
        MOUNT_DIRECTION_HEAD = 1,
        MOUNT_DIRECTION_TAIL = 2,
    };

    class CArcName {
    public:
        CArcName() {}

        u16 getHash() const { return mHash; }

        const char* getString() const { return mData; }

    private:
        u16 mHash;
        u16 mLength;
        char mData[256];
    };

protected:
    JKRArchive();
    JKRArchive(long, EMountMode);
    virtual ~JKRArchive();

public:
    u32 getMountMode() const { return mMountMode; }

protected:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRFileLoader
    /* 0x38 */ JKRHeap* mHeap;
    /* 0x3C */ u8 mMountMode;
    /* 0x3D */ u8 field_0x3d[3];
    /* 0x40 */ s32 mEntryNum;
    /* 0x44 */ SArcDataInfo* mArcInfoBlock;
    /* 0x48 */ SDirEntry* mNodes;
    /* 0x4C */ SDIFileEntry* mFiles;
    /* 0x50 */ s32* mExpandedSize;
    /* 0x54 */ char* mStringTable;
    /* 0x58 */ u32 field_0x58;
};

#endif