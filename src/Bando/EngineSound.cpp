#include "Bando/EngineSound.h"

namespace EngineSound
{
    // Enjin zenkai
    EngineType getEngineType(EKartID kartID)
    {
        EngineType engineType = ENGINE0;
        switch (kartID)
        {
        case DK_JUMBO:
        case BARREL_TRAIN:
            engineType = ENGINE0;
            break;
        case BULLET_BLASTER:
        case GREEN_FIRE:
        case RED_FIRE:       
            engineType = ENGINE1;
            break;
        case KOOPA_KING:
            engineType = ENGINE2;
            break;
        case GOO_GOO_BUGGY:
        case RATTLE_BUGGY:
            engineType = ENGINE3;
            break;
        case HEART_COACH:
        case BLOOM_COACH:
        case BOO_PIPES:
        case PIRANHA_PIPES:
            engineType = ENGINE4;
            break;
        case KOOPA_DASHER:
        case PARA_WING:
            engineType = ENGINE5;
            break;
        case TURBO_YOSHI:
        case TURBO_BIRDO:
        case TOAD_KART:
        case TOADETTE_KART:
            engineType = ENGINE6;
            break;
        case WARIO_CAR:
        case WALUIGI_RACER:
            engineType = ENGINE7;
            break;
        case PARADE_KART:
            engineType = ENGINE8;
            break;
        }
        return engineType;
    }
}