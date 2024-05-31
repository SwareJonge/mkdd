#include "types.h"
#include "JSystem/JParticle/JPABlock.h"
#include "JSystem/JParticle/JPAMath.h"

JPAKeyBlock::JPAKeyBlock(const u8 *data)
    : mDataStart(reinterpret_cast<const JPAKeyBlockData *>(data)), mKeyFrameData(reinterpret_cast<const f32 *>(&data[0xC]))
{
}

void JPAKeyBlock::init_jpa(const u8 *, JKRHeap *)
{
    // UNUSED FUNCTION
}

f32 JPAKeyBlock::calc(f32 currentFrame)
{
    if (mDataStart->_0B != '\0')
    {
        int lastKeyFrameIndex = (int)mKeyFrameData[(mDataStart->mKeyFrameCount - 1) * 4] + 1;
        int currentFrameRatio = ((int)currentFrame / lastKeyFrameIndex);
        currentFrame = currentFrame - (currentFrameRatio * lastKeyFrameIndex);
    }

    return JPACalcKeyAnmValue(currentFrame, mDataStart->mKeyFrameCount, mKeyFrameData);
}