#include "kartLocale.h"

Country KartLocale::msCountry = UNITED_STATES;
Region KartLocale::msRegion = NA;
DatingMode KartLocale::msDatingMode = MM_DD_YYYY;

const char * KartLocale::mscpaLanguageName[] = {
    "English",
    "French",
    "German",
    "Italian",
    "Japanese",
    "Spanish"
};

Language KartLocale::msLanguage;
SpeedUnit KartLocale::msSpeedUnit;
VideoMode KartLocale::msVideoMode;
VideoFrameMode KartLocale::msVideoFrameMode;

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


