#ifndef JSYSTEM_HASHCODE_H
#define JSYSTEM_HASHCODE_H

#include "types.h"

namespace JGadget
{
    u32 getHashCode(const char *, u32);
    u32 getHashCode(const char *);
    
    namespace
    {
        template <typename T>
        struct TPRIsEnd_value_
        {
            TPRIsEnd_value_()
            {
                val = 0;
            }
            T val;
        };

        template <typename T, typename U>
        static u32 getHashCode_(const char *pName, U end)
        {
            u32 hash = 0;

            for (; *pName != end.val; pName++)
            {
                hash = *pName + hash * 31;
            }
            return hash;
        }
    }
}

#endif
