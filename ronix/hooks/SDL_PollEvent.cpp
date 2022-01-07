#include <ronix.hpp>

int SDLCALL Ronix::Hooks::SDL_PollEvent(SDL_Event *event)
{
	return ::SDL_PollEvent(event);
}
