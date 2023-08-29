#ifndef JAUDIO_JASMEMPOOL_H
#define JAUDIO_JASMEMPOOL_H

#include "types.h"
#include <dolphin/os.h>

struct JASGenericMemPool
{
    JASGenericMemPool();
    void free(void *, u32);

    u32 _0[4];
};

template <typename T>
struct JASMemPool : public JASGenericMemPool
{
    JASMemPool<T>() : JASGenericMemPool() {}
    ~JASMemPool<T>();
};

template <typename T>
struct JASPoolAllocObject
{
    static void operator delete(void *mem, u32 n)
    {
        BOOL inter = OSDisableInterrupts();
        memPool_.free(mem, n);
        OSRestoreInterrupts(inter);
    }
    static JASMemPool<T> memPool_;
};

template <typename T>
struct JASMemPool_MultiThreaded : public JASGenericMemPool
{
    JASMemPool_MultiThreaded<T>() : JASGenericMemPool() { }
    ~JASMemPool_MultiThreaded<T>();
};


template <typename T>
struct JASPoolAllocObject_MultiThreaded
{
    static void operator delete(void *mem, u32 n)
    {
        BOOL inter = OSDisableInterrupts();
        memPool_.free(mem, n);
        OSRestoreInterrupts(inter);
    }
    static JASMemPool_MultiThreaded<T> memPool_;
    static bool __init__memPool_;
};



#endif