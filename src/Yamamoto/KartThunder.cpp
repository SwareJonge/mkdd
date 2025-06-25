#include "Yamamoto/KartThunder.h"
#include "Yamamoto/kartBody.h"
#include "Yamamoto/kartCtrl.h"
#include <std/math.h>

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartThunder::Init(int idx) {
    mBody = GetKartCtrl()->getKartBody(idx);
    mFlags = 0;
    mState = 0;
    mTimer = 0;
    mScale = 1.0f;
    _c = 0.7f;
    _10 = 0.0f;
}

bool KartThunder::DoReturn() {
    _c = 0.2f;
    _10 = 0.0f;
    mScale = GetKartCtrl()->fcnvge(mScale, 1.0f, 0.5f, 0.5f);
    if (mScale >= 1.0f)
        return true;
    return false;
}

bool KartThunder::DoShake() {
    _c = GetKartCtrl()->fcnvge(_c, 0.0f, 0.006f, 0.006f);
    _10 += 0.680333f;
    mScale = _c * std::sinf(_10) + 1.0f;
    if (_c == 0.0f)
        return true;
    return false;
}

void KartThunder::ClearThunder() {
    KartBody *body = mBody;
    if (mFlags & 1) {
        mFlags = 0;
        mScale = 1.0f;
        mState = 0;
        body->mCarStatus &= ~0x80000000ull;
        body->mShadowModel->setScale(1.0f, 1.0f);
        GetKartCtrl()->getKartSound(body->mMynum)->DoChibiSound(false);
    }
}

void KartThunder::MakeThunder() {
    KartBody *body = mBody;
    int kartCount = GetKartCtrl()->GetKartNumber();
    GetKartCtrl()->getKartSound(body->mMynum)->DoThunderSound();
    if (GetKartCtrl()->IsMiniGameEnd())
        return;

    for (int i = 0; i < kartCount; i++) {
        KartBody *other = GetKartCtrl()->getKartBody(i);
        if ((body == other) || (other->mCarStatus & (0x100000000000ull | 0x8000000000ull | 0x100000000ull | 0x500000ull | 0x40000ull)) || ((other->getThunder()->mFlags & 1))) {
            continue;
        }

        other->getThunder()->mFlags |= 1;
        if ((body->mCarStatus & 0x8000000000ull)) {
            body->getCannon()->mFlags &= ~0x20;
        }
        other->getCrash()->MakeThunderSpin();
        other->getItem()->FallAllItem();
        other->getStrat()->DoMotor(MotorManager::MotorType_13);
        GetKartCtrl()->getKartSound(i)->DoChibiSound(true);
        other->getThunder()->mScale = 1.0f;
        other->getThunder()->mTimer = 540;
        other->getThunder()->mState = 1;
        other->getGame()->DoItmCancel();
    }
}

void KartThunder::DoThunderCrl() {
    KartBody *body = mBody;
    switch (mState) {
    case 1: {
      mTimer--;
      mScale = GetKartCtrl()->fcnvge(mScale,0.5f, 0.03f, 0.03f);
      if (mTimer == 510) {
        mState = 2;
      }
        break;
    }
    case 2: {
        mTimer--;
        if (mTimer == 0) {
            mState = 3;
            mTimer = 0;
            GetKartCtrl()->getKartSound(body->mMynum)->DoChibiSound(false);
        }
        break;
    }
    case 3: {
        if (DoReturn()) {
            mState = 4;
        }
        break;
    }
    case 4: {
        if (DoShake()) {
            mState = 5;
        }
        break;
    }
    case 5: {
        ClearThunder();
        break;
    }
    }
}
