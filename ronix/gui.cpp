#include "gui.hpp"
#include <ronix.hpp>

void Gui::Setup()
{

}

void Gui::Render()
{
	if (!this->IsVisible())
		return;
	
	ImGui::Begin("Ronix");
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
