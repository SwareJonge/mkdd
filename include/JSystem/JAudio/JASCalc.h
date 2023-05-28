#include "types.h"

// might be a class, who knows
namespace JASCalc {
    void imixcopy(const s16 *s1, const s16 *s2, s16 *dst, u32 size);
    void bcopyfast(const void *src, void *dest, u32 size);
    void bcopy(const void *src, void *dest, u32 size);
    void bzerofast(void *dest, u32 size);
    void bzero(void *dest, u32 size);

    //template <typename A, typename B>
    //A clamp(A, B);
    // TODO: does this create problems for other files? if so try making JASCalc a class or declare this as extern?
    template <typename A, typename B>
    A clamp(B x)
    {
        if (std::numeric_limits<A>::min() >= x)
            return std::numeric_limits<A>::min();
        if (x >= std::numeric_limits<A>::max())
            return std::numeric_limits<A>::max();
        return x;
    }

    extern const s16 CUTOFF_TO_IIR_TABLE[128][4];
}
