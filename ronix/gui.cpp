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
	ImGui::Checkbox("Silent", &data->autoStrafeSilent);
	ImGui::Hotkey("Key", &data->autoStrafeKey);
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

SDL_Keycode *Gui::GetKeyListen()
{
	return this->keyListen;
}

void Gui::SetKeyListen(SDL_Keycode *key)
{
	this->keyListen = key;
}
