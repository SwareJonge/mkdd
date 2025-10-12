#ifndef STD_MEMORY_H
#define STD_MEMORY_H

#include "JSystem/JUtility/JUTAssert.h"

namespace JGadget {
template <typename T>
struct TAllocator {
    T* allocate(u32 count, void *param_2) {
        return AllocateRaw(count * sizeof(T));
    }

    T* AllocateRaw(u32 size) {
        return (T*)operator new(size);
    }

    void deallocate(T* mem, u32 size) {
        DeallocateRaw(mem);
    }

    void DeallocateRaw(void* mem) {
        delete mem;
    }

    void construct(T* p, const T& value)
	{
		// clang-format off
		JGADGET_ASSERT(p!=0);
		// clang-format on
		(void)::new (p) T(value);
	}

    void destroy(T* p) {
        // clang-format off
		JGADGET_ASSERT(p!=0);
		// clang-format on
		p->~T();
    }

    u8 mAllocator; // 00
};
}

#endif /* STD_MEMORY_H */
