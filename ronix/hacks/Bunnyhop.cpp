#include <ronix.hpp>

using namespace Ronix::Data;

void Ronix::Hacks::Bunnyhop(CUserCmd *cmd)
{
	if (!config->data.bunnyhopEnable)
		return;
	
	if ((cmd->buttons & IN_JUMP) && !(cstrike->LocalPlayer->GetFlags() & FL_ONGROUND) && cstrike->LocalPlayer->GetMoveType() != MoveType_t::MOVETYPE_LADDER)
		cmd->buttons &= ~IN_JUMP;
}
