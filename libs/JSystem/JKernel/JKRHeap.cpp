#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRHeap *JKRHeap::sSystemHeap;
JKRHeap *JKRHeap::sCurrentHeap;
JKRHeap *JKRHeap::sRootHeap;
JKRHeapErrorHandler *JKRHeap::mErrorHandler;
bool JKRHeap::sDefaultFillCheckFlag;
void *JKRHeap::mCodeStart;
void *JKRHeap::mCodeEnd;
void *JKRHeap::mUserRamStart;
void *JKRHeap::mUserRamEnd;
u32 JKRHeap::mMemorySize;
bool JKRHeap::TState::bVerbose_;

bool JKRHeap::sDefaultFillFlag = true;

JKRHeap::JKRHeap(void *data, u32 size, JKRHeap *heap, bool errorFlag) : JKRDisposer(),
                                                                        mHeapTree(this),
                                                                        mDisposerList()
{
    OSInitMutex(&mMutex);
    mHeapSize = size;
    mStartAddress = (u8*)data;
    mEndAddress = ((u8*)data + size);
    if(heap == nullptr) {
        becomeSystemHeap();
        becomeCurrentHeap();
    }
    else {
        heap->mHeapTree.appendChild(&mHeapTree);
        if(sSystemHeap == sRootHeap)
            becomeSystemHeap();
        if(sCurrentHeap == sRootHeap)
            becomeCurrentHeap();
    }
    mErrorFlag = errorFlag;
    if (mErrorFlag == true && mErrorHandler == nullptr)
    mErrorHandler = JKRDefaultMemoryErrorRoutine;
    mIsDebugFill = sDefaultFillFlag;
    mFillCheckFlag = sDefaultFillCheckFlag;
    mInitFlag = false;
}

JKRHeap::~JKRHeap()
{
    mHeapTree.getParent()->removeChild(&mHeapTree);
    JSUTree<JKRHeap> *nextRootHeap = sRootHeap->mHeapTree.getFirstChild();
    if (sCurrentHeap == this)
        sCurrentHeap = !nextRootHeap ? sRootHeap : nextRootHeap->getObject();

    if (sSystemHeap == this)
        sSystemHeap = !nextRootHeap ? sRootHeap : nextRootHeap->getObject();
}

bool JKRHeap::initArena(char **outUserRamStart, u32 *outUserRamSize, int numHeaps)
{
    void *arenaLo = OSGetArenaLo();
    void *arenaHi = OSGetArenaHi();
    if (arenaLo == arenaHi)
    {
        return false;
    }
    void *arenaStart = OSInitAlloc(arenaLo, arenaHi, numHeaps);
    arenaHi = (u8 *)OSRoundDown32B(arenaHi);
    arenaLo = (u8 *)OSRoundUp32B(arenaStart);
    u8 *start = (u8 *)OSPhysicalToCached(0);
    mCodeStart = (u8 *)start;
    mCodeEnd = (u8 *)arenaLo;
    mUserRamStart = (u8 *)arenaLo;
    mUserRamEnd = (u8 *)arenaHi;
    mMemorySize = *(u32 *)((start + 0x28));
    OSSetArenaLo(arenaHi);
    OSSetArenaHi(arenaHi);
    *outUserRamStart = (char *)arenaLo;
    *outUserRamSize = (u32)arenaHi - (u32)arenaLo;
    return true;
}

JKRHeap *JKRHeap::becomeSystemHeap()
{
    JKRHeap *old = sSystemHeap;
    sSystemHeap = this;
    return old;
}

JKRHeap *JKRHeap::becomeCurrentHeap()
{
    JKRHeap *old = sCurrentHeap;
    sCurrentHeap = this;
    return old;
}

void JKRHeap::destroy(JKRHeap * heap) {
    JUT_ASSERT(200, heap != 0);
    heap->destroy();
}

void JKRHeap::destroy() { do_destroy(); }

void *JKRHeap::alloc(u32 byteCount, int padding, JKRHeap *heap)
{
    void *memory;
    if (heap)
    {
        return heap->alloc(byteCount, padding);
    }
    else if (sCurrentHeap)
    {
        return sCurrentHeap->alloc(byteCount, padding);
    }
    return nullptr;
}

void *JKRHeap::alloc(u32 byteCount, int padding) {
    JUT_WARNING_F(317, !mInitFlag, "alloc %x byte in heap %x", byteCount, this);
    return do_alloc(byteCount, padding); 
}

void JKRHeap::free(void *memory, JKRHeap *heap)
{
    if ((heap) || (heap = findFromRoot(memory), heap))
    {
        heap->free(memory);
    }
}

void JKRHeap::free(void *memory) {
    JUT_WARNING_F(365, !mInitFlag, "free %x in heap %x", memory, this);
    do_free(memory); 
}

void JKRHeap::callAllDisposer()
{
    JSUListIterator<JKRDisposer> iterator;
    while (iterator = mDisposerList.getFirst(), iterator != mDisposerList.getEnd())
    {
        iterator->~JKRDisposer();
    }
}

