#ifndef GUI_HPP
#define GUI_HPP

#include <pch.hpp>

class Gui {
private:
	bool visible = false;
public:
	void Setup();
	void Render();
	bool IsVisible();
	void SetVisiblity(bool state);
	void ToggleVisibility();
};

#endif
