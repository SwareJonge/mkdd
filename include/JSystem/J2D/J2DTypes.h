#ifndef _JSYSTEM_J2D_J2DTYPES_H
#define _JSYSTEM_J2D_J2DTYPES_H

#include "types.h"

struct J2DBlendInfo
{
    J2DBlendInfo() {}

    J2DBlendInfo(u8 type, u8 srcFactor, u8 destFactor)
    {
        m_type = type;
        m_srcFactor = srcFactor;
        m_destFactor = destFactor;
    }

    void operator=(J2DBlendInfo const &other)
    {
        m_type = other.m_type;
        m_srcFactor = other.m_srcFactor;
        m_destFactor = other.m_destFactor;
    }

    u8 m_type;
    u8 m_srcFactor;
    u8 m_destFactor;
};

#endif