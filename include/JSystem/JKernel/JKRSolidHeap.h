#ifndef JSYSTEM_JKERNEL_JKRSOLIDHEAP_H
#define JSYSTEM_JKERNEL_JKRSOLIDHEAP_H

#include "JSystem/JKernel/JKRHeap.h"

class JKRSolidHeap : public JKRHeap
{
public:
    struct State
    {
        State(State *other, u32 cnt)
        {
            mCnt = cnt;
            mSize = other->mSize;
            _08 = other->_08;
            _0C = other->_0C;
            mNext = other;
            other = this;
        }
        State(u32 cnt, u32 size, u8 *p3, u8 *p4, State *next)
        {
            mCnt = cnt;
            mSize = size;
            _08 = p3;
            _0C = p4;
            mNext = next;
        }
        u32 mCnt;
        u32 mSize;
        u8 *_08;
        void *_0C;
        State *mNext;
    };

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
    virtual u32 do_getFreeSize() { return mFreeSize; }                // _40 (weak)
    virtual void *do_getMaxFreeBlock() { return (void *)mSolidHead; } // _44 (weak)
    virtual u32 do_getTotalFreeSize() { return getFreeSize(); }       // _48 (weak)
    virtual void state_register(TState *, u32) const;                 // _54
    virtual bool state_compare(const TState &, const TState &) const; // _58

    s32 adjustSize();
    void recordState(u32);
    void restoreState(u32);
    void *allocFromHead(u32, int);
    void *allocFromTail(u32, int);

    static JKRSolidHeap *create(u32, JKRHeap *, bool);

    // TState stupidness
    static u32 getUsedSize(JKRSolidHeap *sldHeap)
    {
        u32 totalFreeSize = sldHeap->getTotalFreeSize();
        return sldHeap->mHeapSize - totalFreeSize;
    }
    static u32 getState_(TState *state) { return getState_buf_(state); } // might instead be a pointer to a next state?

    // _00     = VTBL
    // _00-_6C = JKRHeap
private:
    u32 mFreeSize;  // _6C
    u8 *mSolidHead; // _70
    u8 *mSolidTail; // _74
    State *_78;     // _78, seems to be some linked list struct in TP
};

inline JKRSolidHeap *JKRCreateSolidHeap(size_t size, JKRHeap *heap, bool p3) {
    return JKRSolidHeap::create(size, heap, p3);
}


#endif