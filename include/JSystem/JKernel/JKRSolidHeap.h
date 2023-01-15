#ifndef JKRSOLIDHEAP_H
#define JKRSOLIDHEAP_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class JKRSolidHeap : public JKRHeap
{
public:
    JKRSolidHeap(void *, u32, JKRHeap *, bool);

    virtual ~JKRSolidHeap();                                          // _08
    virtual u32 getHeapType() { return 'SLID'; }                      // _10 (weak)
    virtual bool check();                                             // _14
    virtual bool dump();                                              // _1C
    virtual void do_destroy();                                        // _20
    virtual void *do_alloc(u32, int);                                 // _24
    virtual void do_free(void *);                                     // _28
    virtual void do_freeAll();                                        // _2C
    virtual void do_freeTail();                                       // _30
    virtual void do_fillFreeArea();                                   // _34
    virtual int do_resize(void *, u32);                               // _38
    virtual int do_getSize(void *);                                   // _3C
    virtual u32 do_getFreeSize() { return m_freeSize; }               // _40 (weak)
    virtual void *do_getMaxFreeBlock() { return (void *)_70; }        // _44 (weak)
    virtual u32 do_getTotalFreeSize() { return getFreeSize(); }       // _48 (weak)
    virtual void state_register(TState *, u32) const;                 // _54
    virtual bool state_compare(const TState &, const TState &) const; // _58

    u32 adjustSize();
    void *allocFromHead(u32, int);
    void *allocFromTail(u32, int);

    static JKRSolidHeap *create(u32, JKRHeap *, bool);

private:
    u32 m_freeSize; // _6C
    u32 _70;        // _70
    u32 _74;        // _74
    u32 _78;        // _78
};

inline JKRSolidHeap *JKRCreateSolidHeap(size_t size, JKRHeap *heap, bool p3) {
    return JKRSolidHeap::create(size, heap, p3);
}

#endif