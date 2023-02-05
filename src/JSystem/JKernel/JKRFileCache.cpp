#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRFileCache.h"

// this file misses all functions except for a weak destructor, so i only added the function that is needed and the function calls inside it are stubs, i've not tested if the function matches

JKRDvdFinder * JKRFileCache::getFirstFile(const char *path) const
{
    char *dvdPathName = getDvdPathName(path);
    JKRDvdFinder * fileFinder = new (JKRGetSystemHeap(), 0) JKRDvdFinder(dvdPathName);
    JKRFreeToSysHeap(dvdPathName);
    if (fileFinder->isAvailable()) {
        return fileFinder;
    }
    delete fileFinder;
    return nullptr;
}

// stub
char *JKRFileCache::getDvdPathName(const char *) const
{
    return nullptr;
}