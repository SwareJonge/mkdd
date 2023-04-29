#include <JSystem/JKernel/JKRAssertHeap.h>

JKRAssertHeap::JKRAssertHeap(void *data, unsigned long size, JKRHeap *parent, bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag) {}

JKRAssertHeap::~JKRAssertHeap() { this->dispose(); }

JKRAssertHeap *JKRAssertHeap::create(JKRHeap *parent)
{
    if (!parent)
        parent = sRootHeap;

    // 0x70 is sizeof(JKRAssertHeap) aligned to 16 bytes
    // casting is need for an extra mr
    u8 *ptr = (u8 *)JKRAllocFromHeap(parent, 0x70, 16);
    if (!ptr)
        return nullptr;

    return new (ptr) JKRAssertHeap(nullptr, 0, parent, false);
}

void JKRAssertHeap::do_destroy() { this->~JKRAssertHeap(); }