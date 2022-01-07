#include <ronix.hpp>

using namespace Ronix::Data;

static int SDLCALL CustomPollEvent(SDL_Event *event)
{
	int result = ::SDL_PollEvent(event);
	if (result) {
		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == config->data.guiKey)
			gui->ToggleVisibility();
		
		if (gui->IsVisible()) {
			if (event->type == SDL_KEYDOWN && gui->GetKeyListen()) {
				*gui->GetKeyListen() = event->key.keysym.sym;
				gui->SetKeyListen(nullptr);
			}
			ImGui_ImplSDL2_ProcessEvent(event);
		} else if (gui->GetKeyListen()) {
			gui->SetKeyListen(nullptr);
		}
	}

	return result;
}

int SDLCALL Ronix::Hooks::SDL_PollEvent(SDL_Event *event)
{
	int result;
	do {
		result = CustomPollEvent(event);
	} while (gui->IsVisible() && result); // Don't poll events for the game when the GUI is shown

	return result;
}
