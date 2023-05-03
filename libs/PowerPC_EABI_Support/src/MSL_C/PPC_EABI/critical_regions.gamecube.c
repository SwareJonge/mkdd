#include "types.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/critical_regions.h"

void __init_critical_regions(void) { return; }

void __kill_critical_regions(void) { return; }

void __begin_critical_region(int region) { return; }

void __end_critical_region(int region) { return; }
