#include <ronix.hpp>

using namespace Ronix::Data;

bool IsAttacking(CUserCmd *cmd)
{
	auto active_wpn = cstrike->LocalPlayer->GetActiveWeapon();
	return (
		(active_wpn->Clip1() > 0 && (cmd->buttons & IN_ATTACK)) ||
		(!strcmp(active_wpn->GetName(), "weapon_knife") && (cmd->buttons & (IN_ATTACK | IN_ATTACK2)))
	);
}

void Ronix::Hacks::AutoStrafe(CUserCmd *cmd)
{
	if (!config->data.autoStrafeEnable || (!config->data.autoStrafeHoldKey.IsPressed() && config->data.autoStrafeHoldKey.IsSet()) || !(cmd->buttons & IN_JUMP))
		return;

	static ConVar *cl_sidespeed = cstrike->Cvar->FindVar("cl_sidespeed");
	static ConVar *cl_forwardspeed = cstrike->Cvar->FindVar("cl_forwardspeed");
	float velocity = cstrike->LocalPlayer->GetLocalVelocity().Length2D();

	if (!(cstrike->LocalPlayer->GetFlags() & FL_ONGROUND) && cstrike->LocalPlayer->GetMoveType() != MoveType_t::MOVETYPE_NOCLIP) {
		if (cmd->mousedx > 0 || ((cmd->buttons & IN_RIGHT) && !(cmd->buttons & IN_LEFT))) {
			cmd->sidemove = cl_sidespeed->GetFloat();
		} else if (cmd->mousedx < 0 || ((cmd->buttons & IN_LEFT) && !(cmd->buttons & IN_RIGHT))) {
			cmd->sidemove = -cl_sidespeed->GetFloat();
		} else if (config->data.autoStrafeRage && !IsAttacking(cmd)) {
			if (velocity <= 29.0f) {
				cmd->forwardmove = cl_forwardspeed->GetFloat();
			} else {
				static enum {
					STRAFE_NONE,
					STRAFE_LEFT,
					STRAFE_RIGHT
				} last_strafe = STRAFE_NONE;
				
				float strafe_ang = 0.0f;
				auto active_wpn = cstrike->LocalPlayer->GetActiveWeapon();
				float air_speed_cap = cstrike->GameMovement->GetAirSpeedCap();
				strafe_ang = air_speed_cap * std::fabs(air_speed_cap / velocity);

				if (last_strafe == STRAFE_LEFT) {
					cmd->viewangles.y += strafe_ang;
					cmd->sidemove = cl_sidespeed->GetFloat();
					last_strafe = STRAFE_RIGHT;
				} else {
					cmd->viewangles.y -= strafe_ang;
					cmd->sidemove = -cl_sidespeed->GetFloat();
					last_strafe = STRAFE_LEFT;
				}
			}
		}
		
		NormalizeAngles(cmd->viewangles);
		if (!config->data.autoStrafeSilent)
			cstrike->EngineClient->SetViewAngles(cmd->viewangles);
	}
}
