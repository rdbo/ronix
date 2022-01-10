#include <ronix.hpp>

using namespace Ronix::Data;

void Ronix::Hacks::EspSnapline(PlayerData *player)
{
	if (!config->data.espSnaplineEnable || player->behind)
		return;
	
	auto draw_list = ImGui::GetBackgroundDrawList();
	ImVec2 point0;
	ImVec2 point1;

	switch (config->data.espSnaplinePos) {
	case ConfigData::ESP_SNAPLINE_TOP:
		point0 = ImVec2(static_cast<float>(gameData->screenRes[0]) / 2.0f, 0.0f);
		break;
	case ConfigData::ESP_SNAPLINE_CENTER:
		point0 = ImVec2(static_cast<float>(gameData->screenRes[0]) / 2.0f, static_cast<float>(gameData->screenRes[1]) / 2.0f);
		break;
	case ConfigData::ESP_SNAPLINE_BOTTOM:
		point0 = ImVec2(static_cast<float>(gameData->screenRes[0]) / 2.0f, static_cast<float>(gameData->screenRes[1]));
		break;
	}

	point1 = ImVec2(player->pos2d.x, player->pos2d.y);

	if (player->team == gameData->localPlayerTeam)
		draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineColorTeam[0], config->data.espSnaplineColorTeam[1], config->data.espSnaplineColorTeam[2], config->data.espSnaplineColorTeam[3]));
	else
		draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineColorEnemy[0], config->data.espSnaplineColorEnemy[1], config->data.espSnaplineColorEnemy[2], config->data.espSnaplineColorEnemy[3]));
}
