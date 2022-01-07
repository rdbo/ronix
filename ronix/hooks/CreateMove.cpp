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
