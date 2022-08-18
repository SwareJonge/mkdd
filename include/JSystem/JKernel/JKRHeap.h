#ifndef JKRHEAP_H
#define JKRHEAP_H

#include "types.h"
#include "JSystem/JKernel/JKRDisposer.h"

class JKRHeap : public JKRDisposer {
public:
    JKRHeap(void *, u32, JKRHeap *, bool);

    virtual ~JKRHeap();
    virtual void callAllDisposer();
    virtual s32 getHeapType();
    virtual void check();
    virtual s32 dump_sort();
    virtual JKRHeap* dump();
    virtual void do_destroy();
    virtual void* do_alloc(u32, s32);
    virtual void do_free(void *);
    virtual void do_freeAll();
    virtual void do_freeTail();
    virtual void do_fillFreeArea();
    virtual s32 do_resize(void *, u32);
    virtual s32 do_getSize(void *);
    virtual s32 do_getFreeSize();
    virtual s32 do_getMaxFreeBlock();
    virtual s32 do_getTotalFreeSize();

    u32 initArena(char **, u32 *, s32);
    JKRHeap* becomeSystemHeap();
    JKRHeap* becomeCurrentHeap();
    void destroy(JKRHeap *);
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
