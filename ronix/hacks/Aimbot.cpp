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

float DistFromCrosshair(Vector &pos2d)
{
	float distX = std::abs((gameData->screenRes[0] / 2) - pos2d.x);
	float distY = std::abs((gameData->screenRes[1] / 2) - pos2d.y);
	return std::sqrt(std::pow(distX, 2.0f) + std::pow(distY, 2.0f));
}

void Ronix::Hacks::AimbotCheckPlayer(PlayerData *player)
{
	if (player->team == cstrike->LocalPlayer->GetTeamNumber() || !player->isVisible)
		return;

	if (config->data.aimbotType == ConfigData::AIMBOT_CROSSHAIR_CLOSEST) {
		if (player->behind)
			return;

		if (targetPlayer == nullptr)
			targetPlayer = player;
		else if (DistFromCrosshair(player->boneMatrixPos2d[PlayerData::BoneIndex::NECK]) < DistFromCrosshair(targetPlayer->boneMatrixPos2d[PlayerData::BoneIndex::NECK]))
			targetPlayer = player;
	} else {
		if (targetPlayer == nullptr)
			targetPlayer = player;
		else if (player->headpos3d.DistTo(cstrike->LocalPlayer->EyePosition()) < targetPlayer->headpos3d.DistTo(cstrike->LocalPlayer->EyePosition()))
			targetPlayer = player;
	}
}

void Ronix::Hacks::Aimbot(CUserCmd *cmd)
{
	if (!config->data.aimbotEnable || (!config->data.aimbotHoldKey.IsPressed() && config->data.aimbotHoldKey.IsSet()) || targetPlayer == nullptr)
		return;

	Vector eyePos = cstrike->LocalPlayer->EyePosition();
	Vector bonePos = Vector(
		targetPlayer->boneMatrix[PlayerData::BoneIndex::NECK][0][3],
		targetPlayer->boneMatrix[PlayerData::BoneIndex::NECK][1][3],
		targetPlayer->boneMatrix[PlayerData::BoneIndex::NECK][2][3]
	);
	QAngle desiredAngle = CalcAngles(eyePos, bonePos);
	QAngle deltaAngle = desiredAngle - cmd->viewangles;
	NormalizeAngles(deltaAngle);
	deltaAngle /= config->data.aimbotSmooth;
	cmd->viewangles += deltaAngle;
	NormalizeAngles(cmd->viewangles);
	if (!config->data.aimbotSilent)
		cstrike->EngineClient->SetViewAngles(cmd->viewangles);

	targetPlayer = nullptr;
}
