#ifndef CRITICAL_REGIONS_GAMECUBE_H
#define CRITICAL_REGIONS_GAMECUBE_H

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

void __init_critical_regions();
void __kill_critical_regions();
void __begin_critical_region(int);
void __end_critical_region(int);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif // CRITICAL_REGIONS_GAMECUBE_H