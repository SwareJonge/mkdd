#ifndef JKRFILELOADER_H_
#define JKRFILELOADER_H_

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"

//class JKRFileFinder;
class JKRFileLoader : public JKRDisposer {
public:
    JKRFileLoader(void);
    virtual ~JKRFileLoader();

    bool isMounted() const { return this->mIsMounted; }
    u32 getVolumeType() const { return this->mVolumeType; }
    //static JSUList<JKRFileLoader> sVolumeList;

protected:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRDisposer
    /* 0x18 */ JSULink<JKRFileLoader> mFileLoaderLink;
    /* 0x28 */ const char* mVolumeName;
    /* 0x2C */ u32 mVolumeType;
    /* 0x30 */ bool mIsMounted;
    /* 0x31 */ u8 field_0x31[3];
    /* 0x34 */ u32 mMountCount;
};

#endif