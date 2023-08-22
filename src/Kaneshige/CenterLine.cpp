#include "Kaneshige/CenterLine.h"

CLPoint::CLPoint()
{
    mKind = 0;
    mPointData = nullptr;
    mDivPoint = nullptr;

    for (int i = 0; i < 4; i++)
    {
        mPrevPoints[i] = nullptr;
        mNextPoints[i] = nullptr;
    }
}

bool CLPoint::setPrevPoint(CLPoint *prev)
{
    bool ret = false;

    for (int i = 0; i < 4; i++)
    {
        if (mPrevPoints[i] == nullptr)
        {
            mPrevPoints[i] = prev;
            ret = true;
            break;
        }
    }

    return ret;
}

bool CLPoint::setNextPoint(CLPoint *next)
{
    bool ret = false;

    for (int i = 0; i < 4; i++)
    {
        if (mNextPoints[i] == nullptr)
        {
            mNextPoints[i] = next;
            ret = true;
            break;
        }
    }

    return ret;
}

int CLPoint::getPrevPointNumber()
{
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        if (mPrevPoints[i] != nullptr)
        {
            cnt++;
        }
    }
    return cnt;
}

int CLPoint::getNextPointNumber()
{
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        if (mNextPoints[i] != nullptr)
        {
            cnt++;
        }
    }
    return cnt;
}

int CLPoint::getBrosPointNumber()
{
    int cnt = 0;
    int prevPointCnt = getPrevPointNumber();

    for (int i = 0; i < prevPointCnt; i++)
    {
        CLPoint *prev = getPrevPoint(i);
        int nextCnt = prev->getNextPointNumber();
        for (int j = 0; j < nextCnt; j++)
        {
            if (prev->getNextPoint(j) != this)
            {
                cnt++;
            }
        }
    }
    return cnt;
}

CLPoint *CLPoint::getBrosPoint(int cnt)
{
    int brosPoints = 0;
    int prevPointCnt = getPrevPointNumber();;
    for (int i = 0; i < prevPointCnt; i++)
    {
        CLPoint *prev = getPrevPoint(i);
        int nextCnt = prev->getNextPointNumber();
        for (int j = 0; j < nextCnt; j++)
        {
            CLPoint *next = prev->getNextPoint(j);
            if (next != this)
            {
                if (brosPoints == cnt)
                {
                    return next;
                }

                brosPoints++;
            }
        }
    }
    return nullptr;
}