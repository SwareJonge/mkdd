#include "Kaneshige/Course/Course.h"

// WIP, not actively working on

void Course::Sector::calcGeneration(Course::Sector * sector) {
    Sector * cur;

    short generation = 0;
    for(cur = this; cur != sector; cur = getPrevSector(0)) {
        generation++;
    }

    mGeneration = generation;
}