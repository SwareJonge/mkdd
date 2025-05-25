#include "JSystem/JAudio/JAUSoundTable.h"

#include "JSystem/JAudio/JASFakeMatch5.h"

void JAUSoundTable::init(const void *res) {
    if (res != NULL) {
        mTable.init(res);
    } else {
        mTable.reset();
    }
}

u8 JAUSoundTable::getTypeID(JAISoundID soundID) const {
    if (soundID.isAnonymous()) {
        return 0xff;
    }
    JAUSoundTableSection* section = mTable.getSection(soundID.mId.mBytes.mSectionId);
    if (section == NULL) {
        return 0xff;
    }
    JAUSoundTableGroup* group = mTable.getGroup(section, soundID.mId.mBytes.mGroupId);
    if (group == NULL) {
        return 0xff;
    }
    return group->getTypeID(soundID.mId.mAdvancedId.mShortId);
}

JAUSoundTableItem *JAUSoundTable::getData(JAISoundID soundID) const {
    if (soundID.isAnonymous()) {
        return NULL;
    }
    JAUSoundTableSection *section = mTable.getSection(soundID.mId.mBytes.mSectionId);
    if (section == NULL) {
        return NULL;
    }
    JAUSoundTableGroup *group = mTable.getGroup(section, soundID.mId.mBytes.mGroupId);
    if (group == NULL) {
        return NULL;
    }
    return getItem(group, soundID.mId.mAdvancedId.mShortId);
}

u32 JAUSoundTable::getNumGroups_inSection(u8 sectionID) const {
    JAUSoundTableSection *section = mTable.getSection(sectionID);
    if (section == NULL) {
        return 0xffffffff;
    }
    return section->mNumGroups;
}

u32 JAUSoundTable::getNumItems_inGroup(u8 sectionID, u8 groupID) const {
    JAUSoundTableSection *section = mTable.getSection(sectionID);
    if (section == NULL) {
        return 0xffffffff;
    }
    JAUSoundTableGroup *group = mTable.getGroup(section, groupID);
    if (group == NULL) {
        return 0xffffffff;
    }
    return group->mNumItems;
}

void JAUSoundNameTable::init(const void *res) {
    if (res != NULL) {
        mTable.init(res);
    } else {
        mTable.reset();
    }
}
