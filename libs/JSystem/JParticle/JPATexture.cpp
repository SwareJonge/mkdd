#include "JSystem/JParticle/JPATexture.h"

JPATexture::JPATexture(const unsigned char *p1)
    : mTexture(), _44(p1)
{
    mTexture.storeTIMG(reinterpret_cast<const ResTIMG *>(_44 + 0x20), (u8)'\0');
}

JPATexture::~JPATexture() {}