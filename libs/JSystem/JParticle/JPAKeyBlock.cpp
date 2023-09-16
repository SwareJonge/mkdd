#include "types.h"
#include "JSystem/JParticle/JPABlock.h"
#include "JSystem/JParticle/JPAMath.h"

JPAKeyBlock::JPAKeyBlock(const u8 *data)
    : mDataStart(data), mKeyData(reinterpret_cast<const float *>(&data[0xC]))
{
}

void JPAKeyBlock::init_jpa(const u8 *, JKRHeap *)
{
    // UNUSED FUNCTION
}

void JPAKeyBlock::calc(f32 p1)
{
    if (mDataStart[0xB] != '\0')
    {
        int v1 = (int)mKeyData[(mDataStart[9] - 1) * 4] + 1;
        // p1 -= (v1 * ((int)p1 / v1));
        int v2 = ((int)p1 / v1);
        p1 = p1 - (v2 * v1);
    }
    JPACalcKeyAnmValue(p1, mDataStart[9], mKeyData);
}