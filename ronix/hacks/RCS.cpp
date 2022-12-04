#include <ronix.hpp>

using Ronix::Data::cstrike;

static QAngle old_punch = QAngle(0, 0, 0);

uintptr_t GetVecPunchOffset()
{
	static ClientClass *base_class = cstrike->BaseClientDll->GetAllClasses();
	static RecvProp *m_Local = GetNetVarProp(base_class, "DT_BasePlayer", "m_Local");
	static RecvProp *m_vecPunchAngle = GetNetVarProp(base_class, "DT_BasePlayer", "m_vecPunchAngle");
	if (!m_Local) {
		RONIX_LOG("Unable to find m_Local prop");
		return 0;
	}

	if (!m_vecPunchAngle) {
		RONIX_LOG("Unable to find m_vecPunchAngle prop\n");
		return 0;
	}

	return (uintptr_t)(m_Local->GetOffset() + m_vecPunchAngle->GetOffset());
}

void Ronix::Hacks::RCS(CUserCmd *cmd)
{
	static uintptr_t punch_offset = GetVecPunchOffset();
	if (!punch_offset)
		return;
	QAngle *punch = (QAngle *)(&((char *)cstrike->LocalPlayer)[punch_offset]);
	cmd->viewangles.x -= (punch->x - old_punch.x) * 1.97f;
	cmd->viewangles.y -= (punch->y - old_punch.y) * 1.97f;
	old_punch = *punch;
	cstrike->EngineClient->SetViewAngles(cmd->viewangles);
}
