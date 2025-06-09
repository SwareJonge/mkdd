#ifndef GAMEAUDIOCUSTOM_H
#define GAMEAUDIOCUSTOM_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/JAUSoundAnimator.h"

namespace GameAudio {

class CustomAnimationSoundStarter : public JAISoundStarter {
public:
    CustomAnimationSoundStarter();
    bool startSound(JAISoundID soundID, JAISoundHandle *handle, const JGeometry::TVec3f *);
private:
    u8 _4;
};

class CustomAnimator : public JAUSoundAnimator {
public:
    CustomAnimator(JAISoundHandles *handles);
    JAISoundHandle *getFreeHandle(const JAUSoundAnimationSound *);
    void modifySoundParams(JASSoundParams *, const JAUSoundAnimationSound *, float);
};

}

#endif
