#ifndef PRINTAPP_H
#define PRINTAPP_H

#include "Kameda/PrintProgressive.h"
#include "Kawano/PrintPal.h"

#ifndef REGION_EU
#define PRINTAPP_TYPE PrintProgressive
#else
#define PRINTAPP_TYPE PrintPal
#endif

#endif