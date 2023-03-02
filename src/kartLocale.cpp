#include "kartLocale.h"

Language KartLocale::msLanguage;
SpeedUnit KartLocale::msSpeedUnit;

#if REGION_JP // not tested
VideoMode KartLocale::msVideoMode;
VideoFrameMode KartLocale::msVideoFrameMode;
Country KartLocale::msCountry = JAPAN;
Region KartLocale::msRegion = JP;
DatingMode KartLocale::msDatingMode = YYYY_MM_DD;
#elif REGION_US
VideoMode KartLocale::msVideoMode;
VideoFrameMode KartLocale::msVideoFrameMode;
Country KartLocale::msCountry = UNITED_STATES;
Region KartLocale::msRegion = NA;
DatingMode KartLocale::msDatingMode = MM_DD_YYYY; // worst format
#else                                             // REGION_EU
Country KartLocale::msCountry = UNITED_KINGDOM;
Region KartLocale::msRegion = EU;
VideoMode KartLocale::msVideoMode = PAL;
VideoFrameMode KartLocale::msVideoFrameMode = PAL50;
DatingMode KartLocale::msDatingMode = DD_MM_YYYY;
#endif

const char * KartLocale::mscpaLanguageName[] = {
    "English",
    "French",
    "German",
    "Italian",
    "Japanese",
    "Spanish"
};

void KartLocale::localize() {
    switch (getCountry()) {
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


