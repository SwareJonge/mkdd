#ifndef J3DCLUSTERLOADER_H
#define J3DCLUSTERLOADER_H

#include "types.h"

struct JUTDataBlockHeader {
    u32 mType; // 00
    u32 mSize; // 04

    const JUTDataBlockHeader* getNext() const {  // fake inline
        return reinterpret_cast<const JUTDataBlockHeader*>(reinterpret_cast<const u8*>(this) +
                                                           mSize);
    }
};

struct JUTDataFileHeader {  // actual struct name unknown
    u32 mMagic;                     // 00
    u32 mType;                      // 04
    u32 mFileSize;                  // 08
    u32 mBlockNum;                  // 0C
    u8 _10[0x1C - 0x10];            // 10
    u32 mSeAnmOffset;               // 1C
    JUTDataBlockHeader mFirstBlock; // 20
};


struct J3DClusterLoaderDataBase {
    static void* load(void const*);
};


class J3DClusterBlock : public JUTDataBlockHeader {
public:
    u16 mClusterNum;          // 08
    u16 mClusterKeyNum;       // 0a
    u16 mClusterVertexNum;    // 0c
    u16 mVtxPosNum;           // 0e
    u16 mVtxNrmNum;           // 10
    void* mClusterPointer;    // 14
    void* mClusterKeyPointer; // 18
    void* mClusterVertex;     // 1C
    void* mVtxPos;            // 20
    void* mVtxNrm;            // 24
    void* mClusterName;       // 28
    void* mClusterKeyName;    // 2C
};

class J3DClusterLoader {
public:
    virtual void* load(const void*) = 0;
    virtual ~J3DClusterLoader() {}
};

class J3DDeformData;

class J3DClusterLoader_v15 : public J3DClusterLoader {
public:
    J3DClusterLoader_v15();
    void readCluster(J3DClusterBlock const*);

    virtual void* load(void const*);
    virtual ~J3DClusterLoader_v15();

    J3DDeformData* mpDeformData; // 04
};

#endif /* J3DCLUSTERLOADER_H */
