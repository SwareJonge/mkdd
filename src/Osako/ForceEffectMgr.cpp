#include <JSystem/JUtility/JUTGamePad.h>

#include "Osako/ForceEffectMgr.h"

ForceEffectMgr *ForceEffectMgr::mspForceEffectMgr;

ForceEffectMgr::ForceEffectMgr()
{
    _18 = 40;
    _1a = 120;
}

ForceEffectMgr::~ForceEffectMgr()
{
    end();
    mspForceEffectMgr = nullptr;
}

void ForceEffectMgr::start()
{
    LGWheels *wheels = JUTGamePad::getLGWheels();

    for (u8 i = 0; i < 4; i++)
    {
        if (wheels->IsConnected(i))
        {
            wheels->PlaySpringForce(i, 0, _18, _1a);
        }
    }
}

void ForceEffectMgr::end()
{
    LGWheels *wheels = JUTGamePad::getLGWheels();

    for (u8 i = 0; i < 4; i++)
    {
        if (wheels->IsConnected(i))
        {
#ifdef DEBUG // what's up with the disabling of rumble stuff for the release build?
            wheels->StopBumpyRoadEffect(i);
            wheels->StopSlipperyRoadEffect(i);
            wheels->StopCarAirborne(i);
#endif
            wheels->PlaySpringForce(i, 0, _18, _1a);
        }
    }
}