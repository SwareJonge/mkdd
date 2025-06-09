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
        void set(const SMatrix34C<T> &rSrc);
        void set(T rxx, T ryx, T rzx, T tx, T rxy, T ryy, T rzy, T ty, T rxz, T ryz, T rzz, T tz);

        void scale(T);

        inline void set(const Mtx pSrc)
        {
            JMath::gekko_ps_copy12(this, pSrc);
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
            this->ref(2, 3) = zero;
            this->ref(1, 3) = zero;
            this->ref(0, 3) = zero;

            this->ref(1, 2) = zero;
            this->ref(0, 2) = zero;

            this->ref(2, 1) = zero;
            this->ref(0, 1) = zero;
            this->ref(2, 0) = zero;
            this->ref(1, 0) = zero;

            const T one = TUtil<T>::one();
            this->ref(2, 2) = one;
            this->ref(1, 1) = one;
            this->ref(0, 0) = one;
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
            rDest.set<f32>(this->mMtx[0][0], this->mMtx[1][0], this->mMtx[2][0]);
        };

        void getYDir(TVec3f &rDest) const
        {
            rDest.set<f32>(this->mMtx[0][1], this->mMtx[1][1], this->mMtx[2][1]);
        };

        void getZDir(TVec3f &rDest) const
        {
            rDest.set<f32>(this->mMtx[0][2], this->mMtx[1][2], this->mMtx[2][2]);
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
            this->ref(0, 0) = rSrcX.x;
            this->ref(1, 0) = rSrcX.y;
            this->ref(2, 0) = rSrcX.z;
            this->ref(0, 1) = rSrcY.x;
            this->ref(1, 1) = rSrcY.y;
            this->ref(2, 1) = rSrcY.z;
            this->ref(0, 2) = rSrcZ.x;
            this->ref(1, 2) = rSrcZ.y;
            this->ref(2, 2) = rSrcZ.z;
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
            f32 z = this->mMtx[2][0];
            f32 y = this->mMtx[1][0];
            f32 x = this->mMtx[0][0];
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
        void getTrans(TVec3f &rDest) const {
            rDest.set(this->mMtx[0][3], this->mMtx[1][3], this->mMtx[2][3]);
        }
        void setTrans(const TVec3f &rSrc);
        void setTrans(f32 x, f32 y, f32 z);
        void zeroTrans()
        {
            this->ref(0, 3) = 0.0f;
            this->ref(1, 3) = 0.0f;
            this->ref(2, 3) = 0.0f;
        }

        void makeRotate(const TVec3f &, f32);
        void makeQuat(const TQuat4f &rSrc);
        void setPositionFromLookAt(const TPosition3<T> &rLookAt) {
            // regswaps
            this->ref(0, 0) = -rLookAt[0][0]; 
            this->ref(1, 1) = rLookAt[1][1];
            this->ref(2, 2) = -rLookAt[2][2];            
            this->ref(1, 0) = -rLookAt[0][1]; 
            this->ref(0, 1) = rLookAt[1][0]; 
            this->ref(2, 0) = -rLookAt[0][2];
            this->ref(0, 2) = -rLookAt[2][0]; 
            this->ref(1, 2) = -rLookAt[2][1];
            this->ref(2, 1) = rLookAt[1][2];
            

            f32 x = rLookAt[0][3];
            f32 y = rLookAt[1][3];
            f32 z = rLookAt[2][3];

            this->ref(0, 3) = (rLookAt[0][3] * this->mMtx[0][0]) - (rLookAt[1][3] * this->mMtx[0][1]) + rLookAt[2][3] * this->mMtx[0][2];
            this->ref(1, 3) = (rLookAt[0][3] * this->mMtx[1][0]) - (rLookAt[1][3] * this->mMtx[1][1]) + rLookAt[2][3] * this->mMtx[1][2];
            this->ref(2, 3) = (rLookAt[0][3] * this->mMtx[2][0]) - (rLookAt[1][3] * this->mMtx[2][1]) + rLookAt[0][3] * this->mMtx[2][2];
        }
        void setQT(const TQuat4f &rSrcQuat, const TVec3f &rSrcTrans);

        inline void getTransInline(TVec3f &rDest) const
        {
            f32 z = this->mMtx[2][3];
            f32 y = this->mMtx[1][3];
            f32 x = this->mMtx[0][3];
            rDest.set(x, y, z);
        }
    };

    typedef TMatrix34<TSMtxf> TMtx34f;
    typedef TRotation3<TMtx34f> TRot3f;
    typedef TPosition3<TMtx34f> TPos3f;
}

#endif
