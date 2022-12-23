#ifndef JKRHEAP_H
#define JKRHEAP_H

#include "types.h"
#include "JSystem/JKernel/JKRDisposer.h"

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
    virtual void *do_alloc(u32, s32)/* = 0 */ ;
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
    static void* alloc(u32, s32, JKRHeap *);
    static void free(void *, JKRHeap *);
    void free(void *);
    void freeAll();
    void freeTail();
    void fillFreeArea();
    void resize(void *, u32);

    // ... more functions

    static JKRHeap* findFromRoot(void *);
    static void copyMemory(void *dest, void *src, u32 len);
    static void* setErrorHandler(void(*)(void *, u32, s32));

    static void* mCodeStart;
    static void* mCodeEnd;
    static void* mUserRamStart;
    static void* mUserRamEnd;
    static s32 mMemorySize;

    static JKRHeap* sSystemHeap;
    static JKRHeap* sCurrentHeap;
    static JKRHeap* sRootHeap;
    static void* sErrorHandler;

private:
    u8 _C[0x5C - 0xC];
    JSUPtrList mPtrList; // _5C

};

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
