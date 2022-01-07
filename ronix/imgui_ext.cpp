#include "imgui_ext.hpp"

using namespace Ronix::Data;

void ImGui::Hotkey(const char *label, SDL_Scancode *key)
{
	ImGui::Text(label);
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
}

void ImGui::KeyType(const char *label, ConfigData::KeyType *keytype)
{
	static const char *items[ConfigData::KEYTYPE_INVAL] = { "Hold", "Toggle" };
	ImGui::ListBox(label, reinterpret_cast<int *>(keytype), items, ConfigData::KEYTYPE_INVAL);
}
