#include <ronix.hpp>

using namespace Ronix::Data;

void Ronix::Hacks::EspSnapline(PlayerData *player)
{
	if (!config->data.espSnaplineEnable || (config->data.espSnaplineHoldKey != SDL_SCANCODE_UNKNOWN && keys[config->data.espSnaplineHoldKey] != SDL_KEYDOWN) || player->behind || player->team <= TEAM_SPECTATOR || gameData->localPlayerTeam <= TEAM_SPECTATOR)
		return;

	static const float thickness = 3.0f;
	static const float outline_thickness = 1.0f;
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
	
	draw_list->AddLine(point0, point1, ImColor(0.0f, 0.0f, 0.0f, 1.0f), thickness + 2 * outline_thickness);

	if (player->team == gameData->localPlayerTeam) {
		if (player->isVisible)
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineTeamVisColor[0], config->data.espSnaplineTeamVisColor[1], config->data.espSnaplineTeamVisColor[2], config->data.espSnaplineTeamVisColor[3]), thickness);
		else
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineTeamInvisColor[0], config->data.espSnaplineTeamInvisColor[1], config->data.espSnaplineTeamInvisColor[2], config->data.espSnaplineTeamInvisColor[3]), thickness);
	} else {
		if (player->isVisible)
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineEnemyVisColor[0], config->data.espSnaplineEnemyVisColor[1], config->data.espSnaplineEnemyVisColor[2], config->data.espSnaplineEnemyVisColor[3]), thickness);
		else
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineEnemyInvisColor[0], config->data.espSnaplineEnemyInvisColor[1], config->data.espSnaplineEnemyInvisColor[2], config->data.espSnaplineEnemyInvisColor[3]), thickness);
	}
}
