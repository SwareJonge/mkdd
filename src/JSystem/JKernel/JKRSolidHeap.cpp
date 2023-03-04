#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JUtility/JUTConsole.h>
#include <JSystem/JUtility/JUTDbg.h>

extern "C" {
    #include <ppcdis.h>
}

JKRSolidHeap *JKRSolidHeap::create(u32 size, JKRHeap *heap, bool useErrorHandler)
{
    if (!heap)
    {
        heap = getRootHeap();
    }

    if (size == 0xffffffff)
    {
        size = heap->getMaxAllocatableSize(0x10);
    }

    u32 alignedSize = ALIGN_PREV(size, 0x10);
    u32 solidHeapSize = ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
    if (alignedSize < solidHeapSize)
        return nullptr;

    JKRSolidHeap *solidHeap = (JKRSolidHeap *)JKRAllocFromHeap(heap, alignedSize, 0x10);
    void *dataPtr = (u8 *)solidHeap + solidHeapSize;
    if (!solidHeap)
        return nullptr;

    solidHeap =
        new (solidHeap) JKRSolidHeap(dataPtr, alignedSize - solidHeapSize, heap, useErrorHandler);
    return solidHeap;
}

void JKRSolidHeap::do_destroy()
{
    JKRHeap *parent = getParent();
    if (parent)
    {
        this->~JKRSolidHeap();
        JKRFreeToHeap(parent, this);
    }
}

JKRSolidHeap::JKRSolidHeap(void *start, u32 size, JKRHeap *parent, bool useErrorHandler)
    : JKRHeap(start, size, parent, useErrorHandler)
{
    mFreeSize = mHeapSize;
    mSolidHead = (u8 *)mStartAddress;
    mSolidTail = (u8 *)mEndAddress;
    _78 = nullptr;
}

JKRSolidHeap::~JKRSolidHeap()
{
    dispose();
}

s32 JKRSolidHeap::adjustSize()
{
    JKRHeap *parent = getParent();
    if (parent)
    {
        lock();
        u32 thisSize = (u32)mStartAddress - (u32)this;
        u32 newSize = ALIGN_NEXT(mSolidHead - mStartAddress, 0x20);
        if (parent->resize(this, thisSize + newSize) != -1)
        {
            mFreeSize = 0;
            mHeapSize = newSize;
            mEndAddress = (u8*)mStartAddress + mHeapSize;
            mSolidHead = (u8 *)mEndAddress;
            mSolidTail = (u8 *)mEndAddress;
        }

        unlock();

        return thisSize + newSize;
    }

    return -1;
}

// Not matching for PAL
void *JKRSolidHeap::do_alloc(u32 size, int alignment)
{
    lock();

    if (size < 4)
    {
        size = 4;
    }

    void *ptr;
    if (alignment >= 0)
    {
        ptr = allocFromHead(size, alignment < 4 ? 4 : alignment);
    }
    else
    {
        if (-alignment < 4)
        {
            alignment = 4;
        }
        else
        {
            alignment = -alignment;
        }

        ptr = allocFromTail(size, alignment);
    }

    unlock();
    return ptr;
}

void *JKRSolidHeap::allocFromHead(u32 size, int alignment)
{
    size = ALIGN_NEXT(size, 0x4);
    void *ptr = NULL;
    u32 alignedStart = (alignment - 1 + (u32)mSolidHead) & ~(alignment - 1);
    u32 offset = alignedStart - (u32)mSolidHead;
    u32 totalSize = size + offset;
    if (totalSize <= mFreeSize)
    {
        ptr = (void *)alignedStart;
        mSolidHead += totalSize;
        mFreeSize -= totalSize;
    }
    else
    {
        JUTWarningConsole_f("allocFromHead: cannot alloc memory (0x%x byte).\n", totalSize);
        if (getErrorFlag() == true)
        {
            callErrorHandler(this, size, alignment);
        }
    }

    return ptr;
}

void *JKRSolidHeap::allocFromTail(u32 size, int alignment)
{
    size = ALIGN_NEXT(size, 4);
    void *ptr = NULL;
    u32 alignedStart = ALIGN_PREV((u32)mSolidTail - size, alignment);
    u32 totalSize = (u32)mSolidTail - (u32)alignedStart;
    if (totalSize <= mFreeSize)
    {
        ptr = (void *)alignedStart;
        mSolidTail -= totalSize;
        mFreeSize -= totalSize;
    }
    else
    {
        JUTWarningConsole_f("allocFromTail: cannot alloc memory (0x%x byte).\n", totalSize);
        if (getErrorFlag() == true)
        {
            callErrorHandler(this, size, alignment);
        }
    }
    return ptr;
}

