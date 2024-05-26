#ifndef MSL_BITSET_H_
#define MSL_BITSET_H_

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/abort_exit.h"

namespace std
{
    // TODO: where does this go?
    inline void __msl_error(const char *param_0)
    {
        fprintf(stderr, param_0);
        abort();
    }

    template <size_t N>
    class __bitset_base
    {
    public:
        __bitset_base();

        void set(size_t pos, bool val);
        void reset(size_t pos);
        bool test(size_t pos) const;
        bool any() const;
    private:
        size_t data[N];
    };

    template <size_t N>
    __bitset_base<N>::__bitset_base()
    {
        std::fill<size_t *, size_t>(data, data + N, 0);
    }

    template <size_t N>
    bool __bitset_base<N>::any() const
    {
        for (size_t i = 0; i < N; i++)
        {
            if (data[i] != 0)
                return true;
        }
        return false;
    }

    template <size_t N>
    bool __bitset_base<N>::test(size_t pos) const
    {
        size_t i = pos / (sizeof(size_t) * 8);
        size_t mask = 1 << (pos % (sizeof(size_t) * 8));
        return data[i] & mask;
    }

    template <size_t N>
    void __bitset_base<N>::set(size_t pos, bool val)
    {
        size_t i = pos / (sizeof(size_t) * 8);
        size_t mask = 1 << (pos % (sizeof(size_t) * 8));
        if (val)
        {
            data[i] |= mask;
        }
        else
        {
            data[i] &= ~mask;
        }
    }

    template <size_t N>
    void __bitset_base<N>::reset(size_t pos)
    {
        size_t i = pos / (sizeof(size_t) * 8);
        size_t mask = 1 << (pos % (sizeof(size_t) * 8));
        data[i] &= ~mask;
    }

    template <size_t N>
    class bitset : private __bitset_base<(N - 1) / (sizeof(size_t) * 8) + 1>
    {
    public:
        typedef __bitset_base<(N - 1) / (sizeof(size_t) * 8) + 1> base;

        bitset() {}
        bool any() const { return base::any(); }
        void set(size_t pos, bool val)
        {
            if (pos >= N)
            {
                __msl_error("index out of range of bitset::set");
            }
            base::set(pos, val);
        }
        void reset(size_t pos)
        {
            if (pos >= N)
            {
                __msl_error("index out of range of bitset::reset");
            }
            base::reset(pos);
        }
        bool test(size_t pos) const
        {
            if (pos >= N)
            {
                __msl_error("index out of range of bitset::test");
            }
            return base::test(pos);
        }
        
    };
} // namespace std

#endif