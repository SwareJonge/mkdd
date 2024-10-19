#ifndef JGEOMETRY_VEC_H
#define JGEOMETRY_VEC_H

#include <JSystem/JGeometry/Util.h>
#include "types.h"

namespace JGeometry {
    inline void setTVec3f(register const f32 *src, register f32 *dst)
    {
#ifdef __MWERKS__
        register f32 xy, z;
        __asm {
            // clang-format off
            psq_l xy, 0(src), 0, 0
            lfs z, 8(src)
            psq_st xy, 0(dst), 0, 0
            stfs z, 8(dst)
            // clang-format on
        }
#endif
    }

    inline void negateInternal(register const f32 *src, register f32 *dst)
    {
#ifdef __MWERKS__
        register f32 xy;
        __asm {
            // clang-format off
            psq_l xy, 0(src), 0, 0
            ps_neg xy, xy
            psq_st xy, 0(dst), 0, 0
            // clang-format on
        }
        dst[2] = -src[2];
#endif
    }

    inline void mulInternal(register const f32 *vec1, register const f32 *vec2, register f32 *dst) 
    {
#ifdef __MWERKS__
        register f32 xy1, xy2, res;
        __asm {
            // clang-format off
            psq_l xy1, 0(vec1), 0, 0
            psq_l xy2, 0(vec2), 0, 0
            ps_mul res, xy1, xy2
            psq_st res, 0(dst), 0, 0
            // clang-format on
        }
        dst[2] = vec1[2] * vec2[2];
#endif
    }

    template <typename T>
    class TVec2
    {
    public:
        TVec2() {}
        TVec2(T X, T Y) { set(X, Y); }

        void zero() { x = y = 0; }

        template <typename TY>
        void set(TY X, TY Y)
        {
            x = X;
            y = Y;
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

        void scale(T scalar) {
            x *= scalar;
            y *= scalar;
        }

        f32 dot(const TVec2<T> &other) const { return x * other.x + y * other.y; }
        f32 length() const { return TUtilf::sqrt(squared()); }
        f32 squared() const { return dot(*this); }

        f32 normalize()
        {
            f32 this_squared = squared();
            if (this_squared <= TUtilf::epsilon())
                return 0.0f;
            else
            {
                f32 invsqrt = TUtilf::inv_sqrt(this_squared);
                scale(invsqrt);
                return invsqrt * this_squared;
            }
        }

        /** @fabricated */
        // TVec2<T> adding(const TVec2<T>& other) { return TVec2<T>(x + other.x, y + other.y); }

        bool isAbove(const TVec2<T> &other) const { return (x >= other.x) && (y >= other.y) ? true : false; }

        T x;
        T y;
    };

    template <typename T>
    class TVec3
    {
    public:
        // constructors
        TVec3() {}
        TVec3(const TVec3<f32> &other) { setTVec3f((const f32 *)&other, (f32 *)this); }
        TVec3(const Vec &other) { setTVec3f((const f32 *)&other, (f32 *)this); }

        template <typename TY>
        TVec3(TY X, TY Y, TY Z) { set(X, Y, Z); }

        // getters/setters
        bool equals(const TVec3<f32> &other) 
        {
            bool equal = false;
            if (x == other.x && y == other.y && z == other.z)
            {
                equal = true;
            }
            return equal;
        }

        bool isZero() const { return (squared() <= TUtilf::epsilon()); }

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

        template <typename TY>
        void setAll(TY val)
        {
            x = val;
            y = val;
            z = val;
        }

        f32 setLength(f32 length)
        {
            f32 sq = squared();
            if (sq <= TUtilf::epsilon())
                return 0.0f;

            f32 invsqrt = TUtilf::inv_sqrt(sq);
            scale(invsqrt * length);
            return invsqrt * sq;
        }
        void setLength(const TVec3 &operand, f32 length);
        void setMax(const TVec3 &other);
        void setMin(const TVec3 &other);
        void setNormal(const TVec3 &, const TVec3 &, const TVec3 &);

        void zero() { x = y = z = 0; }        

