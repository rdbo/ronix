#include <ronix.hpp>

void SDLCALL Ronix::Hooks::SDL_GL_SwapWindow(SDL_Window *window)
{
	::SDL_GL_SwapWindow(window);
}
