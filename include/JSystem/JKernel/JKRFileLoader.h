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
    static const char * fetchVolumeName(char *, long, char const *);

    static JKRFileLoader *getCurrentVolume() { return sCurrentVolume; }
    static void setCurrentVolume(JKRFileLoader *fileLoader) { sCurrentVolume = fileLoader; }
    static JSUList<JKRFileLoader> &getVolumeList() { return sVolumeList; }

    static JKRFileLoader *sCurrentVolume;
    static JSUList<JKRFileLoader> sVolumeList;

protected:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRDisposer
    JSULink<JKRFileLoader> mFileLoaderLink; // 0x18
    const char *mVolumeName;                // 0x28
    u32 mVolumeType;                        // 0x2C
    bool mIsMounted;                        // 0x30
    u8 field_0x31[3];                       // 0x31
    u32 mMountCount;                        // 0x34
};

inline void *JKRGetNameResource(const char *path, JKRFileLoader *loader) { return JKRFileLoader::getGlbResource(path, loader); }
inline void *JKRGetResource(const char *path) { return JKRFileLoader::getGlbResource(path); }

#endif
