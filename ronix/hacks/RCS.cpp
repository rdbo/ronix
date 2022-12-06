#include <ronix.hpp>

using namespace Ronix::Data;

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

	if (!config->data.rcsEnable || (!config->data.rcsHoldKey.IsPressed() && config->data.rcsHoldKey.IsSet())) {
		return;
	}

	if (shotsFired > 1) {
		QAngle *punch = (QAngle *)(&((char *)cstrike->LocalPlayer)[punch_offset]);
		QAngle viewangles;
		cstrike->EngineClient->GetViewAngles(viewangles);
		viewangles.x -= (punch->x - old_punch.x) * 2.0f;
		viewangles.y -= (punch->y - old_punch.y) * 2.0f;
		NormalizeAngles(viewangles);
		old_punch = *punch;
		cstrike->EngineClient->SetViewAngles(viewangles);
	} else {
		old_punch = QAngle(0.0f, 0.0f, 0.0f);
	}
}
