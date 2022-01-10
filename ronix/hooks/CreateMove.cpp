#include <ronix.hpp>

using namespace Ronix::Data;

typedef void (*CreateMoveFn)(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);

void Ronix::Hooks::CreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active)
{
	static CreateMoveFn fnCreateMove = Ronix::Data::BaseClientDllVmt->GetOriginal<CreateMoveFn>(21);
	fnCreateMove(thisptr, sequence_number, input_sample_frametime, active);

	cstrike->LocalPlayer = (CBasePlayer *)cstrike->EntityList->GetClientEntity(cstrike->EngineClient->GetLocalPlayer());
	if (!cstrike->LocalPlayer)
		return;
	
	CUserCmd *cmd = cstrike->Input->GetUserCmd(sequence_number);

	// Update GameData
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
		gameData->players[i].team = player->GetTeamNumber();
		gameData->players[i].pos3d = player->GetAbsOrigin();
		gameData->players[i].behind = FrustumTransform(cstrike->EngineClient->WorldToScreenMatrix(), gameData->players[i].pos3d, gameData->players[i].pos2d);
		gameData->players[i].pos2d.x *= gameData->screenRes[0] / 2.0f;
		gameData->players[i].pos2d.x += gameData->screenRes[0] / 2.0f;
		gameData->players[i].pos2d.y *= -1.0f;
		gameData->players[i].pos2d.y *= gameData->screenRes[1] / 2.0f;
		gameData->players[i].pos2d.y += gameData->screenRes[1] / 2.0f;
	}

	// Run Hacks
	Ronix::Hacks::AutoStrafe(cmd);
	Ronix::Hacks::Bunnyhop(cmd);

	// Normalize
	NormalizeAngles(cmd->viewangles);

	// Verify UserCMD
	CInput::CVerifiedUserCmd *verified_cmd = reinterpret_cast<CInput::CVerifiedUserCmd *>(&cstrike->Input->m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP]);
	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();
}
