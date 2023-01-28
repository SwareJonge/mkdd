#ifndef JKRFILELOADER_H_
#define JKRFILELOADER_H_

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"

class JKRFileFinder;
class JKRFileLoader : public JKRDisposer {
public:
    JKRFileLoader();

    virtual ~JKRFileLoader();                                                                          // _08
    virtual void unmount();                                                                            // _0C
    virtual bool becomeCurrent(const char *) = 0;                                                      // _10
    virtual void *getResource(const char *path) = 0;                                                   // _14
    virtual void *getResource(u32 type, const char *name) = 0;                                         // _18
    virtual size_t readResource(void *resourceBuffer, u32 bufferSize, const char *path) = 0;           // _1C
    virtual size_t readResource(void *resourceBuffer, u32 bufferSize, u32 type, const char *name) = 0; // _20
    virtual void removeResourceAll() = 0;                                                              // _24
    virtual bool removeResource(void *) = 0;                                                           // _28
    virtual bool detachResource(void *) = 0;                                                           // _2C
    virtual long getResSize(const void *) const = 0;                                                   // _30
    virtual u32 countFile(const char *) const = 0;                                                     // _34
    virtual JKRFileFinder *getFirstFile(const char *) const = 0;                                       // _38

    bool isMounted() const { return mIsMounted; }
    u32 getVolumeType() const { return mVolumeType; }

    static void *getGlbResource(char const *);
    static void *getGlbResource(char const *, JKRFileLoader *);
    static JKRFileLoader *findVolume(char const **);
    static void fetchVolumeName(char *, long, char const *);

    static JKRFileLoader *sCurrentVolume;
    static JSUList<JKRFileLoader> sVolumeList;

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