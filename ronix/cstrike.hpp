#ifndef GAME_HPP
#define GAME_HPP

#include <pch.hpp>

struct cstrike_t {
	ILauncherMgr *LauncherMgr;
	IClientEntityList *EntityList;
	CGameMovement *GameMovement;
	IBaseClientDLL *BaseClientDll;
	CInput *Input;
	CGlobalVarsBase *GlobalVars;
	IVEngineClient *EngineClient;
	IEngineTrace *EngineTrace;
	IVModelInfoClient *ModelInfoClient;
	IVModelRender *ModelRender;
	IVRenderView *RenderView;
	ICvar *Cvar;
	IMaterialSystem *MaterialSystem;
	CBasePlayer *LocalPlayer;
	IClientMode *ClientMode;
};

#endif
