#include <ronix.hpp>

using namespace Ronix::Data;

#define _B(b) PlayerData::BoneIndex::b

void Ronix::Hacks::EspSkeleton(PlayerData *player)
{
	if (!config->data.espSkelEnable || (!config->data.espSkelHoldKey.IsPressed() && config->data.espSkelHoldKey.IsSet()) || !player->isBoneMatrixSet)
		return;

	auto draw_list = ImGui::GetBackgroundDrawList();
	float *drawColorBuf;
	if (player->team == gameData->localPlayerTeam) {
		if (player->isVisible)
			drawColorBuf = config->data.espSkelTeamVisColor;
		else
			drawColorBuf = config->data.espSkelTeamInvisColor;
	} else {
		if (player->isVisible)
			drawColorBuf = config->data.espSkelEnemyVisColor;
		else
			drawColorBuf = config->data.espSkelEnemyInvisColor;
	}

	ImColor drawColor = ImColor(drawColorBuf[0], drawColorBuf[1], drawColorBuf[2], drawColorBuf[3]);

	ImVec2 headDotPoint = ImVec2(
		player->boneMatrixPos2d[_B(NECK)].x,
		player->boneMatrixPos2d[_B(NECK)].y
	);
	float headDotRadius = (1.0f * config->data.espSkelThickness) / (player->headpos3d.DistTo(cstrike->LocalPlayer->m_vecOrigin) / 512.0f);
	draw_list->AddCircleFilled(headDotPoint, headDotRadius, drawColor);

	static const std::vector<std::vector<int>> boneDrawList = {
		{ _B(NECK), _B(CHEST) },
		{ _B(CHEST), _B(LSHOULDER), _B(LELBOW), _B(LHAND) },
		{ _B(CHEST), _B(RSHOULDER), _B(RELBOW), _B(RHAND) },
		{ _B(CHEST), _B(CORE) },
		{ _B(CORE), _B(LPELVIS), _B(LKNEE), _B(LANKLE), _B(LFOOT) },
		{ _B(CORE), _B(RPELVIS), _B(RKNEE), _B(RANKLE), _B(RFOOT) }
	};

	ImVec2 points[2];
	for (size_t i = 0; i < boneDrawList.size(); ++i) {
		for (size_t j = 0; j < boneDrawList[i].size() - 1; ++j) {
			for (size_t k = 0; k < 2; ++k) {
				Vector &bonePos = player->boneMatrixPos2d[boneDrawList[i][j + k]];
				points[k] = ImVec2(bonePos.x, bonePos.y);
			}

			draw_list->AddLine(points[0], points[1], drawColor, config->data.espSkelThickness);
		}
	}
}
