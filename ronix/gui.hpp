#ifndef GUI_HPP
#define GUI_HPP

#include <pch.hpp>

class Gui {
private:
	bool visible = false;
	SDL_Scancode *keyListen = nullptr;
public:
	void Setup();
	void Render();
	bool IsVisible();
	void SetVisiblity(bool state);
	void ToggleVisibility();
	SDL_Scancode *GetKeyListen();
	void SetKeyListen(SDL_Scancode *key);
};

#endif
