#ifndef JGEOMETRY_PUBLIC_H
#define JGEOMETRY_PUBLIC_H

#include "types.h"
#include <JSystem/JMath/JMath.h>

namespace JGeometry
{
    inline void setTVec3f(register const f32 *src, register f32 *dst)
    {
        register f32 xy, z;
        __asm {
            psq_l xy, 0(src), 0, 0
            lfs z, 8(src)
            psq_st xy, 0(dst), 0, 0
            stfs z, 8(dst)
        }
    }

    template <typename T>
    class TUtil
    {
    public:
        static f32 atan2(f32 y, f32 x) { return JMAAtan2Radian(y, x); }
        static const f32 epsilon() { return 32.0f * FLT_EPSILON; }

        static int epsilonEquals(T a1, T a2, T a3)
        {
            int ret = 0;

            a1 -= a2;

            if (-a3 <= a1 && a1 <= a3)
            {
                ret = 1;
            }

            return ret;
        }

        static T sqrt(T val)
        {
            if (val <= 0.0f)
                return 0.0f;


            float outVal = __frsqrte(val);
            return (val * (0.5f * outVal * (3.0f - (val * (outVal * outVal)))));
        }

        static T inv_sqrt(T number)
        {
            if (number <= 0.0f)
                return number;

            const f32 threehalfs = 3.0f;
            f32 y = __frsqrte(number);
            y = 0.5f * y * (threehalfs - (number * (y * y)));

            return y;
        }

        /*static f32 asin(f32 val) NO_INLINE
        {
            if (val >= 1.0f)
            {
                return 1.5707964f;
            }

            if (val <= -1.0f)
            {
                return -1.5707964f;
            }

            if (val < 0.0f)
            {
                return -flt_80630CA0[(u32)(1023.5f * -val)];
            }

            return flt_80630CA0[(u32)(1023.5f * val)];
        }*/

        /*static f32 PI() NO_INLINE
        {
            return 3.1415927f;
        }*/

        static T clamp(T, T, T);
    };

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
        TVec3 &operator-=(const TVec3 &operand);

