#include <ronix.hpp>

using namespace Ronix::Data;

void Ronix::Hacks::EspSnapline(PlayerData *player)
{
	if (!config->data.espSnaplineEnable || (config->data.espSnaplineHoldKey != SDL_SCANCODE_UNKNOWN && keys[config->data.espSnaplineHoldKey] != SDL_KEYDOWN) || player->behind || player->team <= TEAM_SPECTATOR || gameData->localPlayerTeam <= TEAM_SPECTATOR)
		return;

	auto draw_list = ImGui::GetBackgroundDrawList();
	ImVec2 point0 = ImVec2(0.0f, 0.0f);
	ImVec2 point1 = ImVec2(0.0f, 0.0f);

	switch (config->data.espSnaplinePos) {
	case ConfigData::ESP_SNAPLINE_POS_TOP:
		point0 = ImVec2(static_cast<float>(gameData->screenRes[0]) / 2.0f, 0.0f);
		break;
	case ConfigData::ESP_SNAPLINE_POS_CENTER:
		point0 = ImVec2(static_cast<float>(gameData->screenRes[0]) / 2.0f, static_cast<float>(gameData->screenRes[1]) / 2.0f);
		break;
	case ConfigData::ESP_SNAPLINE_POS_BOTTOM:
		point0 = ImVec2(static_cast<float>(gameData->screenRes[0]) / 2.0f, static_cast<float>(gameData->screenRes[1]));
		break;
	}

	switch (config->data.espSnaplineType) {
	case ConfigData::ESP_SNAPLINE_TYPE_HEAD:
		point1 = ImVec2(player->headpos2d.x, player->headpos2d.y);	
		break;
	case ConfigData::ESP_SNAPLINE_TYPE_FOOT:
		point1 = ImVec2(player->pos2d.x, player->pos2d.y);
		break;
	}
	
	draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineOutlineColor[0], config->data.espSnaplineOutlineColor[1], config->data.espSnaplineOutlineColor[2], config->data.espSnaplineOutlineColor[3]), 2 * config->data.espSnaplineThickness);

	if (player->team == gameData->localPlayerTeam) {
		if (player->isVisible)
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineTeamVisColor[0], config->data.espSnaplineTeamVisColor[1], config->data.espSnaplineTeamVisColor[2], config->data.espSnaplineTeamVisColor[3]), config->data.espSnaplineThickness);
		else
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineTeamInvisColor[0], config->data.espSnaplineTeamInvisColor[1], config->data.espSnaplineTeamInvisColor[2], config->data.espSnaplineTeamInvisColor[3]), config->data.espSnaplineThickness);
	} else {
		if (player->isVisible)
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineEnemyVisColor[0], config->data.espSnaplineEnemyVisColor[1], config->data.espSnaplineEnemyVisColor[2], config->data.espSnaplineEnemyVisColor[3]), config->data.espSnaplineThickness);
		else
			draw_list->AddLine(point0, point1, ImColor(config->data.espSnaplineEnemyInvisColor[0], config->data.espSnaplineEnemyInvisColor[1], config->data.espSnaplineEnemyInvisColor[2], config->data.espSnaplineEnemyInvisColor[3]), config->data.espSnaplineThickness);
	}
}
