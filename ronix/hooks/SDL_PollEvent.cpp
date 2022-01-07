#include <ronix.hpp>

static int SDLCALL CustomPollEvent(SDL_Event *event)
{
	int result = ::SDL_PollEvent(event);
	if (result) {
		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_INSERT)
			Ronix::Data::gui->ToggleVisibility();
		
		if (Ronix::Data::gui->IsVisible())
			ImGui_ImplSDL2_ProcessEvent(event);
	}

	return result;
}

int SDLCALL Ronix::Hooks::SDL_PollEvent(SDL_Event *event)
{
	int result;
	do {
		result = CustomPollEvent(event);
	} while (Ronix::Data::gui->IsVisible() && result); // Don't poll events for the game when the GUI is shown

	return result;
}
