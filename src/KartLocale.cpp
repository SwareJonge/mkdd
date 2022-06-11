#include "KartLocale.h"

namespace KartLocale {
    void KartLocale::localize() {
        switch (msRegion) {
        case JAPAN:
            msLanguage = JAPANESE;
            msSpeedUnit = KMH;
            msVideoMode = NTSC;
            msVideoFrameMode = EURGB60;
            return;
        case UNITED_STATES:
            msLanguage = ENGLISH;
            msSpeedUnit = MPH;
            msVideoMode = NTSC;
            msVideoFrameMode = EURGB60;
            return;
        case UNITED_KINGDOM:
            msLanguage = ENGLISH;
            msSpeedUnit = MPH;
            msVideoMode = PAL;
            msVideoFrameMode = PAL50;
            return;
        case GERMANY:
            msLanguage = GERMAN;
            msSpeedUnit = KMH;
            msVideoMode = PAL;
            msVideoFrameMode = PAL50;
            return;
        case FRANCE:
            msLanguage = FRENCH;
            msSpeedUnit = KMH;
            msVideoMode = PAL;
            msVideoFrameMode = PAL50;
            return;
        case ITALY:
            msLanguage = ITALIAN;
            msSpeedUnit = KMH;
            msVideoMode = PAL;
            msVideoFrameMode = PAL50;
            return;
        case SPAIN:
            msLanguage = SPANISH;
            msSpeedUnit = KMH;
            msVideoMode = PAL;
            msVideoFrameMode = PAL50;
            return;
        }
    }
}

