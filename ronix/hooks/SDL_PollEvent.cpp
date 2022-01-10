#include <ronix.hpp>

using namespace Ronix::Data;

static void HandleToggle(SDL_Event *event, SDL_Scancode key, bool &var)
{
	if (SDL_GetScancodeFromKey(event->key.keysym.sym) == key)
		var = !var;
}

static int SDLCALL CustomPollEvent(SDL_Event *event)
{
	int result = ::SDL_PollEvent(event);
	if (result) {
		if (event->type == SDL_KEYDOWN && SDL_GetScancodeFromKey(event->key.keysym.sym) == config->data.guiKey)
			gui->ToggleVisibility();
		
		if (gui->IsVisible()) {
			if (event->type == SDL_KEYDOWN && gui->GetKeyListen()) {
				*gui->GetKeyListen() = SDL_GetScancodeFromKey(event->key.keysym.sym);
				gui->SetKeyListen(nullptr);
			}
			ImGui_ImplSDL2_ProcessEvent(event);
		} else if (gui->GetKeyListen()) {
			gui->SetKeyListen(nullptr);
		}

		if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
			keys[SDL_GetScancodeFromKey(event->key.keysym.sym)] = event->type;
			if (event->type == SDL_KEYDOWN && !gui->IsVisible()) {
				HandleToggle(event, config->data.autoStrafeToggleKey, config->data.autoStrafeEnable);
				HandleToggle(event, config->data.chamsToggleKey, config->data.chamsEnable);
				HandleToggle(event, config->data.espSnaplineToggleKey, config->data.espSnaplineEnable);
			}
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
