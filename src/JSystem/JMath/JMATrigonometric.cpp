#include "JSystem/JMAth/JMath.h"
#include "types.h"

namespace JMath {
    float TAtanTable<1024, float>::atan2_(float param_1, float param_2) const
    {
        float result;
        if (param_1 >= 0.0f)
        {
            if (param_2 >= 0.0f)
            {
                if (param_2 >= param_1)
                    result = (param_2 == 0.0f ? 0.0f : m_table[(int)((param_1 * 1024.0f) / param_2 + 0.5f)]); // fmr is here instead of at the end
                else
                    result = HALF_PI - (param_1 == 0.0f ? 0.0f : m_table[(int)((param_2 * 1024.0f) / param_1 + 0.5f)]);
            }
            else if (-param_2 < param_1)
                result = (param_1 == 0.0f ? 0.0f : m_table[(int)((-param_2 * 1024.0f) / param_1 + 0.5f)]) + HALF_PI;
            else
                result = PI - (-param_2 == 0.0f ? 0.0f : m_table[(int)((param_1 * 1024.0f) / -param_2 + 0.5f)]);
        }
        else
        {
            float fVar2 = -param_1;
            if (param_2 < 0.0f)
            {
                float fVar3 = -param_2;
                if (fVar3 >= fVar2)
                {
                    result = (fVar3 == 0.0f ? 0.0f : m_table[(int)((fVar2 * 1024.0f) / fVar3 + 0.5f)]) + -PI;
                }
                else
                {
                    result = (-HALF_PI - (fVar2 == 0.0f ? 0.0f : m_table[(int)((fVar3 * 1024.0f) / fVar2 + 0.5f)]));
                }
            }
            else
            {
                if (param_2 < fVar2)
                {
                    result = (fVar2 == 0.0f ? 0.0f : m_table[(int)((param_2 * 1024.0f) / fVar2 + 0.5f)]) + -HALF_PI;
                }
                else
                {
                    result = -(param_2 == 0.0f ? 0.0f : m_table[(int)((fVar2 * 1024.0f) / param_2 + 0.5f)]);
                }
            }
        }
        return result;
    }
    const TSinCosTable<2048, float> sincosTable_ __attribute__((aligned(32)));
    const TAtanTable<1024, float> atanTable_ __attribute__((aligned(32)));
    const TAsinAcosTable<1024, float> asinAcosTable_ __attribute__((aligned(32)));
}
    