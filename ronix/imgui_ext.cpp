#include "imgui_ext.hpp"

using namespace Ronix::Data;

void ImGui::Hotkey(const char *label, SDL_Scancode *key)
{
	ImGui::PushID(label);
	std::string label_text = std::string(label);
	auto pos = label_text.find('#');
	if (pos != label_text.npos)
		label_text.resize(pos);
	ImGui::Text(label_text.c_str());
	ImGui::SameLine();

	std::stringstream ss;
	if (gui->GetKeyListen() == key)
		ss << "...";
	else if (*key == SDL_SCANCODE_UNKNOWN)
		ss << "None";
	else
		ss << static_cast<int>(*key);
	
	if (ImGui::Button(ss.str().c_str()))
		gui->SetKeyListen(key);
	ImGui::SameLine();
	if (ImGui::Button("X")) {
		*key = SDL_SCANCODE_UNKNOWN;
		gui->SetKeyListen(nullptr);
	}

	ImGui::PopID();
}
