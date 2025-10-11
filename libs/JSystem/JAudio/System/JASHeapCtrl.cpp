#include "JSystem/JAudio/System/JASHeap.h"

#include "JSystem/JAudio/System/JASDisposer.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASKernel.h"
#include "JSystem/JAudio/System/JASMutex.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/ar/ar.h"
#include "dolphin/os.h"

JKRHeap *JASDram;

JASHeap::JASHeap(JASDisposer *disposer) : mTree(this) {
    mDisposer = disposer;
    mBase = NULL;
    mSize = 0;
    _40 = NULL;
    OSInitMutex(&mMutex);
}

void JASHeap::initRootHeap(void *base, u32 size) {
#line 97
    JUT_ASSERT(! isAllocated());
    JASMutexLock lock(&mMutex);
    mBase = (u8*)OSRoundUp32B(base);
    _40 = NULL;
    mSize = size - (u32(mBase) - u32(base));
}

bool JASHeap::alloc(JASHeap *mother, u32 param_1) {
#line 120
    JUT_ASSERT(mother != 0);
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        JUT_REPORT_MSG("[JASHeap::alloc] すでにヒープは確保されています。初期化してからにしてください。\n");
        return 0;
    }
    if (!mother->isAllocated()) {
        return 0;
    }
    param_1 = OSRoundUp32B(param_1);
    u32 local_28 = mother->getCurOffset();
    u32 local_2c = mother->getTailOffset();
    if (local_28 + param_1 <= local_2c) {
        mother->insertChild(this, mother->getTailHeap(), mother->mBase + local_28, param_1, false);
        return 1;
    }
    s32 r27 = -1;
    u8 *r29 = mother->mBase;
    bool local_43 = false;
    JASHeap *local_30 = NULL;
    void *local_34;
    for (JSUTreeIterator<JASHeap> it = mother->mTree.getFirstChild(); it != mother->mTree.getEndChild(); it++) {
        if (r29 >= mother->mBase + local_2c) {
            break;
        }
        u32 local_3c = u32(it->mBase) - u32(r29);
        if (local_3c >= param_1 && local_3c < r27) {
            local_30 = *it;
            local_34 = r29;
            r27 = local_3c;
            local_43 = true;
        }
        u32 r25 = it->mSize;
        r29 = (u8*)it->mBase + r25;
    }
    if (r29 != mother->mBase && r29 < mother->mBase + local_2c) {
        u32 local_40 = mother->mBase + mother->mSize - r29;
        if (local_40 >= param_1 && local_40 < r27) {
            local_30 = NULL;
            local_34 = r29;
            r27 = local_40;
            local_43 = true;
        }
    }
    if (!local_43) {
#ifdef DEBUG
        OSReport("[JASHeap::alloc] マザーメモリが足りないので確保できません。\n");
#endif
        return 0;
    }
    mother->insertChild(this, local_30, local_34, param_1, false);
    return 1;
}

bool JASHeap::allocTail(JASHeap *mother, u32 size) {
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        return false;
    }
    if (!mother->isAllocated()) {
        return false;
    }
    u32 aligned_size = (size + 0x1f) & ~0x1f;
    u32 cur_offset = mother->getCurOffset();
    u32 tail_offset = mother->getTailOffset();
    if (cur_offset + aligned_size > tail_offset) {
        return false;
    }
    mother->insertChild(this, mother->getTailHeap(), mother->mBase + tail_offset - aligned_size,
                        aligned_size, true);
    return true;
}

bool JASHeap::free() {
    JASMutexLock lock(&mMutex);
    if (!isAllocated()) {
        return false;
    }
    JSUTreeIterator<JASHeap> stack_20;
    for (JSUTreeIterator<JASHeap> it(mTree.getFirstChild()); it != mTree.getEndChild(); it = stack_20) {
        stack_20 = it;
        stack_20++;
        it->free();
    }
    JSUTree<JASHeap> *parentTree = mTree.getParent();
    if (parentTree) {
        JASHeap *parentHeap = parentTree->getObject();
        if (parentHeap->_40 == this) {
            JSUTreeIterator<JASHeap> stack_28(mTree.getPrevChild());
            if (stack_28 != mTree.getEndChild()) {
                parentHeap->_40 = *stack_28;
            } else {
                parentHeap->_40 = NULL;
            }
        }
        parentTree->removeChild(&mTree);
    }
    mBase = NULL;
    _40 = NULL;
    mSize = 0;
    if (mDisposer) {
        mDisposer->onDispose();
    }
    return true;
}

void JASHeap::dump() {
    
}

