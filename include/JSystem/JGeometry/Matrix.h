#ifndef JGEOMETRY_MATRIX_H
#define JGEOMETRY_MATRIX_H

#include <JSystem/JGeometry/Vec.h>
#include <JSystem/JGeometry/Quat.h>

#include "types.h"

namespace JGeometry {
    template <typename T>
    struct SMatrix34C
    {
    public:
        SMatrix34C() {}
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
                // clang-format off
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
                // clang-format on
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
                // clang-format off
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
                // clang-format on
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
                // clang-format off
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
                // clang-format on
            }
            ;
        }

        T &ref(u32 i, u32 j) { return mMtx[i][j]; }

        inline Mtx *toMtx()
        {
            return (Mtx *)mMtx;
        }

        inline const Mtx *toCMtx() const
        {
            return (const Mtx *)mMtx;
        }

        operator MtxPtr() const { return (MtxPtr)mMtx; }

        T mMtx[3][4];
    };

    typedef SMatrix34C<f32> TSMtxf;

    template <typename T>
    struct TMatrix34 : public T
    {
    public:
        void identity() // TODO: OK?
        {
            const T zero = (T)0;
            ref(2, 3) = zero;
            ref(1, 3) = zero;
            ref(0, 3) = zerof;

            ref(1, 2) = zero;
            ref(0, 2) = zero;

            ref(2, 1) = zero;
            ref(0, 1) = zero;
            ref(2, 0) = zero;
            ref(1, 0) = zero;

            const T one = TUtil<T>::one();
            ref(2, 2) = one;
            ref(1, 1) = one;
            ref(0, 0) = one;
        }
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
        TRotation3() {}
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
        void setXYZDir(const TVec3f &rSrcX, const TVec3f &rSrcY, const TVec3f &rSrcZ)
        {
            ref(0, 0) = rSrcX.x;
            ref(1, 0) = rSrcX.y;
            ref(2, 0) = rSrcX.z;
            ref(0, 1) = rSrcY.x;
            ref(1, 1) = rSrcY.y;
            ref(2, 1) = rSrcY.z;
            ref(0, 2) = rSrcZ.x;
            ref(1, 2) = rSrcZ.y;
            ref(2, 2) = rSrcZ.z;
        }

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
        TPosition3() {}
        void getTrans(TVec3f &rDest) const;
        void setTrans(const TVec3f &rSrc);
        void setTrans(f32 x, f32 y, f32 z);
        void zeroTrans()
        {
            ref(0, 3) = 0.0f;
            ref(1, 3) = 0.0f;
            ref(2, 3) = 0.0f;
        }

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
}

#endif
