#include <ronix.hpp>

using namespace Ronix::Data;

static bool hasAttacked = false;

class MyTraceFilter : public ITraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask)
	{
		return pEntity != pSkip;
	}

	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}

	IHandleEntity *pSkip;
};

void Ronix::Hacks::Triggerbot(CUserCmd *cmd)
{
	if (hasAttacked) {
		cmd->buttons &= ~IN_ATTACK;
		hasAttacked = false;
		return;
	}

	if (!config->data.triggerbotEnable || (!config->data.triggerbotHoldKey.IsPressed() && config->data.triggerbotHoldKey.IsSet()) || !cstrike->LocalPlayer->IsAlive() || !cstrike->LocalPlayer->GetHealth() > 0 || cstrike->LocalPlayer->GetTeamNumber() <= TEAM_SPECTATOR)
		return;
	
	Ray_t ray;
	trace_t trace;
	MyTraceFilter traceFilter;
	traceFilter.pSkip = cstrike->LocalPlayer;
	auto eye_pos = cstrike->LocalPlayer->EyePosition();
	auto eye_angles = cstrike->LocalPlayer->EyeAngles();
	Vector eye_forward;
	AngleVectors(eye_angles, &eye_forward);
	auto eye_end = eye_pos + (eye_forward * 8192.0f);

	ray.Init(eye_pos, eye_end);
	cstrike->EngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &trace);
	auto target = reinterpret_cast<CBasePlayer *>(trace.m_pEnt);
	
	if (target && target->IsPlayer() && target->IsAlive() && target->GetHealth() > 0 && target->GetTeamNumber() > TEAM_SPECTATOR && target->GetTeamNumber() != cstrike->LocalPlayer->GetTeamNumber()) {
		cmd->buttons |= IN_ATTACK;
		hasAttacked = true;
	}
}
