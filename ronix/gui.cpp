#include "gui.hpp"
#include <ronix.hpp>
#include "imgui_ext.hpp"

using namespace Ronix::Data;

void Gui::Setup()
{

}

static int InputFilterCallback(ImGuiInputTextCallbackData *data)
{
	if (
		(data->EventChar >= L'a' && data->EventChar <= L'z') ||
		(data->EventChar >= L'A' && data->EventChar <= L'Z') ||
		(data->EventChar >= L'0' && data->EventChar <= L'9') ||
		data->EventChar == ' ' || data->EventChar == '_'
	) return 0;

	return 1;
}

void Gui::Render()
{
	if (!this->IsVisible())
		return;

	static auto data = &config->data;	
	ImGui::Begin("Ronix");
	ImGui::Checkbox("Bunnyhop", &data->bunnyhopEnable);
	ImGui::Separator();
	ImGui::Checkbox("Auto Strafe", &data->autoStrafeEnable);
	ImGui::Hotkey("Toggle Key##Auto Strafe", &data->autoStrafeToggleKey);
	ImGui::Hotkey("Hold Key##Auto Strafe", &data->autoStrafeHoldKey);
	ImGui::Checkbox("Silent##Auto Strafe", &data->autoStrafeSilent);
	ImGui::Checkbox("Rage##Auto Strafe", &data->autoStrafeRage);

	ImGui::Separator();

	ImGui::Checkbox("Triggerbot", &data->triggerbotEnable);
	ImGui::Hotkey("Toggle Key##Triggerbot", &data->triggerbotToggleKey);
	ImGui::Hotkey("Hold Key##Triggerbot", &data->triggerbotHoldKey);
	ImGui::SliderFloat("Delay (ms)", &data->triggerbotDelay, 0.0f, 200.0f, "%.0f", 1.0f);

	ImGui::Separator();

	static const char *esp_snapline_pos_list[ConfigData::ESP_SNAPLINE_POS_INVAL] = { "Top", "Center", "Bottom" };
	static const char *esp_snapline_type_list[ConfigData::ESP_SNAPLINE_TYPE_INVAL] = { "Head", "Foot" };

	ImGui::Checkbox("ESP Snapline", &data->espSnaplineEnable);
	ImGui::Hotkey("Toggle Key##ESP Snapline", &data->espSnaplineToggleKey);
	ImGui::Hotkey("Hold Key##ESP Snapline", &data->espSnaplineHoldKey);
	ImGui::ListBox("Position##ESP Snapline", reinterpret_cast<int *>(&data->espSnaplinePos), esp_snapline_pos_list, ConfigData::ESP_SNAPLINE_POS_INVAL);
	ImGui::ListBox("Type##ESP Snapline", reinterpret_cast<int *>(&data->espSnaplineType), esp_snapline_type_list, ConfigData::ESP_SNAPLINE_TYPE_INVAL);
	ImGui::SliderFloat("Thickness##ESP Snapline", &data->espSnaplineThickness, 0.0f, 10.0f, "%.0f", 1.0f);
	ImGui::ColorEdit4("Color Enemy Visible##ESP Snapline", data->espSnaplineEnemyVisColor);
	ImGui::ColorEdit4("Color Enemy Invisible##ESP Snapline", data->espSnaplineEnemyInvisColor);
	ImGui::ColorEdit4("Color Team Visible##ESP Snapline", data->espSnaplineTeamVisColor);
	ImGui::ColorEdit4("Color Team Invisible##ESP Snapline", data->espSnaplineTeamInvisColor);
	ImGui::ColorEdit4("Color Outline##ESP Snapline", data->espSnaplineOutlineColor);

	ImGui::Separator();

	ImGui::Checkbox("ESP Box", &data->espBoxEnable);
	ImGui::Hotkey("Toggle Key##ESP Box", &data->espBoxToggleKey);
	ImGui::Hotkey("Hold Key##ESP Box", &data->espBoxHoldKey);
	ImGui::SliderFloat("Thickness##ESP Box", &data->espBoxThickness, 0.0f, 10.0f, "%.0f", 1.0f);
	ImGui::ColorEdit4("Color Enemy Visible##ESP Box", data->espBoxEnemyVisColor);
	ImGui::ColorEdit4("Color Enemy Invisible##ESP Box", data->espBoxEnemyInvisColor);
	ImGui::ColorEdit4("Color Team Visible##ESP Box", data->espBoxTeamVisColor);
	ImGui::ColorEdit4("Color Team Invisible##ESP Box", data->espBoxTeamInvisColor);
	ImGui::ColorEdit4("Color Outline##ESP Box", data->espBoxOutlineColor);

	ImGui::Separator();

	static int chams_num = 0;
	static int chams_type = ConfigData::CHAMS_TYPE_ENEMIES;
	static const char *chams_type_list[ConfigData::CHAMS_TYPE_INVAL] = { "Enemies", "Allies", "Viewmodel", "Weapons", "Grenades", "C4" };
	ImGui::Checkbox("Chams", &data->chamsEnable);
	ImGui::Hotkey("Toggle Key##Chams", &data->chamsToggleKey);
	ImGui::Hotkey("Hold Key##Chams", &data->chamsHoldKey);
	if (ImGui::ListBox("Chams Type##Chams", &chams_type, chams_type_list, ConfigData::CHAMS_TYPE_INVAL))
		chams_num = 0;
	ImGui::Text("Current Chams Number: %d", chams_num + 1);
	ImGui::SameLine();
	if (ImGui::ArrowButton("-##Chams", ImGuiDir_Left) && chams_num > 0)
		--chams_num;
	ImGui::SameLine();
	if (ImGui::ArrowButton("+##Chams", ImGuiDir_Right) && (chams_num + 1) < CHAMS_COUNT)
		++chams_num;
	
	ImGui::Checkbox("Enable##Chams", &data->chamsData[chams_type][chams_num].enable);
	ImGui::Checkbox("Wireframe##Chams", &data->chamsData[chams_type][chams_num].wireframe);
	ImGui::Checkbox("Ignore Z##Chams", &data->chamsData[chams_type][chams_num].ignoreZ);
	ImGui::Checkbox("Self Illum##Chams", &data->chamsData[chams_type][chams_num].selfIllum);
	
	static const char *mat_type_list[ConfigData::CHAMS_MAT_INVAL] = { "Flat", "Plastic", "Glass" };
	ImGui::ListBox("Material Type##Chams", reinterpret_cast<int *>(&data->chamsData[chams_type][chams_num].mat), mat_type_list, ConfigData::CHAMS_MAT_INVAL);

	ImGui::ColorEdit4("Material Color##Chams", data->chamsData[chams_type][chams_num].color);

	ImGui::Separator();

	ImGui::Checkbox("Enable RCS##RCS", &data->rcsEnable);
	ImGui::Hotkey("Toggle Key##RCS", &data->rcsToggleKey);
	ImGui::Hotkey("Hold Key##RCS", &data->rcsHoldKey);

	ImGui::Separator();

	ImGui::Checkbox("ESP Skeleton", &data->espSkelEnable);
	ImGui::Hotkey("Toggle Key##ESP Skeleton", &data->espSkelToggleKey);
	ImGui::Hotkey("Hold Key##ESP Skeleton", &data->espSkelHoldKey);
	ImGui::SliderFloat("Thickness##ESP Skeleton", &data->espSkelThickness, 0.0f, 10.0f, "%.0f", 1.0f);
	ImGui::ColorEdit4("Color Enemy Visible##ESP Skeleton", data->espSkelEnemyVisColor);
	ImGui::ColorEdit4("Color Enemy Invisible##ESP Skeleton", data->espSkelEnemyInvisColor);
	ImGui::ColorEdit4("Color Team Visible##ESP Skeleton", data->espSkelTeamVisColor);
	ImGui::ColorEdit4("Color Team Invisible##ESP Skeleton", data->espSkelTeamInvisColor);

	ImGui::Separator();

	static char config_name[64] = { 0 };
	ImGui::InputText("Config Name", config_name, sizeof(config_name), ImGuiInputTextFlags_CallbackCharFilter, InputFilterCallback);
	if (ImGui::Button("Load Config"))
		config->Load(config_name);
	if (ImGui::Button("Save Config"))
		config->Save(config_name);
	if (ImGui::Button("Reset Config"))
		config->Reset();
	
	ImGui::Separator();

	if (ImGui::Button("Dump materials")) {
		MaterialHandle_t hmat;
		RONIX_LOG("Material List Start\n");
		hmat = cstrike->MaterialSystem->FirstMaterial();
		do {
			IMaterial *mat = cstrike->MaterialSystem->GetMaterial(hmat);
			RONIX_LOG("%s\n", mat->GetName());
		} while ((hmat = cstrike->MaterialSystem->NextMaterial(hmat)) != cstrike->MaterialSystem->InvalidMaterial());
		RONIX_LOG("Material Dump End\n");
	}

	ImGui::Separator();

	if (ImGui::Button("Unhook"))
		Ronix::Shutdown();
	ImGui::End();
}

bool Gui::IsVisible()
{
	return this->visible;
}

void Gui::SetVisiblity(bool state)
{
	this->visible = state;
}

void Gui::ToggleVisibility()
{
	this->SetVisiblity(!this->IsVisible());
}

Keybind *Gui::GetKeyListen()
{
	return this->keyListen;
}

void Gui::SetKeyListen(Keybind *key)
{
	this->keyListen = key;
}
