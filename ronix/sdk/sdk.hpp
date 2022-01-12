#ifndef SDK_HPP
#define SDK_HPP

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#ifndef GNUC
#define GNUC 1
#endif

#ifndef _LINUX
#define _LINUX 1
#endif

#ifndef LINUX
#define LINUX 1
#endif

#ifndef POSIX
#define POSIX 1
#endif

#ifndef CLIENT_DLL
#define CLIENT_DLL 1
#endif

#ifndef USE_SDL
#define USE_SDL 1
#endif

#ifndef DX_TO_GL_ABSTRACTION
#define DX_TO_GL_ABSTRACTION
#endif

#ifndef RAD_TELEMETRY_DISABLED
#define RAD_TELEMETRY_DISABLED 1
#endif

// HL2SDK
#define CRC32_INIT_VALUE 0xFFFFFFFFUL
#define CRC32_XOR_VALUE  0xFFFFFFFFUL
#define NUM_BYTES 256

#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <hl2sdk/public/tier0/platform.h>
#include <hl2sdk/public/tier0/basetypes.h>
#include <hl2sdk/public/tier1/interface.h>
#include <hl2sdk/public/const.h>
#include <hl2sdk/public/networkvar.h>
#include <hl2sdk/public/icliententity.h>
#include <hl2sdk/public/icliententitylist.h>
#include <hl2sdk/game/shared/sheetsimulator.h>
#include <hl2sdk/game/client/cbase.h>
#include <hl2sdk/game/client/c_baseentity.h>
#include <hl2sdk/game/client/c_baseplayer.h>
#include <hl2sdk/public/appframework/ilaunchermgr.h>
#include <hl2sdk/public/cdll_int.h>
#include <hl2sdk/game/client/cdll_client_int.h>
#include <hl2sdk/game/client/input.h>
#include <hl2sdk/game/client/iclientmode.h>
#include <hl2sdk/game/client/clientmode_shared.h>
#include <hl2sdk/game/shared/in_buttons.h>
#include <hl2sdk/public/engine/ivmodelinfo.h>
#include <hl2sdk/public/engine/ivmodelrender.h>
#include <hl2sdk/public/materialsystem/imaterialsystem.h>
#include <hl2sdk/public/materialsystem/imaterial.h>
#include <hl2sdk/public/materialsystem/imaterialvar.h>
#include <hl2sdk/public/ivrenderview.h>
#include <hl2sdk/public/globalvars_base.h>
#include <hl2sdk/game/shared/gamemovement.h>
#include <hl2sdk/public/engine/IEngineTrace.h>

int FrustumTransform( const VMatrix &worldToSurface, const Vector& point, Vector& screen );

#endif
