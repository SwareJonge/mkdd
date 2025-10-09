#ifndef JGEOMETRY_QUAT_H
#define JGEOMETRY_QUAT_H

#include <JSystem/JGeometry/Vec.h>
#include "JSystem/JGeometry/Util.h"
#include "dolphin/mtx.h"
#include "types.h"

namespace JGeometry {
    template <typename T>
    struct TQuat4
    {
    public:
        /* Constructors */
        inline TQuat4() {}

        template <typename A>
        TQuat4(A _x, A _y, A _z, A _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        template <typename A>
        void set(A _x, A _y, A _z, A _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
    };

    template <>
    class TQuat4<f32> : public Quaternion {
    public:
        template <typename A>
        void set(A _x, A _y, A _z, A _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
        /* General operations */
        void scale(f32 s) {
            PSQUATScale(this, this, s);
        }
        
        void scale(f32 s, const TQuat4 &rSrc) {
            PSQUATScale(&rSrc, this, s);
        }

        f32 squared() const {
            return PSQUATDotProduct(this, this);
        }

        f32 squared(const TQuat4 &rOther) const {
            return PSQUATDotProduct(this, &rOther);
        }

        f32 normalize() { // TODO: OK?
            f32 sq = squared();
            if (sq <= TUtilf::epsilon())
                return 0.0f;
            else
            {
                f32 invsqrt = TUtilf::inv_sqrt(sq);
                scale(invsqrt);
                return invsqrt * sq;
            }
        }

        f32 normalize(const TQuat4 &other) {
            f32 sq = other.squared();
            if (sq <= TUtilf::epsilon()) {
                set(0.0f, 0.0f, 0.0f, 1.0f);
                return 0.0f;
            }
            
            f32 invsqrt = TUtilf::inv_sqrt(sq);
            scale(invsqrt, other);
            return invsqrt * sq;
        }

        void getXDir(TVec3f &rDest) const;
        void getYDir(TVec3f &rDest) const;
        void getZDir(TVec3f &rDest) const;

        void getEuler(TVec3f &rDest) const;
        void setEuler(f32 _x, f32 _y, f32 _z);
        void setEulerZ(f32 _z);

        void setRotate(const TVec3f &, const TVec3f &, f32);
        void setRotate(const TVec3f &, const TVec3f &);
        void setRotate(const TVec3f &, f32);
        void rotate(TVec3f &rDest) const;

        void slerp(const TQuat4 &a1, const TQuat4 &a2, f32 a3) {
            this->x = a1.x;
            this->y = a1.y;
            this->z = a1.z;
            this->w = a1.w;
            this->slerp(a2, a3);
        }
        
        void slerp(const TQuat4 &other, f32 t) {
            // Normalize both quaternions
            TQuat4f q1;
            q1.normalize(*this);

            TQuat4f q2;
            q2.normalize(other);

            // Compute the cosine of the angle between them
            f32 dot = q1.squared(q2);

            // If dot < 0, negate q2 for shortest path
            if (dot < 0.0f) {
                dot = -dot;
                q2.x = -q2.x;
                q2.y = -q2.y;
                q2.z = -q2.z;
                q2.w = -q2.w;
            }

            f32 s0, s1;
            if (1.0f - dot > TUtilf::epsilon()) {
                // Standard slerp
                f32 theta = acos(dot);
                f32 sinTheta = sin(theta);
                s0 = sin((1.0f - t) * theta) / sinTheta;
                s1 = sin(t * theta) / sinTheta;
            } else {
                // Quaternions are very close, use linear interpolation
                s0 = 1.0f - t;
                s1 = t;
            }

            this->x = s0 * q1.x + s1 * q2.x;
            this->y = s0 * q1.y + s1 * q2.y;
            this->z = s0 * q1.z + s1 * q2.z;
            this->w = s0 * q1.w + s1 * q2.w;
        }

        void transform(const TVec3f &, TVec3f &rDest);
        void transform(TVec3f &rDest);

        bool equals(const TQuat4 &other) 
        {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }
    };

    typedef TQuat4<f32> TQuat4f;
}

#endif
