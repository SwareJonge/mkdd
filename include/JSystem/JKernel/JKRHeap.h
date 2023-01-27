#ifndef JKRHEAP_H
#define JKRHEAP_H

#include "dolphin/OS.h"
#include "JSystem/JKernel/JKRDisposer.h"

#include "types.h"

typedef void (*JKRErrorHandler)(void *, u32, int);

class JKRHeap : public JKRDisposer {
public:
    class TState
    {
    public:
        /* 0x00 */ u32 mUsedSize;
        /* 0x04 */ u32 mCheckCode;
        /* 0x08 */ u32 mBuf;
        /* 0x0C */ u32 field_0xc;
        /* 0x10 */ JKRHeap *mHeap;
        /* 0x14 */ u32 mId;

    public:
        u32 getUsedSize() const { return mUsedSize; }
        u32 getCheckCode() const { return mCheckCode; }
        JKRHeap *getHeap() const { return mHeap; }
        u32 getId() const { return mId; }
    };

public:
    JKRHeap(void *, u32, JKRHeap *, bool);

    virtual ~JKRHeap();
    virtual void callAllDisposer();
    virtual u32 getHeapType() ;
    virtual bool check()/* = 0 */ ;
    virtual bool dump_sort();
    virtual bool dump()/* = 0 */ ;
    virtual void do_destroy()/* = 0 */ ;
    virtual void *do_alloc(u32, int)/* = 0 */ ;
    virtual void do_free(void *)/* = 0 */ ;
    virtual void do_freeAll()/* = 0 */ ;
    virtual void do_freeTail()/* = 0 */ ;
    virtual void do_fillFreeArea()/* = 0 */ ;
    virtual s32 do_resize(void *, u32)/* = 0 */ ;
    virtual s32 do_getSize(void *)/* = 0 */ ;
    virtual s32 do_getFreeSize()/* = 0 */ ;
    virtual void *do_getMaxFreeBlock()/* = 0 */ ;
    virtual s32 do_getTotalFreeSize()/* = 0 */ ;
    virtual s32 do_changeGroupID(u8);
    virtual u8 do_getCurrentGroupId();
    virtual void state_register(JKRHeap::TState *, u32) const;
    virtual bool state_compare(JKRHeap::TState const &, JKRHeap::TState const &) const;
    virtual void state_dump(JKRHeap::TState const &) const;

    u32 initArena(char **, u32 *, s32);
    JKRHeap* becomeSystemHeap();
    JKRHeap* becomeCurrentHeap();
    void destroy();
    static void *alloc(u32, int, JKRHeap *);
    void *alloc(u32, int);
    static void free(void *, JKRHeap *);
    void free(void *);
    void freeAll();
    void freeTail();
    void fillFreeArea();
    void resize(void *, u32);

    // ... more functions

    s32 getSize(void *ptr);
    s32 getFreeSize();
    void *getMaxFreeBlock();
    s32 getTotalFreeSize();
    s32 changeGroupID(u8 newGroupId);
    u32 getMaxAllocatableSize(int alignment);

    static JKRHeap* findFromRoot(void *);
    static void copyMemory(void *dest, void *src, u32 len);
    static void* setErrorHandler(void(*)(void *, u32, s32));

    void setDebugFill(bool debugFill) { mDebugFill = debugFill; }
    bool getDebugFill() const { return mDebugFill; }
    void* getStartAddr() const { return (void*)mStart; }
    void* getEndAddr() const { return (void*)mEnd; }
    u32 getSize() const { return mSize; }
    bool getErrorFlag() const { return mErrorFlag; }
    void callErrorHandler(JKRHeap *heap, u32 size, int alignment) {
        if (mErrorHandler)        {
            (*mErrorHandler)(heap, size, alignment);
        }
    }

    static void * getCodeStart() {
        return mCodeStart;
    }

    static void *getCodeEnd() {
        return mCodeEnd;
    }

    static void *getUserRamStart() {
        return mUserRamStart;
    }

    static void *getUserRamEnd() {
        return mUserRamEnd;
    }

    static u32 getMemorySize() {
        return mMemorySize;
    }

    static JKRHeap * getCurrentHeap() {
        return sCurrentHeap;
    }

    static JKRHeap *getRootHeap()    {
        return sRootHeap;
    }

    static JKRHeap *getSystemHeap() {
        return sSystemHeap;
    }

    static void* mCodeStart;
    static void* mCodeEnd;
    static void* mUserRamStart;
    static void* mUserRamEnd;
    static u32 mMemorySize;

    static JKRHeap* sSystemHeap;
    static JKRHeap* sCurrentHeap;
    static JKRHeap* sRootHeap;
    static void* sErrorHandler;

    static JKRErrorHandler mErrorHandler;

protected:
    /* 0x00 */ // vtable
    /* 0x04 */ // JKRDisposer
    /* 0x18 */ OSMutex mMutex;
    /* 0x30 */ u8 *mStart;
    /* 0x34 */ u8 *mEnd;
    /* 0x38 */ u32 mSize;
    /* 0x3C */ bool mDebugFill;
    /* 0x3D */ bool mCheckMemoryFilled;
    /* 0x3E */ u8 mAllocationMode; // EAllocMode?
    /* 0x3F */ u8 mGroupId;
    /* 0x40 */ JSUTree<JKRHeap> mChildTree;
    /* 0x5C */ JSUList<JKRDisposer> mDisposerList;
    /* 0x68 */ bool mErrorFlag;
    /* 0x69 */ bool mInitFlag;
    /* 0x6A */ u8 padding_0x6a[2];
};

inline JKRHeap * JKRGetCurrentHeap() {
    return JKRHeap::getCurrentHeap();
}

inline void *JKRAllocFromSysHeap(u32 size, int alignment) {
    JKRHeap *systemHeap = JKRHeap::sSystemHeap;
    return systemHeap->alloc(size, alignment);
}

inline void * JKRAllocFromHeap(JKRHeap * heap, u32 size, int alignment) {
    return JKRHeap::alloc(size, alignment, heap);
}

inline void JKRFree(void * pBuf) {
    JKRHeap::free(pBuf, nullptr);
}

void JKRDefaultMemoryErrorRoutine(void *, u32, s32);

void* operator new(size_t);
void* operator new(size_t, s32);
void* operator new(size_t, JKRHeap*, int);

void* operator new[](size_t);
void* operator new[](size_t, s32);
void* operator new[](size_t, JKRHeap*, int);

void operator delete(void*);
void operator delete[](void*);

#endif // !JKRHEAP_H
