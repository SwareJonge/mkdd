#include "JSystem/JMAth/JMath.h"
#include "types.h"

namespace JMath {
    const TSinCosTable<2048, float> sincosTable_;
    const TAtanTable<1024, float> atanTable_;

    float JMath::TAtanTable<1024, float>::atan2_(float param_1, float param_2) const
    {
        // const register float zero = 0.0f;
        // float fVar1 = 0.0f;
        float result;
        if (param_1 >= 0.0f)
        {
            if (param_2 >= 0.0f)
            {
                if (param_2 >= param_1)
                {
                    // if (param_2 != 0.0f) {
                    // 	fVar1 = m_table[(int)((param_1 * 1024.0f) / param_2 +
                    // 0.5f)];
                    // }
                    // return param_2 == 0.0f ? fVar1 : m_table[(int)((param_1 *
                    // 1024.0f) / param_2 + 0.5f)]; fVar1 = param_2 == 0.0f ? fVar1
                    // : m_table[(int)((param_1 * 1024.0f) / param_2 + 0.5f)];
                    // return (param_2 == 0.0f ? 0.0f : m_table[(int)((param_1 *
                    // 1024.0f) / param_2 + 0.5f)]);
                    result = (param_2 == 0.0f ? 0.0f : m_table[(int)((param_1 * 1024.0f) / param_2 + 0.5f)]);
                }
                else
                {
                    // if (param_1 != 0.0f) {
                    // 	fVar1 = m_table[(int)((param_2 * 1024.0f) / param_1 +
                    // 0.5f)];
                    // }
                    // fVar1 = HALF_PI - fVar1;
                    // return HALF_PI - (param_1 == 0.0f ? 0.0f :
                    // m_table[(int)((param_2 * 1024.0f) / param_1 + 0.5f)]);
                    result = HALF_PI - (param_1 == 0.0f ? 0.0f : m_table[(int)((param_2 * 1024.0f) / param_1 + 0.5f)]);
                }
            }
            else
            {
                float fVar2 = -param_2;
                if (fVar2 < param_1)
                {
                    // if (param_1 != 0.0f) {
                    // 	fVar1 = m_table[(int)((fVar2 * 1024.0f) / param_1 + 0.5f)];
                    // }
                    // fVar1 = fVar1 + HALF_PI;
                    // return (param_1 == 0.0f ? 0.0f : m_table[(int)((fVar2 *
                    // 1024.0f) / param_1 + 0.5f)]) + HALF_PI;
                    result = (param_1 == 0.0f ? 0.0f : m_table[(int)((fVar2 * 1024.0f) / param_1 + 0.5f)]) + HALF_PI;
                }
                else
                {
                    // if (fVar2 != 0.0f) {
                    // 	fVar1 = m_table[(int)((param_1 * 1024.0f) / fVar2 + 0.5f)];
                    // }
                    // fVar1 = PI - fVar1;
                    // return PI - (fVar2 == 0.0f ? 0.0f : m_table[(int)((param_1 *
                    // 1024.0f) / fVar2 + 0.5f)]);
                    result = PI - (fVar2 == 0.0f ? 0.0f : m_table[(int)((param_1 * 1024.0f) / fVar2 + 0.5f)]);
                }
            }
        }
        else
        {
            float fVar2 = -param_1;
            if (param_2 < 0.0f)
            {
                float fVar3 = -param_2;
                if (fVar3 >= fVar2)
                {
                    // if (fVar3 != 0.0f) {
                    // 	fVar1 = m_table[(int)((fVar2 * 1024.0f) / fVar3 + 0.5f)];
                    // }
                    // fVar1 = fVar1 + -PI;
                    // return (fVar3 == 0.0f ? 0.0f : m_table[(int)((fVar2 *
                    // 1024.0f) / fVar3 + 0.5f)]) + -PI;
                    result = (fVar3 == 0.0f ? 0.0f : m_table[(int)((fVar2 * 1024.0f) / fVar3 + 0.5f)]) + -PI;
                }
                else
                {
                    // if (fVar2 != 0.0f) {
                    // 	fVar1 = m_table[(int)((fVar3 * 1024.0f) / fVar2 + 0.5f)];
                    // }
                    // fVar1 = -HALF_PI - fVar1;
                    // return -HALF_PI - (fVar2 == 0.0f ? 0.0f :
                    // m_table[(int)((fVar3 * 1024.0f) / fVar2 + 0.5f)]);
                    result = -HALF_PI - (fVar2 == 0.0f ? 0.0f : m_table[(int)((fVar3 * 1024.0f) / fVar2 + 0.5f)]);
                }
            }
            else
            {
                if (param_2 < fVar2)
                {
                    // if (fVar2 != 0.0f) {
                    // 	fVar1 = m_table[(int)((param_2 * 1024.0f) / fVar2 + 0.5f)];
                    // }
                    // fVar1 = fVar1 + -HALF_PI;
                    // return (fVar2 == 0.0f ? 0.0f : m_table[(int)((param_2 *
                    // 1024.0f) / fVar2 + 0.5f)]) + -HALF_PI;
                    result = (fVar2 == 0.0f ? 0.0f : m_table[(int)((param_2 * 1024.0f) / fVar2 + 0.5f)]) + -HALF_PI;
                }
                else
                {
                    // if (param_2 != 0.0f) {
                    // 	fVar1 = m_table[(int)((fVar2 * 1024.0f) / param_2 + 0.5f)];
                    // }
                    // fVar1 = -fVar1;
                    // return -(param_2 == 0.0f ? 0.0f : m_table[(int)((fVar2 *
                    // 1024.0f) / param_2 + 0.5f)]);
                    result = -(param_2 == 0.0f ? 0.0f : m_table[(int)((fVar2 * 1024.0f) / param_2 + 0.5f)]);
                }
            }
        }
        // return fVar1;
        return result;
    }
}