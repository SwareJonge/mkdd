#ifndef JGEOMETRY_VEC_H
#define JGEOMETRY_VEC_H

#include <JSystem/JGeometry/Util.h>
#include "types.h"

namespace JGeometry {
    inline void setTVec3f(register const f32 *src, register f32 *dst)
    {
        register f32 xy, z;
        __asm {
            // clang-format off
            psq_l xy, 0(src), 0, 0
            lfs z, 8(src)
            psq_st xy, 0(dst), 0, 0
            stfs z, 8(dst)
            // clang-format on
        }
    }

    inline void negateInternal(register const f32 *src, register f32 *dst)
    {
        register f32 xy, z, negz;
        __asm {
            // clang-format off
            psq_l xy, 0(src), 0, 0
            ps_neg xy, xy
            psq_st xy, 0(dst), 0, 0

            lfs z, 8(src)            
            fneg negz, z;            
            stfs negz, 8(dst)
            // clang-format on
        }
    }

    template <typename T>
    struct TVec2
    {
        TVec2() {}
        TVec2(T x, T y) { set(x, y); }

        void set(T x, T y)
        {
            this->x = x;
            this->y = y;
        }

        void set(const TVec2 &other)
        {
            x = other.x;
            y = other.y;
        }

        void setMin(const TVec2<f32> &min)
        {
            if (x >= min.x)
                x = min.x;
            if (y >= min.y)
                y = min.y;
        }

        void setMax(const TVec2<f32> &max)
        {
            if (x <= max.x)
                x = max.x;
            if (y <= max.y)
                y = max.y;
        }

        void add(const TVec2<T> &other)
        {
            x += other.x;
            y += other.y;
        }

        /** @fabricated */
        // TVec2<T> adding(const TVec2<T>& other) { return TVec2<T>(x + other.x, y + other.y); }

        bool isAbove(const TVec2<T> &other) const { return (x >= other.x) && (y >= other.y) ? true : false; }

        T x;
        T y;
    };

    // sort this someday
    template <typename T>
    class TVec3
    {
    public:
        TVec3() {}
        TVec3(const TVec3<f32> &other)
        {
            setTVec3f((const f32 *)&other, (f32 *)this);
        }

        template <typename TY>
        TVec3(TY X, TY Y, TY Z)
        {
            set(X, Y, Z);
        }

        TVec3 &operator=(const TVec3 &);
        TVec3 &operator*=(const TVec3 &operand);
        TVec3 &operator+(const TVec3 &operand){
            add(operand);
            return *this;
        } 
        TVec3 & operator*=(float scalar)
        {
            scale(scalar);
        }
        TVec3 &operator-=(const TVec3 &operand);

        /*TVec3 &operator*(float scalar) const
        {
            TVec3 scaled(*this);
            scaled *= scalar;
            this->TVec3(scaled);
        }*/

        f32 angle(TVec3 vec2) {
            TVec3 crossp;
            crossp.cross(*this, vec2);

            f32 ang = TUtil<f32>::atan2(crossp.length(), dot(vec2));

            return TUtil<f32>::abs(ang);
        }

        void add(const TVec3 &operand)
        {
            JMathInlineVEC::PSVECAdd((const Vec *)this, (Vec *)&operand, (Vec *)this);
        }
        void add(const TVec3 &vec1, const TVec3 &vec2)
        {
            JMathInlineVEC::PSVECAdd((const Vec *)&vec1, (const Vec *)&vec2, (Vec *)this);
        }
        void div(f32 divisor);
        void cross(const TVec3 &vec1, const TVec3 &vec2)
        {
            PSVECCrossProduct((const Vec *)&vec1, (const Vec *)&vec2, (Vec *)this);
        }
        f32 dot(const TVec3 &operand) const
        {
            return JMathInlineVEC::PSVECDotProduct((const Vec *)this, (Vec *)&operand);
        }
        f32 length() const {
            return PSVECMag((Vec *)this);
        }
        void negate()
        {
            negateInternal((const f32 *)this, (f32 *)this);
        }
        void negate(const TVec3 &other)
        {
            negateInternal((const f32 *)&other, (f32 *)this);
        }
        f32 normalize()
        {
            f32 this_squared = squared();
            if (this_squared <= TUtil<f32>::epsilon())
                return 0.0f;
            else
            {
                f32 invsqrt = TUtil<f32>::inv_sqrt(this_squared);
                scale(invsqrt);
                return invsqrt * this_squared;
            }
        }
        void scale(f32 scalar)
        {
            JMathInlineVEC::PSVECScale((const Vec *)this, (Vec *)this, scalar);
        }

        void scale(f32 scalar, const TVec3 &operand)
        {
            JMathInlineVEC::PSVECScale((const Vec *)&operand, (Vec *)this, scalar);
        }

        void scaleAdd(f32 scalar, const TVec3 &operand, const TVec3 &translate) {
            JMAVECScaleAdd((const Vec *)&operand, (const Vec *)&translate, (Vec *)this, scalar);
        }

        f32 squared() const
        {
            return JMathInlineVEC::PSVECSquareMag((Vec *)this);
        }

        template <typename TY>
        void set(TY X, TY Y, TY Z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        template <typename TY>
        void set(const TVec3<TY> &src)
        {
            x = src.x;
            y = src.y;
            z = src.z;
        }

        f32 setLength(f32 length)
        {
            if (length <= TUtil<f32>::epsilon())
            {
                return 0.0f;
            }
            else
            {
                f32 invsqrt = TUtil<f32>::inv_sqrt(length);
                scale(invsqrt);
                return invsqrt * length;
            }
        }
        void setLength(const TVec3 &operand, f32 length);
        void setMax(const TVec3 &other);
        void setMin(const TVec3 &other);
        void sub(const TVec3<T> &translate)
        { // matches but it doesn't really look nice
            JMathInlineVEC::PSVECSubtract((const Vec *)this, (const Vec *)&translate, (Vec *)this);
        }
        void sub(const TVec3 &base, const TVec3 &translate)
        {
            JMathInlineVEC::PSVECSubtract((const Vec *)&base, (const Vec *)&translate, (Vec *)this);
        }

        void zero()
        {
            z = (T)0;
            y = (T)0;
            x = (T)0;
        }

        bool isZero() const {
            return (squared() <= TUtil<f32>::epsilon());
        }

        T x;
        T y;
        T z;
    };
    typedef TVec2<f32> TVec2f;
    typedef TVec3<f32> TVec3f;
}

#endif
