#include <ronix.hpp>

using namespace Ronix::Data;

static PlayerData *targetPlayer = nullptr;

QAngle CalcAngles(Vector &origin, Vector &dest)
{
	QAngle ang = QAngle(0.0f, 0.0f, 0.0f);
	Vector diff = dest - origin;
	ang.x = RAD2DEG(std::atan2(-diff.z, std::hypot(diff.x, diff.y)));
	ang.y = RAD2DEG(std::atan2(diff.y, diff.x));
	return ang;
}

void Ronix::Hacks::AimbotCheckPlayer(PlayerData *player)
{
	if (player->team != cstrike->LocalPlayer->GetTeamNumber())
		targetPlayer = player;
}

void Ronix::Hacks::Aimbot(CUserCmd *cmd)
{
	if (targetPlayer == nullptr)
		return;

	Vector eyePos = cstrike->LocalPlayer->EyePosition();
	Vector bonePos = Vector(
		targetPlayer->boneMatrix[PlayerData::BoneIndex::NECK][0][3],
		targetPlayer->boneMatrix[PlayerData::BoneIndex::NECK][1][3],
		targetPlayer->boneMatrix[PlayerData::BoneIndex::NECK][2][3]
	);
	cmd->viewangles = CalcAngles(eyePos, bonePos);
	NormalizeAngles(cmd->viewangles);
	cstrike->EngineClient->SetViewAngles(cmd->viewangles);

	targetPlayer = nullptr;
}
