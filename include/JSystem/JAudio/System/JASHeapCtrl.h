#ifndef JAUDIO_JASHEAPCTRL_H
#define JAUDIO_JASHEAPCTRL_H

#include <dolphin/os.h>

#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JAudio/System/JASHeap.h"

#include "dolphin/os/OSMutex.h"
#include "types.h"


struct JASGenericMemPool
{
    struct TNextOnFreeList {
        u8 _0[4];
    };

    JASGenericMemPool();
    ~JASGenericMemPool();
    void newMemPool(u32 n, int count);
    void free(void *p, u32 n);
    void *alloc(u32 n);

    /*bool isActive() {
        return mTotalMemCount != mFreeMemCount;
    }*/

    int getFreeMemCount() const { return mFreeMemCount; }
    int getTotalMemCount() const { return mTotalMemCount; }

    void *mRunner;
    int mFreeMemCount;
    int mTotalMemCount;
    u32 mUsedMemCount;
};

template <typename T>
struct JASMemPool : public JASGenericMemPool
{
    JASMemPool<T>() : JASGenericMemPool() {}
    ~JASMemPool<T>() {}

    typedef JASMemPool<T> JASMemPoolT;

    int getTotalMemCount() const {
        JASThreadingModel::SingleThreaded<JASMemPoolT>::Lock(*this);
        return JASGenericMemPool::getTotalMemCount();
    }

    int getFreeMemCount() const {
        JASThreadingModel::SingleThreaded<JASMemPoolT>::Lock(*this);
        return JASGenericMemPool::getFreeMemCount();
    }

    void newMemPool(int count) {
        JASThreadingModel::SingleThreaded<JASMemPoolT>::Lock(*this);
        JASGenericMemPool::newMemPool(sizeof(T), count);
    }

    void free(void *p, u32 n) {
#line 187
        JUT_ASSERT(n == sizeof(T));
        JASThreadingModel::SingleThreaded<JASMemPoolT>::Lock(*this);
        JASGenericMemPool::free(p, n);
    }

    T *alloc(u32 n)
    {
        JUT_ASSERT(n == sizeof(T));
        JASThreadingModel::SingleThreaded<JASMemPoolT>::Lock(*this);
        return (T*)JASGenericMemPool::alloc(n); // evil cast fixes stuff
    }
};

template<u32 ChunkSize, template<typename> class T>
class JASMemChunkPool
{
public:
    JASMemChunkPool() {
        OSInitMutex(&mMutex);
        mChunk = NULL;
        bool ret = createNewChunk();
#line 315
        JUT_ASSERT(ret);
    }
    
    typedef JASMemChunkPool<ChunkSize, T> TJASMemChunkPool;
    typedef typename T<TJASMemChunkPool>::Lock TLock;
    friend class T<TJASMemChunkPool>::Lock;

    struct MemoryChunk
    {
        MemoryChunk(MemoryChunk *nextChunk)
        {
            mNextChunk = nextChunk;
            mUsedSize = 0;
            mChunks = 0;
        }

        bool checkArea(const void *ptr) const
        {
            return (u8 *)this + 0xc <= (u8 *)ptr && (u8 *)ptr < (u8 *)this + (ChunkSize + 0xc);
        }

        MemoryChunk *getNextChunk()
        {
            return mNextChunk;
        }

        void *alloc(u32 size)
        {
            u8 *rv = mBuffer + mUsedSize;
            mUsedSize += size;
            mChunks++;
            return rv;
        }

        void free(void*)
        {
            mChunks--;
        }

        bool isEmpty() const
        {
            return mChunks == 0;
        }

        void setNextChunk(MemoryChunk *chunk)
        {
            mNextChunk = chunk;
        }

        u32 getFreeSize() const
        {
            return ChunkSize - mUsedSize;
        }

        void revive()
        {
            mUsedSize = 0;
        }

        MemoryChunk *mNextChunk; // 00
        u32 mUsedSize;           // 04
        u32 mChunks;             // 08
        u8 mBuffer[ChunkSize];   // 0c
    };

public:
    void free(void *ptr)
    {
        TLock lock(*this); // takes *this
        MemoryChunk *chunk = mChunk;
        MemoryChunk *prevChunk = NULL;
        while (chunk != NULL)
        {
            if (chunk->checkArea(ptr))
            {
                chunk->free(ptr);

                if ((chunk != mChunk && chunk->isEmpty()))
                {
                    MemoryChunk *nextChunk = chunk->getNextChunk();
                    delete chunk;
                    prevChunk->setNextChunk(nextChunk);
                }
                return;
            }
            prevChunk = chunk;
            chunk = chunk->getNextChunk();
        }
#line 357
        JUT_ASSERT_MSG(false, "Cannnot free for JASMemChunkPool");
    }

    bool createNewChunk()
    {
        if ((mChunk != NULL && mChunk->isEmpty()))
        {
            mChunk->revive();
            return true;
        }

        MemoryChunk *chunk = mChunk;
        mChunk = new (JASKernel::getSystemHeap(), 0) MemoryChunk(chunk);
        if (mChunk != NULL)
        {
            return true;                
        }
    #line 390
        JUT_WARNING_F2("%s", "Not enough JASSystemHeap");
        mChunk = new (JKRHeap::getSystemHeap(), 0) MemoryChunk(chunk);
        if (mChunk != NULL)
        {
            return true;
        }

        mChunk = chunk;
        return false;            
        
    }

    void *alloc(u32 size)
    {
        TLock lock(*this); // takes *this
        u32 freeSize = mChunk->getFreeSize();
        if (freeSize < size)
        {
            if (ChunkSize < size)
            {
                return NULL;
            }
            if (createNewChunk() == 0)
            {
                return NULL;
            }
        }
        return mChunk->alloc(size);
    }

private:
    mutable OSMutex mMutex;
    MemoryChunk *mChunk; // 18
};

template <typename T>
struct JASPoolAllocObject
{
    static void newMemPool(int count) 
    {
        memPool_.newMemPool(count);
    }

    static void operator delete(void *mem, u32 n)
    {
        memPool_.free(mem, n);
    }

    static void *operator new(u32 n) {
        return memPool_.alloc(n);
    }

    static bool isActive() {
        return memPool_.isActive();
    }

    static u32 getFreeMemCount() { return memPool_.getFreeMemCount(); }
    static u32 getTotalMemCount() { return memPool_.getTotalMemCount(); }

private:
    static JASMemPool<T> memPool_;
};

template <typename T>
struct JASMemPool_MultiThreaded : public JASGenericMemPool
{
    typedef JASMemPool_MultiThreaded<T> TJASMemPool_MultiThreaded;
    typedef typename JASThreadingModel::InterruptsDisable<TJASMemPool_MultiThreaded>::Lock TLock;
    
    void newMemPool(int count)
    {
        TLock lock(*this);
        JASGenericMemPool::newMemPool(sizeof(T), count);
    }

    void *alloc(u32 n)
    {
        TLock lock(*this);
        return JASGenericMemPool::alloc(n);
    }

    void free(void* ptr, u32 n) {
        TLock lock(*this);
        JASGenericMemPool::free(ptr, n);
    }

    
};


template <typename T>
struct JASPoolAllocObject_MultiThreaded
{
    static void newMemPool(int count)
    {
        memPool_.newMemPool(count);
    }

    static void *operator new(u32 n)
    {
        return memPool_.alloc(n);
    }
    static void operator delete(void *mem, u32 n)
    {
        memPool_.free(mem, n);
    }
private:
    static JASMemPool_MultiThreaded<T> memPool_;
};

#endif
