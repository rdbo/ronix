#include "gui.hpp"
#include <ronix.hpp>

using namespace Ronix::Data;

void Gui::Setup()
{

}

int ConfigNameCallback(ImGuiInputTextCallbackData *data)
{
	switch (data->EventChar) {
	case L'/':
	case L'\\':
	case L'-':
		data->EventChar = 0;
		return 1;
	}

	return 0;
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
	ImGui::Separator();
	static char config_name[64] = { 0 };
	ImGui::InputText("Config Name", config_name, sizeof(config_name), 0, ConfigNameCallback);
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
