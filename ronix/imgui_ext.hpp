#ifndef IMGUI_EXT_HPP
#define IMGUI_EXT_HPP

#include <ronix.hpp>

namespace ImGui {
	void Hotkey(const char *label, SDL_Scancode *key);
	void KeyType(const char *label, ConfigData::KeyType *keytype);
}

#endif
