#include "types.h"

// might be a class, who knows
namespace JASCalc {
    void imixcopy(const s16 *s1, const s16 *s2, s16 *dst, u32 size);
    void bcopyfast(const void *src, void *dest, u32 size);
    void bcopy(const void *src, void *dest, u32 size);
    void bzerofast(void *dest, u32 size);
    void bzero(void *dest, u32 size);

    template <typename T1, typename T2>
    T1 clamp(T1, T2);
    template <typename T1, typename T2>
    extern T1 clamp(T2) {
        if (std::numeric_limits<T1>::min() >= val)
        {
            return std::numeric_limits<T1>::min();
        }
        if (val >= std::numeric_limits<T1>::max())
        {
            return std::numeric_limits<T1>::max();
        }
        return val;
    }

    extern const s16 CUTOFF_TO_IIR_TABLE[128][4];
}
