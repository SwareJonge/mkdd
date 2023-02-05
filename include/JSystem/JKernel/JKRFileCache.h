#ifndef JKRFILECACHE_H
#define JKRFILECACHE_H

class JKRDvdFinder;

struct JKRFileCache
{
    /* virtual */ JKRDvdFinder *getFirstFile(const char *) const;
    char *getDvdPathName(const char *) const;
};

#endif