        // operators
        TVec3 &operator=(const TVec3 &other) {
            setTVec3f((const f32*)&other, (f32 *)this);
            return *this;
        }

        /*void operator+(const TVec3 &operand)
        {
            add(operand);
            return *this;
        }*/

        TVec3 operator*(f32 scalar) const
        {
            TVec3 scaled(*this);
            scaled *= scalar;
            return scaled;
        }

        TVec3 &operator+=(const TVec3 &operand)
        {
            add(operand);
            return *this;
        }

        TVec3 &operator-=(const TVec3 &operand)
        {
            sub(operand);
            return *this;
        }

        TVec3 &operator*=(f32 scalar)
        {
            scale(scalar);
            return *this;
        }

        TVec3 &operator/=(f32 divisor)
        {
            div(divisor);
            return *this;
        }

        // simple math operations

        void add(const TVec3 &operand) { JMathInlineVEC::PSVECAdd((const Vec *)this, (Vec *)&operand, (Vec *)this); }
        void add(const TVec3 &vec1, const TVec3 &vec2) { JMathInlineVEC::PSVECAdd((const Vec *)&vec1, (const Vec *)&vec2, (Vec *)this); }
        
        void sub(const TVec3<T> &translate) { JMathInlineVEC::PSVECSubtract((const Vec *)this, (const Vec *)&translate, (Vec *)this); }
        void sub(const TVec3 &base, const TVec3 &translate) { JMathInlineVEC::PSVECSubtract((const Vec *)&base, (const Vec *)&translate, (Vec *)this); }

        void negate() { negateInternal((const f32 *)this, (f32 *)this); }
        void negate(const TVec3 &other) { negateInternal((const f32 *)&other, (f32 *)this); }

        void mul(const TVec3 &multiplier) { mulInternal((const f32 *)this, (const f32 *)multiplier, (f32 *)this); }
        void mul(const TVec3 &vec1, const TVec3 &vec2) { mulInternal((const f32 *)vec1, (const f32 *)vec2, (f32 *)this); }

        void scale(f32 scalar) { JMathInlineVEC::PSVECScale((const Vec *)this, (Vec *)this, scalar); }
        void scale(f32 scalar, const TVec3 &operand) { JMathInlineVEC::PSVECScale((const Vec *)&operand, (Vec *)this, scalar); }

        void scaleAdd(f32 scalar, const TVec3 &operand, const TVec3 &translate) { JMAVECScaleAdd((const Vec *)&operand, (const Vec *)&translate, (Vec *)this, scalar); }

        void div(f32 divisor) { return scale(TUtilf::invert(divisor)); }
        void div(f32 divisor, const TVec3 &operand) { return scale(TUtilf::invert(divisor), operand); }

        f32 squared() const { return JMathInlineVEC::PSVECSquareMag((Vec *)this); }
        f32 squaredZX() const { return dotZX(this); }
        // more complex math operations

        void cross(const TVec3 &vec1, const TVec3 &vec2) { PSVECCrossProduct((const Vec *)&vec1, (const Vec *)&vec2, (Vec *)this); }
        f32 dot(const TVec3 &operand) const { return JMathInlineVEC::PSVECDotProduct((const Vec *)this, (Vec *)&operand); }
        f32 dotZX(const TVec3 &operand) const { return x * operand.x + z * operand.z; }
        f32 length() const { return PSVECMag((Vec *)this); }

        f32 angle(const TVec3 &vec2) const
        {
            TVec3 crossp;
            crossp.cross(*this, vec2);
            f32 len = crossp.length();
            f32 dp = dot(vec2);
            f32 ang = TUtilf::atan2(len, dp);

            return TUtilf::abs(ang);
        }

        f32 normalize()
        {
            f32 this_squared = squared();
            if (this_squared <= TUtilf::epsilon())
                return 0.0f;
            else
            {
                f32 invsqrt = TUtilf::inv_sqrt(this_squared);
                scale(invsqrt);
                return invsqrt * this_squared;
            }
        }

        T x;
        T y;
        T z;
    };
    typedef TVec2<f32> TVec2f;
    typedef TVec3<f32> TVec3f;
}

#endif
