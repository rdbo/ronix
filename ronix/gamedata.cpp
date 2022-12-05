#include "gamedata.hpp"
#include <ronix.hpp>

using namespace Ronix::Data;

void FixPos2d(Vector &vec)
{
	vec.x *= gameData->screenRes[0] / 2.0f;
	vec.x += gameData->screenRes[0] / 2.0f;
	vec.y *= -1.0f;
	vec.y *= gameData->screenRes[1] / 2.0f;
	vec.y += gameData->screenRes[1] / 2.0f;
}

void GameData::Update()
{
	gameData->localPlayerTeam = cstrike->LocalPlayer->GetTeamNumber();
	cstrike->LauncherMgr->DisplayedSize(gameData->screenRes[0], gameData->screenRes[1]);
	gameData->players.resize(cstrike->EngineClient->GetMaxClients() + 1);
	for (int i = 1; i < gameData->players.size(); ++i) {
		CBasePlayer *player = reinterpret_cast<CBasePlayer *>(cstrike->EntityList->GetClientEntity(i));
		if (!player || player == cstrike->LocalPlayer || player->IsDormant() || !player->IsAlive() || player->GetHealth() <= 0) {
			gameData->players[i].isValid = false;
			continue;
		}

		gameData->players[i].isValid = true;

		Ray_t ray;
		trace_t trace;
		ray.Init(cstrike->LocalPlayer->EyePosition(), player->GetAbsOrigin());
		cstrike->EngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | MASK_SHOT, nullptr, &trace);
		gameData->players[i].isVisible = trace.m_pEnt == reinterpret_cast<CBaseEntity *>(player) || trace.fraction == 1.0f;
		if (!gameData->players[i].isVisible) {
			ray.Init(cstrike->LocalPlayer->EyePosition(), player->EyePosition());
			cstrike->EngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | MASK_SHOT, nullptr, &trace);
			gameData->players[i].isVisible = trace.m_pEnt == reinterpret_cast<CBaseEntity *>(player) || trace.fraction == 1.0f;
		}

		gameData->players[i].team = player->GetTeamNumber();
		gameData->players[i].pos3d = player->GetAbsOrigin();
		gameData->players[i].headpos3d = player->EyePosition();
		gameData->players[i].behind = FrustumTransform(cstrike->EngineClient->WorldToScreenMatrix(), gameData->players[i].pos3d, gameData->players[i].pos2d);
		FrustumTransform(cstrike->EngineClient->WorldToScreenMatrix(), gameData->players[i].headpos3d, gameData->players[i].headpos2d);
	
		FixPos2d(gameData->players[i].pos2d);
		FixPos2d(gameData->players[i].headpos2d);
		
		gameData->players[i].isBoneMatrixSet = player->SetupBones(gameData->players[i].boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f);
		for (int j = 0; j < MAXSTUDIOBONES; ++j) {
			Vector origin = Vector( 
				gameData->players[i].boneMatrix[j][0][3],
				gameData->players[i].boneMatrix[j][1][3],
				gameData->players[i].boneMatrix[j][2][3]
			);
			FrustumTransform(cstrike->EngineClient->WorldToScreenMatrix(), origin, gameData->players[i].boneMatrixPos2d[j]);
			FixPos2d(gameData->players[i].boneMatrixPos2d[j]);
		}
	}
}
