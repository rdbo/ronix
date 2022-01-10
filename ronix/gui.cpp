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
	ImGui::Checkbox("AutoStrafe", &data->autoStrafeEnable);
	ImGui::Checkbox("Silent##AutoStrafe", &data->autoStrafeSilent);
	ImGui::Checkbox("Rage##AutoStrafe", &data->autoStrafeRage);
	ImGui::Hotkey("Toggle Key##AutoStrafe", &data->autoStrafeToggleKey);
	ImGui::Hotkey("Hold Key##AutoStrafe", &data->autoStrafeHoldKey);

	ImGui::Separator();

	static const char *esp_snapline_pos_list[ConfigData::ESP_SNAPLINE_INVAL] = { "Top", "Center", "Bottom" };
	ImGui::Checkbox("ESP Snapline", &data->espSnaplineEnable);
	ImGui::ListBox("Position##ESP Snapline", reinterpret_cast<int *>(&data->espSnaplinePos), esp_snapline_pos_list, ConfigData::ESP_SNAPLINE_INVAL);
	ImGui::ColorEdit4("Color Team##ESP Snapline", data->espSnaplineColorTeam);
	ImGui::ColorEdit4("Color Enemy##ESP Snapline", data->espSnaplineColorEnemy);

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
	ImGui::Separator();

	ImGui::ColorEdit4("Material Color##Chams", data->chamsData[chams_type][chams_num].color);

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

SDL_Scancode *Gui::GetKeyListen()
{
	return this->keyListen;
}

void Gui::SetKeyListen(SDL_Scancode *key)
{
	this->keyListen = key;
}
