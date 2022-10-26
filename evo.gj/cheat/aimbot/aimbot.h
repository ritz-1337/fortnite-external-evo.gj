#pragma once
#include "../sdk/sdk.h"
#include "../esp/esp.h"

extern void aimbot();

inline uintptr_t TargetPawn = 0;
inline float ClosestDistance = FLT_MAX;
inline int FovSize = 200;
inline float Smooth = 4;
inline bool bAimbot = true;
inline bool bIsTargeting = false;