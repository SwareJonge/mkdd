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
VideoMode KartLocale::msVideoMode = VideoMode_PAL;
VideoFrameMode KartLocale::msVideoFrameMode = VideoFrameMode_50Hz;
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
        msSpeedUnit = SpeedUnit_Metric;
        msVideoMode = VideoMode_NTSC;
        msVideoFrameMode = VideoFrameMode_60Hz;
        return;
    case UNITED_STATES:
        msLanguage = ENGLISH;
        msSpeedUnit = SpeedUnit_Impractical;
        msVideoMode = VideoMode_NTSC;
        msVideoFrameMode = VideoFrameMode_60Hz;
        return;
    case UNITED_KINGDOM:
        msLanguage = ENGLISH;
        msSpeedUnit = SpeedUnit_Impractical;
        msVideoMode = VideoMode_PAL;
        msVideoFrameMode = VideoFrameMode_50Hz;
        return;
    case GERMANY:
        msLanguage = GERMAN;
        msSpeedUnit = SpeedUnit_Metric;
        msVideoMode = VideoMode_PAL;
        msVideoFrameMode = VideoFrameMode_50Hz;
        return;
    case FRANCE:
        msLanguage = FRENCH;
        msSpeedUnit = SpeedUnit_Metric;
        msVideoMode = VideoMode_PAL;
        msVideoFrameMode = VideoFrameMode_50Hz;
        return;
    case ITALY:
        msLanguage = ITALIAN;
        msSpeedUnit = SpeedUnit_Metric;
        msVideoMode = VideoMode_PAL;
        msVideoFrameMode = VideoFrameMode_50Hz;
        return;
    case SPAIN:
        msLanguage = SPANISH;
        msSpeedUnit = SpeedUnit_Metric;
        msVideoMode = VideoMode_PAL;
        msVideoFrameMode = VideoFrameMode_50Hz;
        return;
    }
}


