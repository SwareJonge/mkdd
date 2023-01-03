#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/JFrameWork/JFWDisplay.h"

JFWDisplay::~JFWDisplay() {
    if(JUTVideo::getManager() != nullptr) {
        waitBlanking(2);
    }
    JUTProcBar::destroy();
    JUTXfb::destroyManager();
    m_Xfb = 0;
}