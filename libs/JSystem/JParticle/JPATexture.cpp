#include "JSystem/JParticle/JPATexture.h"

#include "JSystem/JParticle/JPATexture.h"

JPATexture::JPATexture(const u8 *p1)
    : mTexture(), mData((const JPATextureData *)p1)
{
    mTexture.storeTIMG(&mData->mResTIMG, (u8)'\0');
}

JPATexture::~JPATexture() {}
