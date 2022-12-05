#include <ronix.hpp>

using namespace Ronix::Data;

void Ronix::Hacks::EspBox(PlayerData *player)
{
	if (!config->data.espBoxEnable || (!config->data.espBoxHoldKey.IsPressed() && config->data.espBoxHoldKey.IsSet()) || player->behind || player->team <= TEAM_SPECTATOR || gameData->localPlayerTeam <= TEAM_SPECTATOR)
		return;

	auto draw_list = ImGui::GetBackgroundDrawList();
	ImVec2 point0 = ImVec2(player->headpos2d.x, player->headpos2d.y);
	ImVec2 point1 = ImVec2(player->pos2d.x, player->pos2d.y);
	auto height = std::fabs(point0.y - point1.y);
	point0.x -= height / 4.0f;
	point1.x += height / 4.0f;
	
	draw_list->AddRect(point0, point1, ImColor(config->data.espBoxOutlineColor[0], config->data.espBoxOutlineColor[1], config->data.espBoxOutlineColor[2], config->data.espBoxOutlineColor[3]), 0.0f, 0, 2 * config->data.espBoxThickness);

	if (player->team == gameData->localPlayerTeam) {
		if (player->isVisible)
			draw_list->AddRect(point0, point1, ImColor(config->data.espBoxTeamVisColor[0], config->data.espBoxTeamVisColor[1], config->data.espBoxTeamVisColor[2], config->data.espBoxTeamVisColor[3]), 0.0f, 0, config->data.espBoxThickness);
		else
			draw_list->AddRect(point0, point1, ImColor(config->data.espBoxTeamInvisColor[0], config->data.espBoxTeamInvisColor[1], config->data.espBoxTeamInvisColor[2], config->data.espBoxTeamInvisColor[3]), 0.0f, 0, config->data.espBoxThickness);
	} else {
		if (player->isVisible)
			draw_list->AddRect(point0, point1, ImColor(config->data.espBoxEnemyVisColor[0], config->data.espBoxEnemyVisColor[1], config->data.espBoxEnemyVisColor[2], config->data.espBoxEnemyVisColor[3]), 0.0f, 0, config->data.espBoxThickness);
		else
			draw_list->AddRect(point0, point1, ImColor(config->data.espBoxEnemyInvisColor[0], config->data.espBoxEnemyInvisColor[1], config->data.espBoxEnemyInvisColor[2], config->data.espBoxEnemyInvisColor[3]), 0.0f, 0, config->data.espBoxThickness);
	}
}
