#include "imgui_ext.hpp"

using namespace Ronix::Data;

void ImGui::Hotkey(const char *label, Keybind *keybind)
{
	ImGui::PushID(label);
	std::string label_text = std::string(label);
	auto pos = label_text.find('#');
	if (pos != label_text.npos)
		label_text.resize(pos);
	ImGui::Text(label_text.c_str());
	ImGui::SameLine();

	std::stringstream ss;
	if (gui->GetKeyListen() == keybind) {
		ss << "...";
	} else if (keybind->key == SDL_SCANCODE_UNKNOWN) {
		ss << "None";
	} else {
		ss << (keybind->type == Keybind::KEYBOARD ? "K" : "M");
		ss << static_cast<int>(keybind->key);
	}
	
	if (ImGui::Button(ss.str().c_str()))
		gui->SetKeyListen(keybind);
	ImGui::SameLine();
	if (ImGui::Button("X")) {
		keybind->key = SDL_SCANCODE_UNKNOWN;
		gui->SetKeyListen(nullptr);
	}

	ImGui::PopID();
}