void JKRHeap::freeAll() {
    JUT_WARNING_F(417, !mInitFlag, "freeAll in heap %x", this);
    do_freeAll(); 
}

void JKRHeap::freeTail() {
    JUT_WARNING_F(431, !mInitFlag, "freeTail in heap %x", this);
    do_freeTail(); 
}

// fabricated
void JKRHeap::fillFreeArea() {
    JUT_WARNING_F(461, !mInitFlag, "fillFreeArea in heap %x", this);
    do_fillFreeArea();
}

int JKRHeap::resize(void *memoryBlock, u32 newSize) {
    JUT_WARNING_F(491, !mInitFlag, "resize block %x into %x in heap %x", memoryBlock, newSize, this);
    return do_resize(memoryBlock, newSize); 
}
s32 JKRHeap::getFreeSize() { return do_getFreeSize(); }
u32 JKRHeap::getTotalFreeSize() { return do_getTotalFreeSize(); }

u8 JKRHeap::changeGroupID(u8 newGroupID) {
    JUT_WARNING_F(570, !mInitFlag, "change heap ID into %x in heap %x", newGroupID, this);
    return do_changeGroupID(newGroupID); 
}

u8 JKRHeap::getCurrentGroupId() { return do_getCurrentGroupId(); }

u32 JKRHeap::getMaxAllocatableSize(int p1)
{
    u32 maxFreeBlock = (u32)do_getMaxFreeBlock();
    u32 freeSize = do_getFreeSize();
    return ~(p1 - 1) & freeSize - (p1 - 1 & p1 - ((u32)maxFreeBlock & 0xf));
}

JKRHeap *JKRHeap::findFromRoot(void *ptr)
{
    if (sRootHeap == nullptr)
    {
        return nullptr;
    }

    if (sRootHeap->mStartAddress <= ptr && ptr < sRootHeap->mEndAddress)
    {
        return sRootHeap->find(ptr);
    }

    return sRootHeap->findAllHeap(ptr);
}

JKRHeap* JKRHeap::find(void* memory) const
{
	if ((mStartAddress <= memory) && (memory < mEndAddress)) {
		if (mHeapTree.getNumChildren() != 0) {
            for (JSUTreeIterator<JKRHeap> iterator(mHeapTree.getFirstChild()); iterator != mHeapTree.getEndChild(); ++iterator)
            {
                JKRHeap* result = iterator->find(memory);
				if (result) {
					return result;
				}
            }
        }
		return const_cast<JKRHeap*>(this);
	}
	return nullptr;
}

JKRHeap *JKRHeap::findAllHeap(void *memory) const
{
    if (mHeapTree.getNumChildren() != 0)
    {
        for (JSUTreeIterator<JKRHeap> iterator(mHeapTree.getFirstChild()); iterator != mHeapTree.getEndChild(); ++iterator)
        {
            JKRHeap *result = iterator->findAllHeap(memory);
            if (result)
            {
                return result;
            }
        }
    }

    if (mStartAddress <= memory && memory < mEndAddress)
    {
        return const_cast<JKRHeap *>(this);
    }

    return nullptr;
}

// generates __as__25JSUTreeIterator<7JKRHeap>FP17JSUTree<7JKRHeap> and __ct__25JSUTreeIterator<7JKRHeap>Fv, remove this
void JKRHeap::dispose_subroutine(u32 begin, u32 end)
{
    JSUListIterator<JKRDisposer> last_iterator;
    JSUListIterator<JKRDisposer> next_iterator;
    JSUListIterator<JKRDisposer> iterator;
    for (iterator = mDisposerList.getFirst(); iterator != mDisposerList.getEnd();
         iterator = next_iterator)
    {
        JKRDisposer *disposer = iterator.getObject();

        if ((void *)begin <= disposer && disposer < (void *)end)
        {
            disposer->~JKRDisposer();
            if (last_iterator == nullptr)
            {
                next_iterator = mDisposerList.getFirst();
            }
            else
            {
                next_iterator = last_iterator;
                next_iterator++;
            }
        }
        else
        {
            last_iterator = iterator;
            next_iterator = iterator;
            next_iterator++;
        }
    }
}

bool JKRHeap::dispose(void *memory, u32 size)
{
    u32 begin = (u32)memory;
    u32 end = (u32)memory + size;
    dispose_subroutine(begin, end);
    return false;
}

void JKRHeap::dispose(void *begin, void *end)
{
    dispose_subroutine((u32)begin, (u32)end);
}

void JKRHeap::dispose()
{
    JSUListIterator<JKRDisposer> iterator;
    while (iterator = mDisposerList.getFirst(), iterator != mDisposerList.getEnd())
    {
        iterator->~JKRDisposer();
    }
}

void JKRHeap::copyMemory(void *dst, void *src, u32 size)
{
    u32 count = (size + 3) / 4;

    u32 *dst_32 = (u32 *)dst;
    u32 *src_32 = (u32 *)src;
    while (count > 0)
    {
        *dst_32 = *src_32;
        dst_32++;
        src_32++;
        count--;
    }
}

