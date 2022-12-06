#include <ronix.hpp>

using namespace Ronix::Data;

// #define DEBUG cstrike->Cvar->ConsolePrintf
// #define OFFSET(ptr, offset) (&((char *)ptr)[offset])

static QAngle old_punch = QAngle(0.0f, 0.0f, 0.0f);

static uintptr_t GetVecPunchOffset()
{
	static ClientClass *base_class = cstrike->BaseClientDll->GetAllClasses();
	static RecvProp *m_Local = GetNetVarProp(base_class, "DT_BasePlayer", "m_Local");
	static RecvProp *m_vecPunchAngle = GetNetVarProp(base_class, "DT_BasePlayer", "m_vecPunchAngle");
	if (!m_Local) {
		RONIX_LOG("Unable to find m_Local prop\n");
		return 0;
	}

	if (!m_vecPunchAngle) {
		RONIX_LOG("Unable to find m_vecPunchAngle prop\n");
		return 0;
	}

	return (uintptr_t)(m_Local->GetOffset() + m_vecPunchAngle->GetOffset());
}

static uintptr_t GetShotsFiredOffset()
{
	static ClientClass *base_class = cstrike->BaseClientDll->GetAllClasses();
	static RecvProp *m_iShotsFired = GetNetVarProp(base_class, "DT_CSPlayer", "m_iShotsFired");
	if (!m_iShotsFired) {
		RONIX_LOG("Unable to find m_iShotsFired prop\n");
		return 0;
	}

	return (uintptr_t)m_iShotsFired->GetOffset();
}

void Ronix::Hacks::RCS(CUserCmd *cmd)
{
	static uintptr_t punch_offset = GetVecPunchOffset();
	static uintptr_t shots_fired_offset = GetShotsFiredOffset();
	if (!punch_offset || !shots_fired_offset)
		return;

	int shotsFired = *(int *)(&((char *)cstrike->LocalPlayer)[shots_fired_offset]);
	cstrike->Cvar->ConsolePrintf("[RONIX] Shots Fired: %d\n", shotsFired);

	if (!config->data.rcsEnable || !(cmd->buttons & IN_ATTACK) || shotsFired <= 1 || (!config->data.rcsHoldKey.IsPressed() && config->data.rcsHoldKey.IsSet())) {
		old_punch = QAngle(0.0f, 0.0f, 0.0f);
		return;
	}

	QAngle *punch = (QAngle *)(&((char *)cstrike->LocalPlayer)[punch_offset]);

	// debug (searching for bad padding in CBasePlayer)
	/*
	static ClientClass *base_class = cstrike->BaseClientDll->GetAllClasses();
	static RecvProp *m_iHealth = GetNetVarProp(base_class, "DT_BasePlayer", "m_iHealth");
	static RecvProp *m_iAmmo = GetNetVarProp(base_class, "DT_BasePlayer", "m_iAmmo");
	static RecvProp *m_flModelScale = GetNetVarProp(base_class, "DT_BasePlayer", "m_flModelScale");
	static RecvProp *m_nMuzzleFlashParity = GetNetVarProp(base_class, "DT_BasePlayer", "m_nMuzzleFlashParity");
	static RecvProp *m_bSimulatedEveryTick = GetNetVarProp(base_class, "DT_BasePlayer", "m_bSimulatedEveryTick");

	DEBUG("[RONIX] Real m_iHealth: %p\n", OFFSET(cstrike->LocalPlayer, m_iHealth->GetOffset()));
	DEBUG("[RONIX] m_iHealth:      %p\n", &cstrike->LocalPlayer->m_iHealth);
	DEBUG("[RONIX] Real m_iAmmo:   %p\n", OFFSET(cstrike->LocalPlayer, m_iAmmo->GetOffset()));
	DEBUG("[RONIX] m_iAmmo:        %p\n", &cstrike->LocalPlayer->m_iAmmo);
	DEBUG("[RONIX] Real m_flModelScale: %p\n", OFFSET(cstrike->LocalPlayer, m_flModelScale->GetOffset()));
	DEBUG("[RONIX] m_flModelScale: %p\n", &cstrike->LocalPlayer->m_flModelScale);
	DEBUG("[RONIX] Real m_nMuzzleFlashParity: %p\n", OFFSET(cstrike->LocalPlayer, m_nMuzzleFlashParity->GetOffset()));
	DEBUG("[RONIX] m_nMuzzleFlashParity: %p\n", &cstrike->LocalPlayer->m_nMuzzleFlashParity);
	DEBUG("[RONIX] Real m_bSimulatedEveryTick: %p\n", OFFSET(cstrike->LocalPlayer, m_bSimulatedEveryTick->GetOffset()));
	DEBUG("[RONIX] m_bSimulatedEveryTick: %p\n", &cstrike->LocalPlayer->m_bSimulatedEveryTick);
	*/

	QAngle viewangles;
	cstrike->EngineClient->GetViewAngles(viewangles);
	viewangles.x -= (punch->x - old_punch.x) * 2.0f;
	viewangles.y -= (punch->y - old_punch.y) * 2.0f;
	NormalizeAngles(viewangles);
	old_punch = *punch;
	cstrike->EngineClient->SetViewAngles(viewangles);
}
