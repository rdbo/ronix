#include "imgui_ext.hpp"

using namespace Ronix::Data;

void ImGui::Hotkey(const char *label, SDL_Keycode *key)
{
	ImGui::Text(label);
	ImGui::SameLine();

	std::stringstream ss;
	if (gui->GetKeyListen() == key)
		ss << "...";
	else if (*key == SDLK_UNKNOWN)
		ss << "None";
	else
		ss << static_cast<int>(*key);
	
	if (ImGui::Button(ss.str().c_str()))
		gui->SetKeyListen(key);
	ImGui::SameLine();
	if (ImGui::Button("X")) {
		*key = SDLK_UNKNOWN;
		gui->SetKeyListen(nullptr);
	}
}
