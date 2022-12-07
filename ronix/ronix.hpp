#ifndef RONIX_HPP
#define RONIX_HPP

#include "pch.hpp"
#include "utils/logger.hpp"
#include "utils/vmtmgr.hpp"
#include "config.hpp"
#include "cstrike.hpp"
#include "gui.hpp"
#include "gamedata.hpp"

#define SDL_NUM_MOUSEBUTTONS 5
#define KEYS_MOUSE_INDEX SDL_NUM_SCANCODES
#define KEYS_LENGTH (SDL_NUM_SCANCODES + SDL_NUM_MOUSEBUTTONS)

namespace Ronix {
	void Init();
	void Shutdown();

	namespace Data {
		extern bool hasShutdown;
		extern std::unique_ptr<Logger> logger;
		extern std::unique_ptr<Config> config;
		extern std::unique_ptr<Gui> gui;
		extern std::unique_ptr<cstrike_t> cstrike;
		extern std::unique_ptr<GameData> gameData;
		extern Uint32 keys[KEYS_LENGTH];
		extern std::unique_ptr<VmtMgr> BaseClientDllVmt;
		extern std::unique_ptr<VmtMgr> ModelRenderVmt;
	}

	namespace Hacks {
		void Bunnyhop(CUserCmd *cmd);
		void AutoStrafe(CUserCmd *cmd);
		bool Chams(IVModelRender *pModelRender, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
		void EspSnapline(PlayerData *player);
		void Triggerbot(CUserCmd *cmd);
		void EspBox(PlayerData *player);
		void RCS(CUserCmd *cmd);
		void EspSkeleton(PlayerData *player);
		void AimbotCheckPlayer(PlayerData *player);
		void Aimbot(CUserCmd *cmd);
	}

	namespace Hooks {
		void SDLCALL SDL_GL_SwapWindow(SDL_Window *window);
		int SDLCALL SDL_PollEvent(SDL_Event *event);
		void CreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);
		void DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
		void FrameStageNotify(IBaseClientDLL *thisptr, ClientFrameStage_t curStage);
	}
}

#define RONIX_LOG Ronix::Data::logger->Log
#define RONIX_ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