void JASHeap::dump(int level) {
    // JUTAssertion::showAssert needs to not inline here
    JUT_REPORT_MSG("------------------------------------\n");
    JUT_REPORT_MSG("        ");
    JUT_ASSERT(level >= 0 && level < 7)
    JUT_REPORT_MSG("%s Heap %08x [Addr %8x , Max %8x] %c\n", _40, mBase, mBase + mSize, level)
    // functions emitted:
    // void JSUTree<JASHeap>::getEndChild() const;
    // void JSUTreeIterator<JASHeap>::operator++ ();
    // JSUTreeIterator<JASHeap>::JSUTreeIterator(JSUTree<JASHeap> *);
    // void JSUTree<JASHeap>::getObject() const;
    // void JSUTree<JASHeap>::getParent() const;
    // JASMutexLock::JASMutexLock(OSMutex *);
    // void JUTAssertion::showAssert(unsigned long, const char *, int, const char *)
    // void JSUTreeIterator<JASHeap>::operator-> () const;
    // void JSUTree<JASHeap>::getFirstChild() const;
}
void JASHeap::insertChild(JASHeap *heap, JASHeap *next, void *param_2, u32 param_3, bool param_4) {
#line 513
    JUT_ASSERT(heap != 0);
    JUT_ASSERT(next == 0 || &mTree == next->mTree.getParent());
    JASMutexLock lock(&mMutex);
    if (!param_4) {
        JSUTreeIterator<JASHeap> it;
        if (!next) {
            it = mTree.getLastChild();
        } else {
            it = next->mTree.getPrevChild();
        }
        JASHeap *childHeap = it != mTree.getEndChild() ? it.getObject() : NULL;
        if (_40 == childHeap) {
            _40 = heap;
        }
    }
    heap->mBase = (u8*)param_2;
    heap->mSize = param_3;
    heap->_40 = NULL;
    mTree.insertChild(&next->mTree, &heap->mTree);
}

JASHeap *JASHeap::getTailHeap() {
    JSUTreeIterator<JASHeap> it;
    JASMutexLock lock(&mMutex);
    if (!_40) {
        it = mTree.getFirstChild();
    } else {
        it = _40->mTree.getNextChild();
    }
    if (it == mTree.getEndChild()) {
        return NULL;
    }
    return it.getObject();
}

u32 JASHeap::getTailOffset() {
    JASMutexLock lock(&mMutex);
    JASHeap* heap = getTailHeap();
    u32 offset = !heap ? mSize : heap->mBase - mBase;
    return offset;
}

u32 JASHeap::getCurOffset() {
    JASMutexLock lock(&mMutex);
    u32 offset = !_40 ? 0 : _40->mBase + _40->mSize - mBase;
    return offset;
}

JASSolidHeap::JASSolidHeap() {

}

JASSolidHeap::JASSolidHeap(u8 *, u32) {

}

void JASSolidHeap::init(u8 *, u32) {

}

void JASSolidHeap::alloc(u32) {
    JUT_REPORT_MSG("Not enough Memory")
}

void JASSolidHeap::freeLast() {

}

void JASSolidHeap::freeAll() {

}

u32 JASSolidHeap::getRemain() {

}

JASGenericMemPool::JASGenericMemPool() {
    mRunner = NULL;
    mFreeMemCount = 0;
    mTotalMemCount = 0;
    mUsedMemCount = 0;
}

JASGenericMemPool::~JASGenericMemPool() {
    void *chunk = mRunner;
    while (chunk != NULL) {
        void *next_chunk = *(void**)chunk;
        delete[] chunk;
        chunk = next_chunk;
    }
}

void JASGenericMemPool::newMemPool(u32 n, int count) {
#line 734
    JUT_ASSERT(n >= sizeof(TNextOnFreeList))
    for (int i = 0; i < count; i++) {
#line 738
        void *runner = new (JASDram, 0) u8[n];
        JUT_ASSERT(runner);
        *(void**)runner = mRunner;
        mRunner = runner;
    }
    mFreeMemCount += count;
    mTotalMemCount += count;
}

void *JASGenericMemPool::alloc(u32) {
    void *chunk = mRunner;
    if (chunk == NULL) {
        return NULL;
    }
    mRunner = *(void**)chunk;
    mFreeMemCount--;
    if (mUsedMemCount < mTotalMemCount - mFreeMemCount) {
        mUsedMemCount = mTotalMemCount - mFreeMemCount;
    }
    return chunk;
}

void JASGenericMemPool::free(void *ptr, u32 param_1) {
    if (ptr != NULL) {
        *(void**)ptr = mRunner;
        mRunner = ptr;
        mFreeMemCount++;
    }
}

namespace JASKernel {

u32 sAramBase;
JKRHeap *sSystemHeap;
JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable> *sCommandHeap;
static JASHeap audioAramHeap;

void setupRootHeap(JKRSolidHeap *heap, u32 size) {
#line 784
    JUT_ASSERT(heap);

    sSystemHeap = JKRExpHeap::create(size, heap, false);
    JUT_ASSERT(sSystemHeap);
    
    sCommandHeap = new (heap, 0) JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable>();
    JUT_ASSERT(sCommandHeap);
    JASDram = heap;
}

JKRHeap *getSystemHeap() {
    return sSystemHeap;
}

JASMemChunkPool<1024, JASThreadingModel::ObjectLevelLockable> *getCommandHeap() {
    return sCommandHeap;
}

void setupAramHeap(u32 base, u32 size) {
#line 847
    JUT_ASSERT(base >= ARGetBaseAddress());
    sAramBase = base;
    audioAramHeap.initRootHeap((void*)base, size);
}

JASHeap *getAramHeap() {
    return &audioAramHeap;
}
}