void JKRSolidHeap::do_free(void *ptr)
{

}

void JKRSolidHeap::do_freeAll(void)
{
    lock();

    JKRHeap::callAllDisposer();
    mFreeSize = mHeapSize;
    mSolidHead = (u8 *)mStartAddress;
    mSolidTail = (u8 *)mEndAddress;
    _78 = nullptr;

    unlock();
}

void JKRSolidHeap::do_freeTail(void)
{
    lock();

    if (mSolidTail != mEndAddress)
    {
        dispose(mSolidTail, mEndAddress);
    }

    mFreeSize = ((u32)mEndAddress - (u32)mSolidTail + mFreeSize);
    mSolidTail = (u8*)mEndAddress;

    State *unk = _78;
    while (unk)
    {
        unk->field_0xc = mEndAddress;
        unk = unk->mNext;
    }

    unlock();
}

void JKRSolidHeap::do_fillFreeArea() {}

int JKRSolidHeap::do_resize(void *ptr, u32 newSize)
{
    JUTWarningConsole_f("resize: cannot resize memory block (%08x: %d)\n", ptr, newSize);
    return -1;
}

int JKRSolidHeap::do_getSize(void *ptr)
{
    JUTWarningConsole_f("getSize: cannot get memory block size (%08x)\n", ptr);
    return -1;
}

MANGLED_ASM(void g(u32 cnt)){
#include "asm/800862cc.s"
} 
MANGLED_ASM(void JKRSolidHeap::restoreState(u32 cnt))
{
#include "asm/80086348.s"
}
/*
void JKRSolidHeap::recordState(u32 cnt)
{
    lock();
    u32 freeSize = mFreeSize;
    u8 *head = mSolidHead;
    u8 *tail = mSolidTail;
    State *next = _78;
    u8 *mem = (u8 *)alloc(sizeof(State), 4);
    _78 = new (mem) State(cnt, freeSize, head, tail, next); // _78 is inside this Ctor?

    unlock();
}
// currently not really bothered to work on this, effort
void JKRSolidHeap::restoreState(u32 cnt) {
    
    lock();


    unlock();
}
*/

bool JKRSolidHeap::check()
{
    lock();

    bool result = true;
    u32 calculatedSize =
        ((u32)mSolidHead - (u32)mStartAddress) + mFreeSize + ((u32)mEndAddress - (u32)mSolidTail);
    u32 availableSize = mHeapSize;
    if (calculatedSize != availableSize)
    {
        result = false;
        JUTWarningConsole_f("check: bad total memory block size (%08X, %08X)\n", availableSize,
                            calculatedSize);
    }

    unlock();
    return result;
}

bool JKRSolidHeap::dump(void)
{
    bool result = check();

    lock();
    u32 headSize = ((u32)mSolidHead - (u32)mStartAddress);
    u32 tailSize = ((u32)mEndAddress - (u32)mSolidTail);
    s32 htSize = headSize + tailSize;
    JUTReportConsole_f("head %08x: %08x\n", mStartAddress, headSize);
    JUTReportConsole_f("tail %08x: %08x\n", mSolidTail, ((u32)mEndAddress - (u32)mSolidTail));

    u32 totalSize = mHeapSize;
    float percentage = (float)htSize / (float)totalSize * 100.0f;
    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used\n", htSize, totalSize, percentage);
    unlock();

    return result;
}

void JKRSolidHeap::state_register(JKRHeap::TState *p, u32 id) const
{
    JUT_ASSERT(603, p != 0);
    JUT_ASSERT(604, p->getHeap() == this);

    getState_(p);
    setState_u32ID_(p, id);
    setState_uUsedSize_(p, getUsedSize((JKRSolidHeap *)this));
    u32 sum = (u32)mSolidHead;
    sum += (u32)mSolidTail * 3;
    setState_u32CheckCode_(p, sum);
}

bool JKRSolidHeap::state_compare(JKRHeap::TState const &r1, JKRHeap::TState const &r2) const
{
    JUT_ASSERT(631, r1.getHeap() == r2.getHeap());

    bool result = true;
    if (r1.getCheckCode() != r2.getCheckCode())
    {
        result = false;
    }

    if (r1.getUsedSize() != r2.getUsedSize())
    {
        result = false;
    }

    return result;
}