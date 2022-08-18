#ifndef GAMEAUDIOMAIN_H
#define GAMEAUDIOMAIN_H

#include "types.h"

namespace GameAudio {
    class Main {
        public:
        static Main * getAudio();
        void startSystemSe(u32);
    };
}

#endif