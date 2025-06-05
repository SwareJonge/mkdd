#include "JSystem/JAudio/System/JASCmdStack.h"

#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "dolphin/os.h"

JASPortCmd::TPortHead JASPortCmd::sCommandListOnce;
JASPortCmd::TPortHead JASPortCmd::sCommandListStay;

void JASPortCmd::execAllCommand() {
    sCommandListOnce.execCommandOnce();
    sCommandListStay.execCommandStay();
}

void JASPortCmd::TPortHead::execCommandOnce() {
    JAS_CS_START
    JSULink<JASPortCmd> *next;
    for (JSULink<JASPortCmd> *link = getFirst(); link != NULL; link = next) {
        next = link->getNext();
        JASPortCmd *cmd = link->getObject();
        cmd->getFunc()(cmd->getArgs());
        remove(link);
    }
    JAS_CS_END
}

void JASPortCmd::TPortHead::execCommandStay() {
    JAS_CS_START
    for (JSULink<JASPortCmd> *link = getFirst(); link != NULL; link = link->getNext()) {
        JASPortCmd *cmd = link->getObject();
        cmd->getFunc()(cmd->getArgs());
    }
    JAS_CS_END
}
