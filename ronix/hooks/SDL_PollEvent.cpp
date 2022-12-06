#include <ronix.hpp>

using namespace Ronix::Data;

static void HandleToggle(SDL_Event *event, Keybind *key, bool &var)
{
	if ((event->type == SDL_MOUSEBUTTONDOWN && key->type == Keybind::MOUSE && event->button.button == key->key) || (event->type == SDL_KEYDOWN && key->type == Keybind::KEYBOARD && SDL_GetScancodeFromKey(event->key.keysym.sym) == key->key))
		var = !var;
}

static int SDLCALL CustomPollEvent(SDL_Event *event)
{
	int result = ::SDL_PollEvent(event);
	if (result) {
		if (event->type == SDL_KEYDOWN) {
			static bool panicState = false;
			auto key = SDL_GetScancodeFromKey(event->key.keysym.sym);
			if (key == config->data.guiKey.key && !panicState) {
				gui->ToggleVisibility();
			} else if (key == config->data.panicToggleKey.key) {
				if (!panicState) {
					config->Save(".autosave");
					config->Reset();
					if (gui->IsVisible())
						gui->SetVisiblity(false);
					
					panicState = true;
				} else {
					config->Load(".autosave");
					panicState = false;
				}
			}
		}
		
		if (gui->IsVisible()) {
			Keybind *keybind = gui->GetKeyListen();
			if (event->type == SDL_KEYDOWN && keybind) {
				SDL_Scancode scancode = SDL_GetScancodeFromKey(event->key.keysym.sym);
				if (scancode != SDL_SCANCODE_ESCAPE) {
					keybind->type = Keybind::KEYBOARD;
					keybind->key = scancode;
				}
				gui->SetKeyListen(nullptr);
			} else if (event->type == SDL_MOUSEBUTTONDOWN && keybind) {
				keybind->type = Keybind::MOUSE;
				keybind->key = event->button.button;
				gui->SetKeyListen(nullptr);
			}
			ImGui_ImplSDL2_ProcessEvent(event);
		} else if (gui->GetKeyListen()) {
			gui->SetKeyListen(nullptr);
		}

		if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
			Uint32 evtype = event->type;
			Uint32 keyindex;
			if (evtype == SDL_MOUSEBUTTONDOWN || evtype == SDL_MOUSEBUTTONUP) {
				if (evtype == SDL_MOUSEBUTTONDOWN)
					evtype = SDL_KEYDOWN;
				else
					evtype = SDL_KEYUP;

				keyindex = event->button.button + KEYS_MOUSE_INDEX;
			} else {
				keyindex = SDL_GetScancodeFromKey(event->key.keysym.sym);
			}
			keys[keyindex] = evtype;
			if (evtype == SDL_KEYDOWN && !gui->IsVisible()) {
				HandleToggle(event, &config->data.autoStrafeToggleKey, config->data.autoStrafeEnable);
				HandleToggle(event, &config->data.chamsToggleKey, config->data.chamsEnable);
				HandleToggle(event, &config->data.espSnaplineToggleKey, config->data.espSnaplineEnable);
				HandleToggle(event, &config->data.triggerbotToggleKey, config->data.triggerbotEnable);
				HandleToggle(event, &config->data.espBoxToggleKey, config->data.espBoxEnable);
				HandleToggle(event, &config->data.rcsToggleKey, config->data.rcsEnable);
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
