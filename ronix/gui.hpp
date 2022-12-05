#ifndef GUI_HPP
#define GUI_HPP

#include <pch.hpp>
#include "config.hpp"

class Gui {
private:
	bool visible = false;
	Keybind *keyListen = nullptr;
public:
	void Setup();
	void Render();
	bool IsVisible();
	void SetVisiblity(bool state);
	void ToggleVisibility();
	Keybind *GetKeyListen();
	void SetKeyListen(Keybind *key);
};

#endif