        void add(const TVec3 &operand)
        {
            JMathInlineVEC::PSVECAdd((const Vec *)this, (Vec *)&operand, (Vec *)this);
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
        void negate();
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
        /*void scale(T scalar)
        {
            x *= scalar;
            y *= scalar;
        }*/
        void scale(f32 scale, const TVec3 &operand);
        void scaleAdd(f32 scale, const TVec3 &operand, const TVec3 &translate);

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

    template <typename T>
    struct TQuat4
    {
    public:
        /* Constructors */
        inline TQuat4() {}

        template <typename T>
        TQuat4(T _x, T _y, T _z, T _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        /* General operations */
        void normalize();
        void normalize(const TQuat4<T> &rSrc);

        void getXDir(TVec3<T> &rDest) const;
        void getYDir(TVec3<T> &rDest) const;
        void getZDir(TVec3<T> &rDest) const;

        void getEuler(TVec3<T> &rDest) const;
        void setEuler(T _x, T _y, T _z);
        void setEulerZ(T _z);

        void setRotate(const TVec3<T> &, const TVec3<T> &, T);
        void setRotate(const TVec3<T> &, const TVec3<T> &);
        void setRotate(const TVec3<T> &, f32);
        void rotate(TVec3f &rDest) const;

        void slerp(const TQuat4<T> &, const TQuat4<T>, T);
        void slerp(const TQuat4<T> &, T);
        void transform(const TVec3<T> &, TVec3<T> &rDest);
        void transform(TVec3<T> &rDest);

        /* Operators */
        TQuat4<T> &operator=(const TQuat4<T> &rSrc);

        T x, y, z, w;
    };

    typedef TQuat4<f32> TQuat4f;

    // Size: 0x10
    template <class T>
    struct TBox
    {
        TBox()
            : i(), f()
        {
        }
        TBox(const TBox &other)
            : i(other.f), f(other.y)
        {
        }

        T i, f;
    };

    // clang-format off
template<> struct TBox<TVec2<f32> > {
    f32 getWidth() const { return f.x - i.x; }
    f32 getHeight() const { return f.y - i.y; }

    bool isValid() const { return f.isAbove(i); }

    void addPos(f32 x, f32 y) {
        addPos(TVec2<f32>(x, y));
    }

    void addPos(const TVec2<f32>& pos) {
        i.add(pos);
        f.add(pos);
    }

    bool intersect(const TBox<TVec2<f32> >& other) {
        i.setMax(other.i);
        f.setMin(other.f);
        return isValid();
    }

    TVec2<f32> i, f;
};

template <typename T>
struct TBox2 : TBox<TVec2<T> > {
    TBox2() {}
    TBox2(const TVec2<f32>& i, const TVec2<f32> f) { set(i, f); }
    TBox2(f32 x0, f32 y0, f32 x1, f32 y1) { set(x0, y0, x1, y1); }
	TBox2(f32 x0, f32 y0, TVec2<f32>& f) { set(x0, y0, x0 + f.x, y0 + f.y);	}
	TBox2(f32 val)
	{
		f.y = val;
		f.x = val;
		i.y = val;
		i.x = val;
	}

    void absolute() {
        if (!this->isValid()) {
            TBox2<T> box(*this);
            this->i.setMin(box.i);
            this->i.setMin(box.f);
            this->f.setMax(box.i);
            this->f.setMax(box.f);
        }
    }

	// /** @fabricated */
	// TBox2<T>& addingPos(TBox2<T>& result, const TVec2<T>& pos) {
	// 	return TBox2<T>(i.adding(pos), f.adding(pos));
	// }

    void set(const TBox2& other) { set(other.i, other.f); }
    void set(const TVec2<f32>& i, const TVec2<f32>& f) { this->i.set(i), this->f.set(f); }
    void set(f32 x0, f32 y0, f32 x1, f32 y1) { i.set(x0, y0); f.set(x1, y1); }
};

    // clang-format on


    template <typename T>
    struct TBox3
    {
        T minX;
        T minY;
        T minZ;
        T maxX;
        T maxY;
        T maxZ;
    };

    template <typename T>
    class TDirBox3
    {
    public:
        TVec3<T> _0;
        TVec3<T> _C;
        TVec3<T> _18;
        TVec3<T> _24;
        TVec3<T> _30;
    };

    typedef TBox2<f32> TBox2f;
    typedef TBox3<f32> TBox3f;
    typedef TBox3<TVec3f> TBoxVec3f;
    typedef TDirBox3<f32> TDirBox3f;

    template <typename T>
    struct SMatrix34C
    {
    public:
        void set(const MtxPtr);
        void set(const SMatrix34C<T> &rSrc);
        void set(T rxx, T ryx, T rzx, T tx, T rxy, T ryy, T rzy, T ty, T rxz, T ryz, T rzz, T tz);

        void scale(T);

        inline void setInline(const SMatrix34C<T> &rSrc)
        {
            register const SMatrix34C<T> *pSrc = &rSrc;
            register SMatrix34C<T> *pDest = this;
            register f32 rzztz;
            register f32 rxzyz;
            register f32 rzyty;
            register f32 rxyyy;
            register f32 rzxtx;
            register f32 rxxyx;

            __asm {
                psq_l     rxxyx, 0(pSrc), 0, 0
                psq_l     rzxtx, 8(pSrc), 0, 0
                psq_l     rxyyy, 0x10(pSrc), 0, 0
                psq_l     rzyty, 0x18(pSrc), 0, 0
                psq_l     rxzyz, 0x20(pSrc), 0, 0
                psq_l     rzztz, 0x28(pSrc), 0, 0
                psq_st    rxxyx, 0(pDest), 0, 0
                psq_st    rzxtx, 8(pDest), 0, 0
                psq_st    rxyyy, 0x10(pDest), 0, 0
                psq_st    rzyty, 0x18(pDest), 0, 0
                psq_st    rxzyz, 0x20(pDest), 0, 0
                psq_st    rzztz, 0x28(pDest), 0, 0
            }
            ;
        }

        inline void setInline(const MtxPtr rSrc)
        {
            register const MtxPtr pSrc = rSrc;
            register SMatrix34C<T> *pDest = this;
            register f32 rzztz;
            register f32 rxzyz;
            register f32 rzyty;
            register f32 rxyyy;
            register f32 rzxtx;
            register f32 rxxyx;

            __asm {
                psq_l     rxxyx, 0(pSrc), 0, 0
                psq_l     rzxtx, 8(pSrc), 0, 0
                psq_l     rxyyy, 0x10(pSrc), 0, 0
                psq_l     rzyty, 0x18(pSrc), 0, 0
                psq_l     rxzyz, 0x20(pSrc), 0, 0
                psq_l     rzztz, 0x28(pSrc), 0, 0
                psq_st    rxxyx, 0(pDest), 0, 0
                psq_st    rzxtx, 8(pDest), 0, 0
                psq_st    rxyyy, 0x10(pDest), 0, 0
                psq_st    rzyty, 0x18(pDest), 0, 0
                psq_st    rxzyz, 0x20(pDest), 0, 0
                psq_st    rzztz, 0x28(pDest), 0, 0
            }
            ;
        }

        inline void setInline_2(MtxPtr rSrc)
        {
            register MtxPtr pSrc = rSrc;
            register SMatrix34C<T> *pDest = this;
            register f32 rzztz;
            register f32 rxzyz;
            register f32 rzyty;
            register f32 rxyyy;
            register f32 rzxtx;
            register f32 rxxyx;

            __asm {
                psq_l     rxxyx, 0(pSrc), 0, 0
                psq_l     rzxtx, 8(pSrc), 0, 0
                psq_l     rxyyy, 0x10(pSrc), 0, 0
                psq_st    rxxyx, 0(pDest), 0, 0
                psq_l     rzyty, 0x18(pSrc), 0, 0
                psq_st    rzxtx, 8(pDest), 0, 0
                psq_l     rxzyz, 0x20(pSrc), 0, 0
                psq_st    rxyyy, 0x10(pDest), 0, 0
                psq_l     rzztz, 0x28(pSrc), 0, 0
                psq_st    rzyty, 0x18(pDest), 0, 0
                psq_st    rxzyz, 0x20(pDest), 0, 0
                psq_st    rzztz, 0x28(pDest), 0, 0
            }
            ;
        }

        inline Mtx *toMtx()
        {
            return (Mtx *)mMtx;
        }

        inline const Mtx *toCMtx() const
        {
            return (const Mtx *)mMtx;
        }

        inline MtxPtr toMtxPtr()
        {
            return (MtxPtr)mMtx;
        }

        T mMtx[3][4];
    };

    typedef SMatrix34C<f32> TSMtxf;

    template <typename T>
    struct TMatrix34 : public T
    {
    public:
        void identity();
        void concat(const T &rSrcA, const T &rSrcB);
        void concat(const T &rSrc);
        void invert(const TMatrix34<T> &rDest);

        void mult(const TVec3f &rSrc, TVec3f &rDest) const;

        void multTranspose(const TVec3f &a1, const TVec3f &a2) const;
    };

    template <class T>
    struct TRotation3 : public T
    {
    public:
        void identity33();

        void getXDir(TVec3f &rDest) const
        {
            rDest.set<f32>(mMtx[0][0], mMtx[1][0], mMtx[2][0]);
        };

        void getYDir(TVec3f &rDest) const
        {
            rDest.set<f32>(mMtx[0][1], mMtx[1][1], mMtx[2][1]);
        };

        void getZDir(TVec3f &rDest) const
        {
            rDest.set<f32>(mMtx[0][2], mMtx[1][2], mMtx[2][2]);
        };

        void getXYZDir(TVec3f &rDestX, TVec3f &rDestY, TVec3f &rDestZ) const;
        void setXDir(const TVec3f &rSrc);
        void setXDir(f32 x, f32 y, f32 z);
        void setYDir(const TVec3f &rSrc);
        void setYDir(f32 x, f32 y, f32 z);
        void setZDir(const TVec3f &rSrc);
        void setZDir(f32 x, f32 y, f32 z);
        void setXYZDir(const TVec3f &rSrcX, const TVec3f &rSrcY, const TVec3f &rSrcZ);

        void getEuler(TVec3f &rDest) const;
        void getEulerXYZ(TVec3f &rDest) const;
        void setEulerY(f32 val);
        void setEulerZ(f32 val);

        void getQuat(TQuat4f &rDest) const;
        void setQuat(const TQuat4f &rSrc);

        void getScale(TVec3f &rDest) const;
        void setScale(const TVec3f &rSrc);

        void setRotate(const TVec3f &, const TVec3f &);
        void setRotate(const TVec3f &, f32);

        void mult33(TVec3f &) const;
        void mult33(const TVec3f &, TVec3f &) const;

        inline void getXDirInline(TVec3f &rDest) const
        {
            f32 z = mMtx[2][0];
            f32 y = mMtx[1][0];
            f32 x = mMtx[0][0];
            rDest.set(x, y, z);
        }

#ifdef NON_MATCHING
        inline void mult33Inline(const TVec3f &rSrc, TVec3f &rDest) const
        {
            rDest.set<f32>(
                rSrc.z * mMtx[0][2] + (rSrc.y * mMtx[0][0] + (rSrc.x * mMtx[0][1])),
                rSrc.z * mMtx[1][2] + (rSrc.y * mMtx[1][0] + (rSrc.x * mMtx[1][1])),
                rSrc.z * mMtx[2][2] + (rSrc.y * mMtx[2][0] + (rSrc.x * mMtx[2][1])));
        }
#endif
    };

    template <class T>
    struct TPosition3 : public TRotation3<T>
    {
    public:
        void getTrans(TVec3f &rDest) const;
        void setTrans(const TVec3f &rSrc);
        void setTrans(f32 x, f32 y, f32 z);
        void zeroTrans();

        void makeRotate(const TVec3f &, f32);
        void makeQuat(const TQuat4f &rSrc);
        void setPositionFromLookAt(const TPosition3<T> &rLookAt);
        void setQT(const TQuat4f &rSrcQuat, const TVec3f &rSrcTrans);

        inline void getTransInline(TVec3f &rDest) const
        {
            f32 z = mMtx[2][3];
            f32 y = mMtx[1][3];
            f32 x = mMtx[0][3];
            rDest.set(x, y, z);
        }
    };

    typedef TMatrix34<TSMtxf> TMtx34f;
    typedef TRotation3<TMtx34f> TRot3f;
    typedef TPosition3<TMtx34f> TPos3f;
};

#endif
