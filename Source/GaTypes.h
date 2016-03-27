#pragma once

#include "Base/BcFixedVectors.h"
#include "Math/MaVec2d.h"
#include "Math/MaVec3d.h"
#include "Math/MaVec4d.h"

#if 0
typedef BcF32 GaReal;
typedef MaVec2d GaVec2d;
typedef MaVec3d GaVec3d;
typedef MaVec4d GaVec4d;
#else
typedef BcFixed<> GaReal;
typedef BcFixedVec2d GaVec2d;
typedef BcFixedVec3d GaVec3d;
typedef BcFixedVec4d GaVec4d;
#endif