void JKRDefaultMemoryErrorRoutine(void * heap, u32 size, int alignment) {
    JUT_REPORT_MSG("Error: Cannot allocate memory %d(0x%x)byte in %d byte alignment from %08x\n", size, size, alignment, heap);
    JUT_PANIC(831, "abort\n");
}

// attempt to reconstruct function but lazy
void JKRHeap::checkMemoryFilled(u8 * address, u32 size, u8 p3)
{
    for (int i = 0; i < size; i++) {
        JUT_WARNING_F(999, p3 == address[i], "**** checkMemoryFilled:\n address %08x size %x:\n (%08x = %02x)\n", address, size, address[i], address[i]);
    }
}

JKRHeapErrorHandler *JKRHeap::setErrorHandler(JKRHeapErrorHandler *newHandler)
{
    JKRHeapErrorHandler *oldHandler = mErrorHandler;
    if (!newHandler)
    {
        newHandler = JKRDefaultMemoryErrorRoutine;
    }
    mErrorHandler = newHandler;
    return oldHandler;
}

bool JKRHeap::isSubHeap(JKRHeap *heap) const
{
    if (!heap)
        return false;

    if (mHeapTree.getNumChildren() != 0)
    {
        JSUTreeIterator<JKRHeap> iterator;
        for (iterator = mHeapTree.getFirstChild(); iterator != mHeapTree.getEndChild(); ++iterator)
        {
            if (iterator.getObject() == heap)
            {
                return true;
            }

            if (iterator.getObject()->isSubHeap(heap))
            {
                return true;
            }
        }
    }

    return false;
}

void *operator new(u32 byteCount) { 
    return JKRHeap::alloc(byteCount, 4, nullptr); 
}
void *operator new(u32 byteCount, int alignment) {
    return JKRHeap::alloc(byteCount, alignment, nullptr);
}
void *operator new(u32 byteCount, JKRHeap *heap, int alignment){
    return JKRHeap::alloc(byteCount, alignment, heap);
}

void *operator new[](u32 byteCount) { 
    return JKRHeap::alloc(byteCount, 4, nullptr); 
}
void *operator new[](u32 byteCount, int alignment) {
    return JKRHeap::alloc(byteCount, alignment, nullptr);
}
void *operator new[](u32 byteCount, JKRHeap *heap, int alignment)
{
    return JKRHeap::alloc(byteCount, alignment, heap);
}

// this is not needed without the other pragma and asm bs
void operator delete(void *memory) { JKRHeap::free(memory, nullptr); }
void operator delete[](void *memory) { JKRHeap::free(memory, nullptr); }

/*JKRHeap::TState::TState(const JKRHeap::TState::TArgument &arg, const JKRHeap::TState::TLocation &location)
{
    // UNUSED FUNCTION
}

JKRHeap::TState::TState(const JKRHeap::TState &other, bool p2)
{
    // UNUSED FUNCTION
}

JKRHeap::TState::TState(const JKRHeap::TState &other, const JKRHeap::TState::TLocation &location, bool p3)
{
    // UNUSED FUNCTION
}*/

// not sure where these lines originally were, i think in the destructor of TState but not many games have that
#if DEBUG
static void genData()
{
    JUT_LOG_F(1000, "heap unchanged");
    JUT_LOG_F(1001, "**** heap changed ****");
    JUT_LOG_F(1002, "location   : [%s:%d]");
    JUT_LOG_F(1003, "**** heap changed : old ****");
    JUT_LOG_F(1004, "**** heap changed : new ****");
}
#endif

JKRHeap::TState::~TState() 
{
    // Unused, however might need it
}

void JKRHeap::state_register(JKRHeap::TState * p, u32) const {
    JUT_ASSERT(1132, p != 0);
    JUT_ASSERT(1133, p->getHeap() == this);
}

bool JKRHeap::state_compare(const JKRHeap::TState &r1, const JKRHeap::TState &r2) const {
    JUT_ASSERT(1141, r1.getHeap() == r2.getHeap());
    return (r1.getCheckCode() == r2.getCheckCode());
}

// fabricated, but probably matches(except for line numbers)
void JKRHeap::state_dumpDifference(const JKRHeap::TState & r1, const JKRHeap::TState & r2) {
    JUT_LOG_F(1157, "heap       : %p / %p", r1.getHeap(), r2.getHeap());
    JUT_LOG_F(1158, "check-code : 0x%08x / 0x%08x", r1.getCheckCode(), r2.getCheckCode());
    JUT_LOG_F(1159, "id         : 0x%08x / 0x%08x", r1.getId(), r2.getId());
    JUT_LOG_F(1160, "used size  : %10u / %10u", r1.getUsedSize(), r2.getUsedSize());
}

void JKRHeap::state_dump(const TState & state) const {
    JUT_LOG_F(1165, "check-code : 0x%08x", state.getCheckCode());
    JUT_LOG_F(1166, "id         : 0x%08x", state.getId());
    JUT_LOG_F(1167, "used size  : %u", state.getUsedSize());
}
